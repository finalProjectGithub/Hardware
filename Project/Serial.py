#!/usr/bin/env python3

import serial
import time
import firebase as fb


ser = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1.0)
time.sleep(3)
ser.reset_input_buffer()
print("Serial OK")

try:
    while True:
        time.sleep(0.1)
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            line = int(line)
            print(line)
            fb.sendData(line)
            
except KeyboardInterrupt:
    print("Close Serial Communication")
    ser.close()
