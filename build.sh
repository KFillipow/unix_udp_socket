#!/bin/bash

echo "Building server and client"
gcc client.c -lpthread -o client
gcc server.c -lpthread -o server
echo "Done building"
