#!/bin/sh
rec -t raw -b 16 -c 1 -e s -r 44100 - | ./serv_send ${1:-8080}
