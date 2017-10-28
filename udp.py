#!/usr/bin/env python

import socket
import serial
import time

#UDP settings:
UDP_IP = "89.103.47.53" #89.102.98.39"
UDP_PORT = 8089


#serialport settings:
ser = serial.Serial('/dev/ttyACM3', 9600, timeout=61, xonxoff=False, rtscts=False, dsrdtr=False)

#serial flushing
time.sleep(0.5)
ser.flushInput()
ser.flushOutput()


while True:
	#precte radek zakonceny \n znakem

#"hodnoty,box=Adrspach temp=%.2f,hum=%.2f,pres=%.2f,pm1=%.2f,pm25=%.2f,pm10=%.2f,time=%d\r\n"
  #funkcni dva radky..!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:
  #UDP_MESSAGE = ser.readline()
  #UDP_MESSAGE = UDP_MESSAGE[:-2]	# cutting off the \r\n from the end of serialport-readed data
  UDP_MESSAGE = 'hodnoty,box=Adrspach temp=21.21,hum=21.21,pres=500.50,pm1=12.32,pm25=12.32,pm10=12.33,time=1111111'

  print "UDP target IP:", UDP_IP
  print "UDP target port:", UDP_PORT
  print "message:", UDP_MESSAGE

  sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
  sock.sendto(UDP_MESSAGE, (UDP_IP, UDP_PORT))


