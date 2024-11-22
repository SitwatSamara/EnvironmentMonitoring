#!/bin/bash
# Run this script to start the data retrieval and logging
gcc -g -o env_monitoring main.c api_handler.c alerts.c -lcurl -lcjson `pkg-config --cflags --libs libnotify`
gcc -o env_monitoring main.c api_handler.c alerts.c -lcurl -lcjson `pkg-config --cflags --libs libnotify`
./env_monitoring
