#!/bin/sh
./client_recv localhost ${1:-8080} | play -t raw -b 16 -c 1 -e s -r 44100 -
