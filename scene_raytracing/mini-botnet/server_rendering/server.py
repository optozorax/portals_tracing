#!/usr/bin/python3 -u

from flask import Flask, request
import json
import base64
from pathlib import Path
from random import shuffle
import os
import time
import subprocess
import logging
import argparse

total_frames = 2023
total_to_process = 0

drawed = 0
to_process = []
sended_to_process = []

prefix_folder = "output/"

start_time = time.time()

app = Flask(__name__)

files_to_send = ["settings.json"]

prefix = "scene.json_"

intervals = (
    ('w', 604800),  # 60 * 60 * 24 * 7
    ('d', 86400),   # 60 * 60 * 24
    ('h', 3600),    # 60 * 60
    ('m', 60),
    ('s', 1),
)

def from_seconds(seconds):
    result = []

    for name, count in intervals:
        value = seconds // count
        if value:
            seconds -= value * count
            result.append("{}{}".format(int(value), name))
    return ' '.join(result)

def print_header():
    print("|{:<11}|{:>11}|{:>11}|{:>11}|".format("percent", "time passed", "approx time", "left time"))
    print("|{:>11}|{:>11}|{:>11}|{:>11}|".format("-----------", "-----------", "-----------", "-----------"))

def print_percent(percent, time_passed):
    approx_time = time_passed/percent
    left_time = approx_time - time_passed
    print("|{:>10.1f}%|{:>11}|{:>11}|{:>11}|".format(percent*100, from_seconds(time_passed), from_seconds(approx_time), from_seconds(left_time)))

def number_to_name(number):
    return prefix + str(number) + ".png"

def name_to_number(name):
    return int(name[len(prefix):name.find('.png')])

def run_bash_command_simple(command):
    os.system(command)

def run_bash_command(command):
    subprocess.call(command, shell=True)

@app.route('/', methods=['POST'])
def post():
    global drawed
    global to_process
    global sended_to_process
    for filename in request.files:
        frame = name_to_number(filename)

        if len([a for a in sended_to_process if a["frame"] == frame]) == 0:
            return 'You was render already rendered frame.'

        sended_to_process = [a for a in sended_to_process if a["frame"] != frame]

        file = request.files[filename]
        file.save("./output/" + filename)

        drawed += 1

        percent = float(drawed)/total_to_process
        print_percent(percent, time.time() - start_time)

    return 'Received'

def edit_settings(frame):
    run_bash_command("cat settings.json | jq '.start_frame.object = {start}' | jq '.end_frame.object = {end}' > settings1.json; rm settings.json; mv settings1.json settings.json".format(start=frame, end=frame+1))

@app.route('/client.tar.gz')
def send_archive():
    return app.send_static_file('client.tar.gz')

@app.route('/what_to_do')
def send_client_instructions():
    return app.send_static_file('what_to_do')

@app.route('/files', methods=['GET'])
def files():
    result = dict()
    files = dict()

    for filename in ["cam_positions.json", "scene.json"]:
        with open(filename, 'rb') as f:
            encoded = base64.encodebytes(f.read())
            files[filename] = encoded.decode('ascii')

    result["files"] = files
    return json.dumps(result)

@app.route('/', methods=['GET'])
def get():
    global drawed
    global to_process
    global sended_to_process
    frame = 0
    if len(to_process):
        frame = to_process[0]
        to_process.remove(frame)
    else:
        if len(sended_to_process):
            first = sended_to_process[0]
            frame = first["frame"]
            sended_to_process.remove(first)
        else:
            raise KeyboardInterrupt

    edit_settings(frame)

    send_to_process = dict()
    send_to_process["time"] = time.time()
    send_to_process["frame"] = frame
    sended_to_process.append(send_to_process)

    result = dict()
    files = dict()

    result["linux_commands"] = ["./scene_raytracing"]
    result["windows_commands"] = ["scene_raytracing.exe"]
    result["send_file"] = number_to_name(frame)

    for filename in files_to_send:
        with open(filename, 'rb') as f:
            encoded = base64.encodebytes(f.read())
            files[filename] = encoded.decode('ascii')

    result["files"] = files
    return json.dumps(result)

parser = argparse.ArgumentParser()
parser.add_argument("-f", "--folder", type=str,
                    help="output folder",
                    default="output")
args = parser.parse_args()
run_bash_command("mkdir {}".format(args.folder))

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
log.propagate = False
log.disabled = True

to_process = [i for i in range(total_frames)]
to_process_copy = [i for i in to_process]
for i in to_process_copy:
    filename = prefix_folder + number_to_name(i)
    my_file = Path(filename)
    if my_file.exists():
        to_process.remove(i)

shuffle(to_process)
total_to_process = len(to_process)
start_time = time.time()

print("Files to rendering: {}".format(len(to_process)))

print_header()

try:
    app.run(port=80, host='0.0.0.0', threaded=False, processes=1)
    print("Done")
except KeyboardInterrupt:
    print("Done")

print("It took {}".format(from_seconds(time.time()-start_time)))
