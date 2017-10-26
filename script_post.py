import requests
import serial
import re
import json
import time

#nastaveni serioveho portu
ser = serial.Serial('/dev/ttyACM3', 9600, timeout=61, xonxoff=False, rtscts=False, dsrdtr=False) #Tried with and without the 	last 3 parameters, and also at 1Mbps, same happens.

#toto je nutne, aby se vycistil vzduch v seriovem potrubi
time.sleep(0.5)
ser.flushInput()
ser.flushOutput()

while True:
	#precte radek zakonceny \n znakem
  data_raw = ser.readline()

	#  timestamp,  teplota,    tlak,   vlhkost
  #  pm1,        pm25,       pm10,   flow,
  #  sampling_period,        checksum,
  #  bin1,       bin2,       bin3,   bin4,
  #  bin5,       bin6,       bin7,   bin8,
  #  bin9,       bin10,      bin11,  bin12,
  #  bin13,      bin14,      bin15,  bin16,  
	
  (timestamp, teplota, tlak, vlhkost, pm01, pm25, pm10, flow, sampling_period, checksum, bin1, bin2, bin3, bin4, bin5, bin6, bin7, bin8, bin9, bin10, bin11, bin12, bin13, bin14, bin15, bin16) = [t(s) for t,s in zip((int, float,  float, float, float, float, float, float, float, int, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float), data_raw.split())]


#zrobi payload tak ze vyparsuje z prijateho retezce, propoji stringy tak jak to potrebuje json a sestavi tu strukturu

  #Sem to Petr chce prijimat
  url = 'http://192.168.1.8:3000/sensor/data/batch/json'
  
  timestamp = time.strftime ( "%Y-%m-%dT%H:%M:%S" )

  #udela jeden veliky string
  payload = { \
    'LABKA_SENSOR1_TEMP':{ 'uid': 'LABKA_SENSOR1_TEMP', 'datetime': str(timestamp), 'value': str(teplota)}, \
    'LABKA_SENSOR1_PRESS':{ 'uid': 'LABKA_SENSOR1_PRESS', 'datetime': str(timestamp), 'value': str(tlak)}, \
    'LABKA_SENSOR1_HUMI':{ 'uid': 'LABKA_SENSOR1_HUMI', 'datetime': str(timestamp), 'value': str(vlhkost) }, \
    'LABKA_SENSOR1_PM01':{ 'uid': 'LABKA_SENSOR1_PM01','datetime': str(timestamp), 'value': str(pm01)}, \
    'LABKA_SENSOR1_PM25':{ 'uid': 'LABKA_SENSOR1_PM25','datetime': str(timestamp), 'value': str(pm25)}, \
    'LABKA_SENSOR1_PM10':{ 'uid':'LABKA_SENSOR1_PM10','datetime': str(timestamp), 'value': str(pm10)}}

  print payload

  #sestavi hlavicku tak jak petr chtel, snad
  hlavicka = {'Content-Type': 'application/json'}
  #print payload
  #praskne to tam.. pouzil sem to podle stackoverflow
  response = requests.post(url, json=payload)
  
  #vypise odpoved serveru
  print response.text

  #profit



'''
  payload = """{LABKA_SENSOR1_TEMP:{ uid: 'LABKA_SENSOR1_TEMP', datetime: """ + str(timestamp) + """, value: """ + str(teplota) + """},LABKA_SENSOR1_PRESS:{ uid: 'LABKA_SENSOR1_PRESS', datetime: """ + str(timestamp) + """, value: """ + str(tlak) + """},LABKA_SENSOR1_HUMI:{ uid: 'LABKA_SENSOR1_HUMI', datetime: """ + str(timestamp) + """, value: """ + str(vlhkost) + """},LABKA_SENSOR1_PM01:{ uid: 'LABKA_SENSOR1_PM01',datetime: """ + str(timestamp) + """, value: """ + str(pm01) + """},LABKA_SENSOR1_PM25:{ uid: 'LABKA_SENSOR1_PM25',datetime: """ + str(timestamp) + """, value: """ + str(pm25) + """},LABKA_SENSOR1_PM10:{ uid:'LABKA_SENSOR1_PM10',datetime: """ + str(timestamp) + """, value: """ + str(pm10) + """}\n}"""
'''

