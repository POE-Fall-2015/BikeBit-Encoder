import serial
import sys
import time

ser = serial.Serial('COM6', 9600)
lastLine = ''
name = 'data.txt'
startTime = time.time()
with open(name, 'a') as myfile:
    currentTime = time.time()
    timeDiff = currentTime-startTime
    while True and timeDiff < 20:
        currentLine = ser.readline()
        if currentLine != lastLine:
            print currentLine
            lastLine = currentLine
            myfile.write(time.ctime()+ "," + currentLine)
#name = 'data.txt'
#open(name,'a')
