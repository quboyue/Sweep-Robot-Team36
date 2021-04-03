import time
import serial 

ser = serial.Serial(  # 下面这些参数根据情况修改
  port='COM6',# 串口
  baudrate=115200,# 波特率
  #parity=serial.PARITY_ODD,
  #stopbits=serial.STOPBITS_TWO,
  #bytesize=serial.SEVENBITS

)
every_time = time.strftime('%Y-%m-%d %H:%M:%S')# 时间戳
data = ''

while True:
  data = ser.readline()
  print(every_time, data)
