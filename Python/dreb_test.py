import sys
from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtWidgets import QWidget, QMainWindow
from PyQt5.QtCore import pyqtSignal
import serial
from time import sleep


# class Logic():
    # def __init__(self,triger_keys=[87, 83, 65, 68]): # triger_keys={"up":87, "down":83, "left":65, "right":68}
    #     self.keys = triger_keys
    #     self.port = None
    # def open_port(self,port, baudrate=9600):
    #     try:
    #         self.port = serial.Serial(port=port, baudrate=baudrate)
    #     except serial.serialutil.SerialException:
    #         raise Exception("bad_port")
    # def precompile(self,keys_presed):
    #     code = "$"
    #     for i in self.keys:
    #         if i in keys_presed:
    #             code += "1 "
    #         else:
    #             code += "0 "
    #     return code.strip()+";"
    # def send_code(self, code):
    #     self.port.write(code.encode())


class QLabelExample(QMainWindow):
    def __init__(self, parent=None):
        self.keys= []
        super(QLabelExample, self).__init__(parent)
        # Set up the user interface from Designer.
        uic.loadUi("GUI.ui", self)



        self.setWindowTitle("Manipulator")

        # self.keyW.keyPressed.connect(lambda : self.ui.label.setText("1"))
        # self.keyW.keyRelease.connect(lambda : self.ui.label.setText("2"))
        self.show()
    def keyPressEvent(self, event):
        if event.isAutoRepeat():
            return
        print('1')



        self.update()


    def keyReleaseEvent(self, event):
        if event.isAutoRepeat():
            return
        print('0')
        self.update()



if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = QLabelExample()
    sys.exit(app.exec())
