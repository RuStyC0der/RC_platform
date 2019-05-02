import sys
# from PyQt5.QtCore import QT_TR_NOOP
from PyQt5 import QtGui, QtWidgets, uic
from PyQt5.QtGui import QKeySequence
from PyQt5.QtWidgets import QWidget, QMainWindow, QMessageBox, QApplication
# from PyQt5.QtCore import pyqtSignal
from model import Model as model
from time import sleep
import serial.tools.list_ports
import pickle



# self.speed_slider.value() # get slider value

# self.auto_connect_flag.checkState() #get checkBox value (0 or 2 if checked)

# self.comPorts.addItems(["2","3"]) # add item list to checkBox
# self.comPorts.addItem("1", userData="33") # add item with userData

# print(self.comPorts.currentIndex())
# print(self.comPorts.currentData())
# print(self.comPorts.currentText())

class Help(QMainWindow):
    def __init__(self, parent=None):
        super(Help, self).__init__(parent)
        # self.setWindowTitle("Help")
        uic.loadUi("Help.ui", self)

class Settings(QMainWindow):
    def __init__(self, parent=None):
        super(Settings, self).__init__(parent)
        uic.loadUi("Settings.ui", self)
        self.cancel.clicked.connect(self.close)
        self.ok.clicked.connect(self.save_config)
        self.default_btn.clicked.connect(self.set_default)
        self.parent = parent
        self.up_key.setKeySequence(QKeySequence(parent.model.keys[0]))
        self.down_key.setKeySequence(QKeySequence(parent.model.keys[1]))
        self.left_key.setKeySequence(QKeySequence(parent.model.keys[2]))
        self.right_key.setKeySequence(QKeySequence(parent.model.keys[3]))


    def save_config(self, reload_flag=True):
        conf = {"keys":[self.up_key.keySequence().__getitem__(0),
                        self.down_key.keySequence().__getitem__(0),
                        self.left_key.keySequence().__getitem__(0),
                        self.right_key.keySequence().__getitem__(0)],
                "sliders":[self.parent.speed_slider.value(),
                        self.parent.sensitivity_slider.value()]}
        with open("config.bin","wb") as config :
            pickle.dump(conf, config)
        if reload_flag:
            self.parent.model = model(conf["keys"])
        self.close()
    def set_default(self):
        self.parent.model = model()
        self.save_config(False)
        self.up_key.setKeySequence(QKeySequence(self.parent.model.keys[0]))
        self.down_key.setKeySequence(QKeySequence(self.parent.model.keys[1]))
        self.left_key.setKeySequence(QKeySequence(self.parent.model.keys[2]))
        self.right_key.setKeySequence(QKeySequence(self.parent.model.keys[3]))



