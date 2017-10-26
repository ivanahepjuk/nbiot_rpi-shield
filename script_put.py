import requests
import serial
import re

ser = serial.Serial('/dev/ttyACM3', 9600, timeout=61, xonxoff=False, rtscts=False, dsrdtr=False) #Tried with and without the 	last 3 parameters, and also at 1Mbps, same happens.
ser.flushInput()
ser.flushOutput()

while True:
	
  data_raw = ser.readline()
	
  (timestamp, teplota, tlak, vlhkost, pm1_1, pm25_1, pm10_1, pm1_2, pm25_2, pm10_2, pm1_3, pm25_3, pm10_3) = [t(s) for t,s in zip((int, float,  float, float, float, float, float, float, float, float, float, float, float), data_raw.split())]



	#teplota
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_temp/" + str(teplota)
  r = requests.put(string, data=0)
  print r

  #tlak
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_press/" + str(tlak)
  r = requests.put(string, data=0)
  print r
  

  #vlhkost
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_hum/" + str(vlhkost)
  r = requests.put(string, data=0)
  print r
 

  #pm1_1
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm1_1/" + str(pm1_1)
  r = requests.put(string, data=0)
  print r


  #pm25_1
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm25_1/" + str(pm25_1)
  r = requests.put(string, data=0)
  print r


  #pm10_1
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm10_1/" + str(pm10_1)
  r = requests.put(string, data=0)
  print r


 

  #pm1_2
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm1_2/" + str(pm1_2)
  r = requests.put(string, data=0)
  print r


  #pm25_2
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm25_2/" + str(pm25_2)
  r = requests.put(string, data=0)
  print r


  #pm10_2
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm10_2/" + str(pm10_2)
  r = requests.put(string, data=0)
  print r
  


  #pm1_3
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm1_3/" + str(pm1_3)
  r = requests.put(string, data=0)
  print r


  #pm25_3
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm25_3/" + str(pm25_3)
  r = requests.put(string, data=0)
  print r


  #pm10_3
  string = "http://labka.stud.slu.cz/sensoricnet/api/insert/labka0001_pm10_3/" + str(pm10_3)
  r = requests.put(string, data=0)
  print r

  print "all sent"
