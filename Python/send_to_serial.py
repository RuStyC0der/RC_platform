# import serial
from time import sleep
# # ser = serial.Serial("/dev/ttyUSB0",9600)
# # ser.open()
# ser = serial.Serial(
#   port='/dev/ttyUSB0',
#   baudrate = 9600,
#   parity=serial.PARITY_NONE,
#   stopbits=serial.STOPBITS_ONE,
#   bytesize=serial.EIGHTBITS,
#   timeout=1)
# # while True:
#
#
#
# ser.write()
import serial
ser = serial.Serial(
  port=None,
  baudrate = 9600,
  parity=serial.PARITY_NONE,
  stopbits=serial.STOPBITS_ONE,
  bytesize=serial.EIGHTBITS,
  timeout=1
)
# sleep(2)
# print ("Serial is open: " + str(ser.isOpen()))
# print ("Now Writing")
# ser.write(b'1 1')
# print ("Did write, now read")
# x = ser.readline()
# print (x)
# sleep(1)
# ser.write(b'0 0')
# print ("Did write, now read")
# x = ser.readline()
# print (x)
ser.close()
