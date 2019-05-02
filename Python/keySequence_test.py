import sys
from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtGui import QKeySequence
from PyQt5.QtWidgets import QWidget, QMainWindow, QMessageBox, QApplication

class GUI(QMainWindow):
    def __init__(self, parent=None):
        self.pressed_keys= []
        super(GUI, self).__init__(parent)
        uic.loadUi("key_seq_test.ui", self)
        self.keySequenceEdit.editingFinished.connect(self.seq)
        seq = QKeySequence(49)
        self.keySequenceEdit.setKeySequence(seq)
        self.show()

    def seq(self):
        print(self.keySequenceEdit.keySequence().__getitem__(0))
        print(type(self.keySequenceEdit.keySequence().__getitem__(0)))

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    window = GUI()
    sys.exit(app.exec())
