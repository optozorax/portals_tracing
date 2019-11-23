#!/bin/bash
chmod +x run
chmod +x scene_raytracing
chmod +x client.py
cp client.service /etc/systemd/system
systemctl daemon-reload
systemctl stop client.service
systemctl start client.service
journalctl -u client.service -f --output=cat