gcc -Wall bandpass.c -o bandpass -lm
sox seiji.wav -t raw -c 1 - | ./bandpass 8192 500 1800 > out.raw
