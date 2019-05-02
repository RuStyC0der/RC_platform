import sys
from PyQt5 import QtGui, QtWidgets, uic, QtCore
from PyQt5.QtWidgets import QWidget, qApp
from PyQt5.QtCore import pyqtSignal, QEvent


class QLabelExample(QtWidgets.QMainWindow):

    def __init__(self, parent=None):

        super(QLabelExample, self).__init__(parent)
        self.ui = uic.loadUi("GUI.ui")
        self.ui.label.setText("lvccvol")
        qApp.installEventFilter(self)
        self.ui.show()

    def eventFilter(self, obj, event):
        if event.type() == QEvent.KeyPress:
            if event.key() == QtCore.Qt.Key_A:
                self.ui.label.setText('sfsfgs')
        return super(QLabelExample, self).eventFilter(obj, event)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = QLabelExample()
    sys.exit(app.exec_())
