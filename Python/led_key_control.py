import serial
from pynput.keyboard import Listener, Key
from time import sleep
ser = serial.Serial(
  port='/dev/ttyUSB0',
  baudrate = 9600,
  parity=serial.PARITY_NONE,
  stopbits=serial.STOPBITS_ONE,
  bytesize=serial.EIGHTBITS,
  timeout=1
)
sleep(2)

pressed = []

def send():
    global pressed


    if Key.f1 in pressed and Key.f2 not in pressed:
        ser.write(b'1 0')
    elif Key.f1 not in pressed and Key.f2 in pressed:
        ser.write(b'0 1')
    else:
        print("relese")
        ser.write(b'0 0')

def on_press(key):
    if str(key) not in pressed:
        pressed.append(key)
    print("rel")
    send()


def on_release(key):
    if str(key) in pressed:
        pressed.remove(key)
    # send()
    ser.write(b'0 0')

print(123)
# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
