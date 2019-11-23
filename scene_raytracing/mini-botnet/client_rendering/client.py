#!/usr/bin/python3 -u

import requests
import base64
import json
import codecs
from urllib.request import urlopen
import time
import subprocess

#url = 'http://217.71.129.139:4207' # Урл, если с обычного компа
url = "http://172.17.2.167:80" # Урл, если с локальной сети cloud.nstu.ru
is_windows = False

def run_bash_command(command):
	subprocess.call(command, shell=True)

def reload_files():
	is_finished = False
	while not is_finished:
		try:
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

after_wait = True
while True:
	try:
		if after_wait:
			reload_files()
			after_wait = False

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

		for command in commands:
			run_bash_command(command)

		send_file = data["send_file"]
		with open(send_file, 'rb') as f:
			r = requests.post(url, files={send_file: f})
			print(r.text)

		if is_windows:
			run_bash_command("del {}".format(send_file))
			run_bash_command("del settings.json")
		else:
			run_bash_command("rm {}".format(send_file))
			run_bash_command("rm settings.json")

		print("Sended file!")
	except Exception as e:
		print(e)
		print("Wait...")
		time.sleep(10)
		after_wait = True
