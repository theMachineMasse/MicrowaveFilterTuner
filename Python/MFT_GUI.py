##############################################
# Project: Microwave Filter Tuner
# File: MFT_GUI.py
# Date: March 04, 2020
# Programmer(s): Jarett Tremblay, Matthew Rostad
# Sub-Systems: Graphical User Interface Sub-System
# Version: 1.11
##############################################

from PyQt5 import QtCore, QtGui, QtWidgets
from MicrowaveFilterTuningBackend import *


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1102, 1000)
        MainWindow.setMinimumSize(QtCore.QSize(1102, 1000))
        MainWindow.setMaximumSize(QtCore.QSize(1102, 1000))
        
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        #Wide Angle Camera Slider
        self.Wide_Sensitivity_Slider = QtWidgets.QSlider(self.centralwidget)
        self.Wide_Sensitivity_Slider.setGeometry(QtCore.QRect(50, 570, 200, 22))
        self.Wide_Sensitivity_Slider.setOrientation(QtCore.Qt.Horizontal)
        self.Wide_Sensitivity_Slider.setObjectName("Wide_Sensitivity_Slider")
        self.Wide_Sensitivity_Slider.setMinimum(10)
        self.Wide_Sensitivity_Slider.setMaximum(40)
        self.Wide_Sensitivity_Slider.setValue(25)
        self.Wide_Sensitivity_Slider.setTickInterval(5)
        self.Wide_Sensitivity_Slider.setTickPosition(QtWidgets.QSlider.TicksBelow)
        self.Wide_Sensitivity_Slider.valueChanged.connect(self.wideSlider)
        
        self.Wide_Sensitivity_Label = QtWidgets.QLabel(self.centralwidget)
        self.Wide_Sensitivity_Label.setGeometry(QtCore.QRect(70, 540, 171, 21))
        self.Wide_Sensitivity_Label.setObjectName("Wide_Sensitivity_Label")

        self.Wide_Value_Label = QtWidgets.QLabel(self.centralwidget)
        self.Wide_Value_Label.setGeometry(QtCore.QRect(255, 565, 171, 21))
        self.Wide_Value_Label.setObjectName("Wide_Value_Label")

        #Enter Command Line
        self.Enter_Cmd_Line = QtWidgets.QLineEdit(self.centralwidget)
        self.Enter_Cmd_Line.setEnabled(True)
        self.Enter_Cmd_Line.setGeometry(QtCore.QRect(330, 770, 381, 41))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.Enter_Cmd_Line.setFont(font)
        self.Enter_Cmd_Line.setText("")
        self.Enter_Cmd_Line.setObjectName("Enter_Cmd_Line")
        
        self.Enter_Cmd_Label = QtWidgets.QLabel(self.centralwidget)
        self.Enter_Cmd_Label.setGeometry(QtCore.QRect(330, 750, 91, 21))
        self.Enter_Cmd_Label.setObjectName("Enter_Cmd_Label")
        
        self.Version_Label = QtWidgets.QLabel(self.centralwidget)
        self.Version_Label.setGeometry(QtCore.QRect(440, 950, 161, 16))
        self.Version_Label.setObjectName("Version_Label")
        
        self.Wide_View_Label = QtWidgets.QLabel(self.centralwidget)
        self.Wide_View_Label.setGeometry(QtCore.QRect(240, 10, 121, 21))
        self.Wide_View_Label.setObjectName("Wide_View_Label")
        
        self.Zoom_View_Label = QtWidgets.QLabel(self.centralwidget)
        self.Zoom_View_Label.setGeometry(QtCore.QRect(760, 10, 131, 21))
        self.Zoom_View_Label.setObjectName("Zoom_View_Label")
        
        self.Output_Label = QtWidgets.QLabel(self.centralwidget)
        self.Output_Label.setGeometry(QtCore.QRect(515, 380, 91, 16))
        self.Output_Label.setObjectName("Output_Label")
        
        self.Settings_Label = QtWidgets.QLabel(self.centralwidget)
        self.Settings_Label.setGeometry(QtCore.QRect(120, 430, 61, 21))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setUnderline(True)
        self.Settings_Label.setFont(font)
        self.Settings_Label.setObjectName("Settings_Label")

        #Send Command
        self.Send_Cmd_Button = QtWidgets.QCommandLinkButton(self.centralwidget)
        self.Send_Cmd_Button.setGeometry(QtCore.QRect(710, 770, 151, 41))
        self.Send_Cmd_Button.setObjectName("Send_Cmd_Button")
        self.Send_Cmd_Button.clicked.connect(self.cmdButton)
        
        self.Wide_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.Wide_Port_Label.setGeometry(QtCore.QRect(65, 740, 121, 21))
        self.Wide_Port_Label.setObjectName("Wide_Port_Label")
        
        self.Serial_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.Serial_Port_Label.setGeometry(QtCore.QRect(95, 860, 81, 21))
        self.Serial_Port_Label.setObjectName("Serial_Port_Label")
        
        self.COM_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.COM_Port_Label.setGeometry(QtCore.QRect(110, 690, 81, 31))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setUnderline(True)
        self.COM_Port_Label.setFont(font)
        self.COM_Port_Label.setObjectName("COM_Port_Label")
        
        self.Zoom_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.Zoom_Port_Label.setGeometry(QtCore.QRect(85, 800, 121, 21))
        self.Zoom_Port_Label.setObjectName("Zoom_Port_Label")

        #Cancel Button
        self.Cancel_Button = QtWidgets.QPushButton(self.centralwidget)
        self.Cancel_Button.setGeometry(QtCore.QRect(870, 701, 161, 41))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Cancel_Button.setFont(font)
        self.Cancel_Button.setObjectName("Cancel_Button")
        self.Cancel_Button.clicked.connect(self.cancelButton)

        #Refresh Button
        self.Refresh_Button = QtWidgets.QPushButton(self.centralwidget)
        self.Refresh_Button.setGeometry(QtCore.QRect(870, 610, 161, 41))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Refresh_Button.setFont(font)
        self.Refresh_Button.setObjectName("Refresh_Button")
        self.Refresh_Button.clicked.connect(self.refreshButton)
        
        #Tighten Button
        self.Tighten_Button = QtWidgets.QPushButton(self.centralwidget)
        self.Tighten_Button.setGeometry(QtCore.QRect(870, 510, 161, 81))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Tighten_Button.setFont(font)
        self.Tighten_Button.setObjectName("Tighten_Button")
        self.Tighten_Button.clicked.connect(self.tightenButton)
        
        #Tune Button
        self.Tune_Button = QtWidgets.QPushButton(self.centralwidget)
        self.Tune_Button.setGeometry(QtCore.QRect(870, 410, 161, 81))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Tune_Button.setFont(font)
        self.Tune_Button.setObjectName("Tune_Button")
        self.Tune_Button.clicked.connect(self.tuneButton)

        #Input TextEdit
        self.Input_TextEdit = QtWidgets.QPlainTextEdit(self.centralwidget)
        self.Input_TextEdit.setGeometry(QtCore.QRect(330, 860, 381, 61))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Input_TextEdit.setFont(font)
        self.Input_TextEdit.setObjectName("Input_TextEdit")
        
        self.Input_Label = QtWidgets.QLabel(self.centralwidget)
        self.Input_Label.setGeometry(QtCore.QRect(330, 840, 91, 21))
        self.Input_Label.setObjectName("Input_Label")
        
        self.MFT_Logo = QtWidgets.QLabel(self.centralwidget)
        self.MFT_Logo.setGeometry(QtCore.QRect(820, 750, 300, 300))
        self.MFT_Logo.setText("")
        self.MFT_Logo.setPixmap(QtGui.QPixmap("Images/MFT_Logo.png"))
        self.MFT_Logo.setScaledContents(True)
        self.MFT_Logo.setObjectName("MFT_Logo")

        #Output Response View
        self.Output_View = QtWidgets.QLabel(self.centralwidget)
        self.Output_View.setGeometry(QtCore.QRect(320, 400, 471, 341))
        self.Output_View.setText("")
        self.Output_View.setPixmap(QtGui.QPixmap("Images/Filter_Response.png"))
        self.Output_View.setScaledContents(True)
        self.Output_View.setObjectName("Output_View")

        #Macro Zoom Camera View
        self.Zoom_View = QtWidgets.QLabel(self.centralwidget)
        self.Zoom_View.setGeometry(QtCore.QRect(570, 30, 471, 341))
        self.Zoom_View.setText("")
        self.Zoom_View.setPixmap(QtGui.QPixmap("Images/zoomOutput.png"))
        self.Zoom_View.setScaledContents(True)
        self.Zoom_View.setObjectName("Zoom_View")

        #Wide Angle Camera View
        self.Wide_View = QtWidgets.QLabel(self.centralwidget)
        self.Wide_View.setGeometry(QtCore.QRect(60, 30, 471, 341))
        self.Wide_View.setText("")
        self.Wide_View.setPixmap(QtGui.QPixmap("Images/output.png"))
        self.Wide_View.setScaledContents(True)
        self.Wide_View.setObjectName("Wide_View")

        #Wide Angle SpinBox
        self.Wide_spinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.Wide_spinBox.setGeometry(QtCore.QRect(200, 740, 42, 22))
        self.Wide_spinBox.setObjectName("Wide_spinBox")
        self.Wide_spinBox.setMinimum(0)
        self.Wide_spinBox.setMaximum(100)
        self.Wide_spinBox.setValue(0)
        self.Wide_spinBox.valueChanged.connect(self.wideSpin)

        #Zoom Angle SpingBox
        self.Zoom_spinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.Zoom_spinBox.setGeometry(QtCore.QRect(200, 800, 42, 22))
        self.Zoom_spinBox.setObjectName("Zoom_spinBox")
        self.Zoom_spinBox.setMinimum(0)
        self.Zoom_spinBox.setMaximum(100)
        self.Zoom_spinBox.setValue(1)
        self.Zoom_spinBox.valueChanged.connect(self.zoomSpin)

        #Serial SpinBox
        self.Serial_spinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.Serial_spinBox.setGeometry(QtCore.QRect(200, 860, 42, 22))
        self.Serial_spinBox.setObjectName("Serial_spinBox")
        self.Serial_spinBox.setMinimum(0)
        self.Serial_spinBox.setMaximum(100)
        self.Serial_spinBox.setValue(7)
        self.Serial_spinBox.valueChanged.connect(self.serialSpin)

        #Zoom Camera Slider
        self.Zoom_Sensitivity_Slider = QtWidgets.QSlider(self.centralwidget)
        self.Zoom_Sensitivity_Slider.setGeometry(QtCore.QRect(50, 630, 200, 22))
        self.Zoom_Sensitivity_Slider.setOrientation(QtCore.Qt.Horizontal)
        self.Zoom_Sensitivity_Slider.setObjectName("Zoom_Sensitivity_Slider")
        self.Zoom_Sensitivity_Slider.setMinimum(10)
        self.Zoom_Sensitivity_Slider.setMaximum(60)
        self.Zoom_Sensitivity_Slider.setValue(40)
        self.Zoom_Sensitivity_Slider.setTickInterval(5)
        self.Zoom_Sensitivity_Slider.setTickPosition(QtWidgets.QSlider.TicksBelow)
        self.Zoom_Sensitivity_Slider.valueChanged.connect(self.zoomSlider)
        
        self.Zoom_Sensitivity_Label = QtWidgets.QLabel(self.centralwidget)
        self.Zoom_Sensitivity_Label.setGeometry(QtCore.QRect(90, 600, 161, 21))
        self.Zoom_Sensitivity_Label.setObjectName("Zoom_Sensitivity_Label")

        self.Zoom_Value_Label = QtWidgets.QLabel(self.centralwidget)
        self.Zoom_Value_Label.setGeometry(QtCore.QRect(255, 625, 171, 21))
        self.Zoom_Value_Label.setObjectName("Zoom_Value_Label")

        #Screw Type List 
        self.Screw_Type_List = QtWidgets.QComboBox(self.centralwidget)
        self.Screw_Type_List.setGeometry(QtCore.QRect(70, 490, 161, 22))
        self.Screw_Type_List.setObjectName("Screw_Type_List")
        self.Screw_Type_List.addItem("")
        self.Screw_Type_List.addItem("")
        
        self.Screw_Type_Label = QtWidgets.QLabel(self.centralwidget)
        self.Screw_Type_Label.setGeometry(QtCore.QRect(120, 470, 61, 21))
        self.Screw_Type_Label.setObjectName("Screw_Type_Label")
        
        self.Send_Cmd_Button.raise_()
        self.Enter_Cmd_Line.raise_()
        self.Enter_Cmd_Label.raise_()
        self.Wide_Sensitivity_Label.raise_()
        self.Wide_Value_Label.raise_()
        self.Output_Label.raise_()
        self.Wide_Sensitivity_Slider.raise_()
        self.Settings_Label.raise_()
        self.Version_Label.raise_()
        self.Wide_View_Label.raise_()
        self.Zoom_View_Label.raise_()
        self.Wide_Port_Label.raise_()
        self.Serial_Port_Label.raise_()
        self.COM_Port_Label.raise_()
        self.Zoom_Port_Label.raise_()
        self.Refresh_Button.raise_()
        self.Cancel_Button.raise_()
        self.Tighten_Button.raise_()
        self.Tune_Button.raise_()
        self.Input_TextEdit.raise_()
        self.Input_Label.raise_()
        self.MFT_Logo.raise_()
        self.Output_View.raise_()
        self.Zoom_View.raise_()
        self.Wide_View.raise_()
        self.Wide_spinBox.raise_()
        self.Zoom_spinBox.raise_()
        self.Serial_spinBox.raise_()
        self.Zoom_Sensitivity_Slider.raise_()
        self.Zoom_Sensitivity_Label.raise_()
        self.Zoom_Value_Label.raise_()
        self.Screw_Type_List.raise_()
        self.Screw_Type_Label.raise_()
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.menuBar = QtWidgets.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 1105, 21))
        self.menuBar.setObjectName("menuBar")
        MainWindow.setMenuBar(self.menuBar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.Wide_Value_Label.setNum(self.Wide_Sensitivity_Slider.value())
        self.Wide_Sensitivity_Label.setText(_translate("MainWindow", "Wide Angle Detection Sensitivity"))
        self.Enter_Cmd_Label.setText(_translate("MainWindow", "Enter Command"))
        self.Version_Label.setText(_translate("MainWindow", "Microwave Filter Tuner GUI v1.0"))
        self.Wide_View_Label.setText(_translate("MainWindow", "Wide Angle Camera View"))
        self.Zoom_View_Label.setText(_translate("MainWindow", "Zoom Camera View"))
        self.Output_Label.setText(_translate("MainWindow", "Output Response"))
        self.Settings_Label.setText(_translate("MainWindow", "Settings"))
        self.Send_Cmd_Button.setText(_translate("MainWindow", "Send Command"))
        self.Wide_Port_Label.setText(_translate("MainWindow", "Wide Angle Camera Port"))
        self.Serial_Port_Label.setText(_translate("MainWindow", "Serial COM Port"))
        self.COM_Port_Label.setText(_translate("MainWindow", "COM Ports"))
        self.Zoom_Port_Label.setText(_translate("MainWindow", "Zoom Camera Port"))
        self.Cancel_Button.setText(_translate("MainWindow", "Cancel"))
        self.Refresh_Button.setText(_translate("MainWindow", "Refresh"))
        self.Tighten_Button.setText(_translate("MainWindow", "Tighten"))
        self.Tune_Button.setText(_translate("MainWindow", "Tune"))
        self.Input_Label.setText(_translate("MainWindow", "Input"))
        self.Zoom_Value_Label.setNum(self.Zoom_Sensitivity_Slider.value())
        self.Zoom_Sensitivity_Label.setText(_translate("MainWindow", "Zoom Detection Sensitivity"))
        self.Screw_Type_List.setItemText(0, _translate("MainWindow", "#10-32 Phillips"))
        self.Screw_Type_List.setItemText(1, _translate("MainWindow", "#10-24 Phillips"))
        self.Screw_Type_Label.setText(_translate("MainWindow", "Screw Type"))
        
    #TUNE BUTTON
    def tuneButton(self):
        print('test')
        screwType = int(self.Screw_Type_List.currentIndex())
        wideSens = int(self.Wide_Sensitivity_Slider.value())
        zoomSens = int(self.Zoom_Sensitivity_Slider.value())
        widePort = int(self.Wide_spinBox.value())
        ZoomPort = int(self.Zoom_spinBox.value())
        SerialPort = int(self.Serial_spinBox.value())

        print(screwType)

        tuningSettings = [screwType, wideSens, zoomSens, widePort, ZoomPort, SerialPort]

        AutomaticModeLoop(tuningSettings)

        self.refreshButton()
     
    #TIGHTEN BUTTON
    def tightenButton(self):
        print('test1')

    #CANCEL BUTTON
    def cancelButton(self):
        print('test2')

    # REFRESH BUTTON
    def refreshButton(self):
        print('test3')
        self.Output_View.setPixmap(QtGui.QPixmap("Images/Filter_Response.png"))
        self.Wide_View.setPixmap(QtGui.QPixmap("Images/output.png"))
        self.Zoom_View.setPixmap(QtGui.QPixmap("Images/zoomOutput.png"))


    #SEND COMMAND BUTTON  
    def cmdButton(self):
        print('test4')
        Command = self.Enter_Cmd_Line.text()
        print(Command)
        self.Enter_Cmd_Line.clear()
        comPort = 'COM' + str(self.Serial_spinBox.value())
        ser = serial.Serial(comPort, 9600, timeout=0.01)
        ser.flushInput()
        ser.write(ser.write(Command.encode('UTF-8')))
        ser.write(b'\r')
        ser.close()

        '''
        f = open("SharedTuningFile.txt", "r")
        #print(f.read(100))
        g = f.read(100)
        self.Input_TextEdit.appendPlainText(g)
        f.close()
        '''

        self.Input_TextEdit.setReadOnly(True)
        print(self.Input_TextEdit.isReadOnly())

    #WIDE SLIDER    
    def wideSlider(self):
        print('test5')
        wideSens = int(self.Wide_Sensitivity_Slider.value())
        self.Wide_Value_Label.setNum(wideSens)
        print(wideSens)

    #ZOOM SLIDER
    def zoomSlider(self):
        print('test6')
        zoomSens = int(self.Zoom_Sensitivity_Slider.value())
        self.Zoom_Value_Label.setNum(zoomSens)
        print(zoomSens)

    #WIDE SPINBOX
    def wideSpin(self):
        print('test7')
        widePort = int(self.Wide_spinBox.value())
        print(widePort)

    #ZOOM SPINBOX
    def zoomSpin(self):
        print('test8')
        zoomPort = int(self.Zoom_spinBox.value())
        print(zoomPort)

    #SERIAL SPINBOX
    def serialSpin(self):
        print('test9')
        serialPort = int(self.Serial_spinBox.value())
        print(serialPort)



if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
