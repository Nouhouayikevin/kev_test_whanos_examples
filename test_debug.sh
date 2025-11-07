#!/bin/bash

# Kill any existing instances
pkill -f rtype_server
pkill -f rtype_client

# Start server in background
cd /home/rold/G-CPP-500-COT-5-1-rtype-24
build/Server/rtype_server 4242 2>&1 | grep -E "(Server Serialize|Component type:|Entity.*has)" &
SERVER_PID=$!

# Wait for server to start
sleep 2

# Run client and capture output
timeout 20  build/Client/rtype_client 2>&1 | grep -E "(DEBUG:|Warning:|Component|Entity)" | head -30

# Kill server
kill $SERVER_PID 2>/dev/null

echo "Test complete"
