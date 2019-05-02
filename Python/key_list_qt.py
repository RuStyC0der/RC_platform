#  KeyboardInputDemoQt.py  (c) Kari Laitinen

#  http://www.naturalprogramming.com

#  2009-12-02  File created.
#  2009-12-09  Last modification.

#  This program shows how to react to key pressings
#  in a Qt program.

#  http://www.riverbankcomputing.co.uk/static/Docs/PyQt4/html/qkeyevent.html

# import sys


import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QWidget


class KeyboardInputDemoWindow(QWidget):

    def __init__(self, parent=None):

        QWidget.__init__(self, parent)

        self.setGeometry(100, 200, 800, 400)

        self.setWindowTitle("PRESS THE KEYS")
        # self.setAutoRepeat(False)ssssssswww

        self.lab = QtWidgets.QLabel("123", self)

    #  The following methods will be called by the program
    #  execution system whenever keys of the keyboard are pressed.
    #  They receive a QKeyEvent object as a parameter.

    def keyPressEvent(self, event):

        self.lab.setText(str(int(event.key())))
        self.update()

    def keyReleaseEvent(self, event):

        self.lab.setText("")
        self.update()




#  The main program begins.

this_application = QtWidgets.QApplication(sys.argv)

application_window = KeyboardInputDemoWindow()

application_window.show()

this_application.exec_()
