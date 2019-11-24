#!/usr/bin/python3 -u

import requests
import base64
import json
import codecs
from urllib.request import urlopen
import time
import subprocess

url = 'http://url'
is_windows = True

def delete_file(file):
	if is_windows:
		run_bash_command("del {}".format(file))
	else:
		run_bash_command("rm {}".format(file))

def run_bash_command(command):
	subprocess.call(command, shell=True)

def reload_files():
	is_finished = False
	while not is_finished:
		try:
			print("Start receiving files for rendering")
			response = urlopen(url + "/files").read().decode('utf8')
			data = json.loads(response)
			files = data["files"]
			for i in files:
				file_content = base64.b64decode(files[i])
				with open(i, 'wb') as f:
					f.write(file_content)
			is_finished = True
		except Exception as e:
			print(e)
			print("Wait for set new files...")
			time.sleep(10)
			is_finished = False

try:
	after_wait = True
	while True:
		try:
			if after_wait:
				reload_files()
				after_wait = False

			print("Start receiving settings for rendering")
			response = urlopen(url).read().decode('utf8')

			if response == "stop_rendering":
				raise Exception("Rendering is DONE")
		
			data = json.loads(response)
			files = data["files"]
			for i in files:
				file_content = base64.b64decode(files[i])
				with open(i, 'wb') as f:
					f.write(file_content)

			commands = []
			if is_windows:
				commands = data["windows_commands"]
			else:
				commands = data["linux_commands"]

			print("Run rendering")

			for command in commands:
				run_bash_command(command)

			send_file = data["send_file"]
			with open(send_file, 'rb') as f:
				r = requests.post(url, files={send_file: f})
				print(r.text)

			delete_file(send_file)
			delete_file("settings.json")

			print("Sended file!")
		except Exception as e:
			print(e)
			print("Wait...")
			delete_file("scene.json")
			delete_file("cam_positions.json")
			time.sleep(10)
			after_wait = True
except KeyboardInterrupt:
	print("Client stopped")
	delete_file("scene.json")
	delete_file("cam_positions.json")