class GUI(QMainWindow):
    def __init__(self, parent=None):
        self.pressed_keys= []
        super(GUI, self).__init__(parent)
        uic.loadUi("GUI.ui", self)
        try:
            with open("config.bin","rb") as conf :
                config = pickle.load(conf)
                self.model = model(config["keys"])
                self.speed_slider.setValue(config["sliders"][0])
                self.sensitivity_slider.setValue(config["sliders"][1])
        except:
            self.model = model()



        self.setWindowIcon(QtGui.QIcon('3.png'))



        self.settings = Settings(parent=self)
        self.help = Help(parent=self)

        # self.comPorts.addItem("1", userData="33")
        self.refresh_button.setIcon(QtGui.QIcon("refresh.png"))
        self.refresh_button.setIconSize(self.refresh_button.rect().size())

        self.refresh_button.clicked.connect(self.refresh_com)
        self.connect_button.clicked.connect(self.connect)
        self.butt_mode_flag.stateChanged.connect(self.controll_btn_ll)
        self.auto_connect_flag.stateChanged.connect(lambda: self.comPorts.setEnabled(not (self.comPorts.isEnabled())))
        self.up_btn.clicked.connect(self.up)
        self.down_btn.clicked.connect(self.down)
        self.left_btn.setAutoDefault(True)
        self.right_btn.setAutoDefault(True)
        self.left_btn.pressed.connect(lambda: self.send_from_buttons([1, 0]))
        self.left_btn.released.connect(lambda: self.send_from_buttons([0, 0]))
        self.right_btn.pressed.connect(lambda: self.send_from_buttons([0, 1]))
        self.right_btn.released.connect(lambda: self.send_from_buttons([0, 0]))

        self.actionQuit.triggered.connect(self.close)
        self.actionSettings.triggered.connect(self.settings.show)
        self.actionHelp.triggered.connect(self.help.show)
        self.actionAbout.triggered.connect(self.about)


        self.connect_button.setStatusTip(self.connect_button.text() + " Arduino")
        self.refresh_button.setStatusTip("Refresh port list")

        self.refresh_com()

        self.show()

    def keyPressEvent(self, event):
        if event.isAutoRepeat() or self.butt_mode_flag.isChecked() or not self.model.check_port():
            return
        print("pressed")
        self.pressed_keys.append(int(event.key()))
        speed = self.speed_slider.value()
        sens = self.sensitivity_slider.value()
        code = self.model.precompile(self.pressed_keys, speed, sens)
        try:
            self.model.send_code(code)
        except:
            QMessageBox.warning(self, "warning", "please reconnect controller")
            self.disconnect()

        self.update()


    def keyReleaseEvent(self, event):
        if event.isAutoRepeat() or self.butt_mode_flag.isChecked() or not self.model.check_port():
            return
        print("relesed")
        self.pressed_keys.remove(int(event.key()))
        speed = self.speed_slider.value()
        sens = self.sensitivity_slider.value()
        code = self.model.precompile(self.pressed_keys, speed, sens)
        try:
            self.model.send_code(code)
        except:
            QMessageBox.warning(self, "warning", "please reconnect controller")
            self.disconnect()


        self.update()

    def closeEvent(self, event):
        try:
            if self.model.port:
                self.model.send_code("$" + "0 " * 6 + ";")
                self.model.port.close()
                self.settings.save_config()
        except:
            pass
        finally:
            event.accept()



    def refresh_com(self):
        for i in range(self.comPorts.count()):
            self.comPorts.removeItem(0)
        for i in list(serial.tools.list_ports.comports()):
            self.comPorts.addItem(str(i).split(" ")[0])

    def about(self):
        QMessageBox.information(self, "About", "Created by max")

    def connect(self):
        # print("*",self.comPorts.currentIndex(),"*")
        if self.auto_connect_flag.checkState():
            port = self.model.auto_connect()
            if not port:
                self.statusBar().showMessage("Error, plug arduino or try to connect manualy")
                return
            self.statusBar().showMessage("connected to " + str(port))
        elif self.comPorts.currentIndex() != -1:
            print("value2")
            try:
                self.model.connect(self.comPorts.currentText())
            except:
                self.refresh_com()
                return
        else:
            self.statusBar().showMessage("Empty port")
            return
        print("**")
        self.speed_slider.setEnabled(True)
        self.sensitivity_slider.setEnabled(True)
        self.speed_l.setEnabled(True)
        self.sensitivity_l.setEnabled(True)
        self.butt_mode_flag.setEnabled(True)
        self.comPorts.setEnabled(False)
        self.refresh_button.setEnabled(False)
        self.auto_connect_flag.setEnabled(False)


        self.connect_button.setText("Disconnect")
        self.connect_button.setStatusTip(self.connect_button.text() + " Arduino")

        self.connect_button.clicked.connect(self.disconnect)

    def disconnect(self):
        print("///")
        self.speed_slider.setEnabled(False)
        self.sensitivity_slider.setEnabled(False)
        self.speed_l.setEnabled(False)
        self.sensitivity_l.setEnabled(False)
        self.butt_mode_flag.setEnabled(False)
        self.comPorts.setEnabled(True)
        self.refresh_button.setEnabled(True)
        self.auto_connect_flag.setEnabled(True)

        self.model.port.close()

        self.connect_button.setText("Connect")
        self.connect_button.setStatusTip(self.connect_button.text() + " Arduino")

        self.connect_button.clicked.connect(self.connect)

    def controll_btn_ll(self):
        self.statusBar().showMessage("Button mode " +\
                ("enabled" if self.butt_mode_flag.isChecked() else "disabled"))
        if self.butt_mode_flag.isChecked():
            self.left_btn.setEnabled(True)
            self.right_btn.setEnabled(True)
            self.up_btn.setEnabled(True)
            self.down_btn.setEnabled(True)
        else:
            self.left_btn.setEnabled(False)
            self.right_btn.setEnabled(False)
            self.up_btn.setEnabled(False)
            self.down_btn.setEnabled(False)

    def up(self):
        if self.down_btn.isChecked():
            self.down_btn.setChecked(False)
        self.send_from_buttons()


    def down(self):
        if self.up_btn.isChecked():
            self.up_btn.setChecked(False)
        self.send_from_buttons()


    def send_from_buttons(self,key=[0,0]):
        print(self.left_btn.isDefault())
        code = "$"
        code += "1 " if self.up_btn.isChecked() else "0 "
        code += "1 " if self.down_btn.isChecked() else "0 "
        code += "1 " if key[0] else "0 "
        code += "1 " if key[1] else "0 "
        code += str(self.speed_slider.value()) + " "
        code += str(self.sensitivity_slider.value()) + " "
        code += ";"
        try:
            self.model.send_code(code)
        except:
            QMessageBox.warning(self, "warning", "please reconnect controller")
            self.disconnect()





if __name__ == '__main__':
    # print(QT_TR_NOOP("Goodbye"))
    app = QtWidgets.QApplication(sys.argv)
    window = GUI()
    sys.exit(app.exec())
