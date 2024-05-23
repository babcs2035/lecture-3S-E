rec -t wav -b 8 -c 1 - | nc -v app.ktak.dev 50000
# rec -t raw -b 8 -c 1 -e s -r 44100  - | nc -v -u localhost 50000
