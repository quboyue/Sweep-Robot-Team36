#coding:utf-8

import socket
import RPi.GPIO as GPIO
import _thread
import time

GPIO.setmode(GPIO.BOARD)

GPIO.setup(16, GPIO.OUT)
GPIO.setup(18,GPIO.OUT)
#GPIO.output(18,GPIO.HIGH) #10 front 01back 11 right

GPIO.setup(13, GPIO.OUT)
GPIO.setup(7,GPIO.OUT)
#GPIO.output(7,GPIO.LOW)
#udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  
#udp.bind(('127.0.0.1', 7403))
#receive=""
#last_receive=""
run=0
GPIO.setwarnings(False)
def gpio_driver():
   while(1):
    global run
    if(run):
     GPIO.output(16, GPIO.HIGH)
     GPIO.output(13, GPIO.HIGH)
#     time.sleep(0.002)
     GPIO.output(16, GPIO.LOW)
     GPIO.output(13, GPIO.LOW)
#     time.sleep(0.002)

#_thread.start_new_thread( gpio_driver, () )

#run=1

#GPIO.output(18, GPIO.HIGH)
#GPIO.output(7, GPIO.HIGH)

#gpio_driver()

def rec_fun():
   udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   udp.bind(('127.0.0.1', 7403))
   receive=""
   last_receive=""
   global run
   while(1):
    data, addr = udp.recvfrom(15)  # 返回数据以及发送数据的地
    receive=str(data.decode('utf-8'))
    if receive==last_receive:
       continue
    print("status ",receive)
    if(receive=="front"):
      run=0
      GPIO.output(18, GPIO.HIGH)
      GPIO.output(7, GPIO.LOW)
      time.sleep(0.005)
      run=1
    if(receive=="left "):
      run=0
      GPIO.output(18, GPIO.LOW)
      GPIO.output(7, GPIO.LOW)
      time.sleep(0.005)
      run=1
    if(receive=="right"):
      run=0
      GPIO.output(18, GPIO.HIGH)
      GPIO.output(7, GPIO.HIGH)
      time.sleep(0.005)
      run=1
    if(receive=="stop "):
      run=0
    last_receive=receive



_thread.start_new_thread( rec_fun, () )

while(1):
   if(run):
    GPIO.output(16, GPIO.HIGH)
    GPIO.output(13, GPIO.HIGH)
    time.sleep(0.002)
    GPIO.output(16, GPIO.LOW)
    GPIO.output(13, GPIO.LOW)
    time.sleep(0.002)


   
udp.close()

