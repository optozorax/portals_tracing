#!/bin/bash -u
client=../client_rendering
if [ -d "$client" ]; then
   tar -zcvf static/client.tar.gz $client
fi
chmod +x run
chmod +x server.py
cp server.service /etc/systemd/system
systemctl daemon-reload
systemctl stop server.service
systemctl start server.service
journalctl -u server.service -f --output=cat