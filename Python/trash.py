import sys
from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtWidgets import QWidget, QMainWindow


class GUI(QMainWindow):
    def __init__(self, parent=None):
        super(GUI, self).__init__(parent)

        uic.loadUi("GUI.ui", self)
        self.show()

        print((self.connect_button.isChecked()))
        # print(help(self.connect_button))

    def keyPressEvent(self, event):
        print("pressed")
        self.update()


    def keyReleaseEvent(self, event):
        print("relesed")
        self.update()



if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = GUI()
    sys.exit(app.exec())
