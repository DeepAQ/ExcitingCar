#!/bin/sh
g++ -o excitinghttpd -O3 -lwiringPi -lmicrohttpd http.cpp libGPIO.a
