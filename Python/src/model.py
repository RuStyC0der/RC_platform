
import serial
from time import sleep
import serial.tools.list_ports
# import serial.tools.list_ports
# print(list(serial.tools.list_ports.comports())) # List available com ports

class Singleton():
    __obj = False  # Private class variable.

    def __new__(cls,*args, **kwargs):
        if cls.__obj:
            return cls.__obj
        cls.__obj = super(Singleton, cls).__new__(cls)
        return cls.__obj

class Model(Singleton):
    def __init__(self,triger_keys=[87, 83, 65, 68]): # triger_keys={"up":87, "down":83, "left":65, "right":68}
        self.keys = triger_keys
        self.port = False

    def auto_connect(self):
        for i in [str(j).split(" ")[0] for j in serial.tools.list_ports.comports()]:
            try:
                self.port = serial.Serial(port=i, baudrate=9600)
                return i
                break
            except:
                continue
        else:
            return False


    def connect(self,port, baudrate=9600):
        try:
            self.port = serial.Serial(port=port, baudrate=baudrate)
        except serial.serialutil.SerialException:
            raise Exception("bad_port")

    def check_port(self):
        if not self.port:
            return False
        if not self.port.isOpen():
            return False
        return True

    def precompile(self,keys_presed, speed,sensivity):
        code = "$"
        for i in self.keys:
            if i in keys_presed:
                code += "1 "
            else:
                code += "0 "
        code += str(speed) + " "
        code += str(sensivity) + " "
        return code.strip()+";"

    def send_code(self, code):
        print("sending: ", code)
        self.port.write(code.encode())
# print(list(serial.tools.list_ports.comports()))
