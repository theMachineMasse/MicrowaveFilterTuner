# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'MFT_GUI.ui'
#
# Created by: PyQt5 UI code generator 5.14.1
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1105, 1000)
        MainWindow.setMinimumSize(QtCore.QSize(1105, 1000))
        MainWindow.setMaximumSize(QtCore.QSize(1105, 1000))
        
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        #Wide Angle Camera Slider
        self.WAC_Sensitivity_Slider = QtWidgets.QSlider(self.centralwidget)
        self.WAC_Sensitivity_Slider.setGeometry(QtCore.QRect(70, 570, 160, 22))
        self.WAC_Sensitivity_Slider.setOrientation(QtCore.Qt.Horizontal)
        self.WAC_Sensitivity_Slider.setObjectName("WAC_Sensitivity_Slider")
        
        self.WAC_Sensitivity_Label = QtWidgets.QLabel(self.centralwidget)
        self.WAC_Sensitivity_Label.setGeometry(QtCore.QRect(70, 540, 171, 21))
        self.WAC_Sensitivity_Label.setObjectName("WAC_Sensitivity_Label")
        
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
        
        self.WAC_View_Label = QtWidgets.QLabel(self.centralwidget)
        self.WAC_View_Label.setGeometry(QtCore.QRect(240, 10, 121, 21))
        self.WAC_View_Label.setObjectName("WAC_View_Label")
        
        self.MZC_View_Label = QtWidgets.QLabel(self.centralwidget)
        self.MZC_View_Label.setGeometry(QtCore.QRect(740, 10, 131, 21))
        self.MZC_View_Label.setObjectName("MZC_View_Label")
        
        self.Output_Label = QtWidgets.QLabel(self.centralwidget)
        self.Output_Label.setGeometry(QtCore.QRect(510, 380, 91, 16))
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
        self.Send_Cmd_Button.clicked.connect(self.cmdButton)
        self.Send_Cmd_Button.setGeometry(QtCore.QRect(710, 770, 151, 41))
        self.Send_Cmd_Button.setObjectName("Send_Cmd_Button")
        
        self.WAC_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.WAC_Port_Label.setGeometry(QtCore.QRect(70, 740, 121, 21))
        self.WAC_Port_Label.setObjectName("WAC_Port_Label")
        
        self.Serial_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.Serial_Port_Label.setGeometry(QtCore.QRect(90, 860, 81, 21))
        self.Serial_Port_Label.setObjectName("Serial_Port_Label")
        
        self.COM_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.COM_Port_Label.setGeometry(QtCore.QRect(110, 690, 81, 31))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setUnderline(True)
        self.COM_Port_Label.setFont(font)
        self.COM_Port_Label.setObjectName("COM_Port_Label")
        
        self.MZC_Port_Label = QtWidgets.QLabel(self.centralwidget)
        self.MZC_Port_Label.setGeometry(QtCore.QRect(70, 800, 121, 21))
        self.MZC_Port_Label.setObjectName("MZC_Port_Label")

        #Tighten Button
        self.Tighten_Button = QtWidgets.QPushButton(self.centralwidget)
        self.Tighten_Button.clicked.connect(self.tightenButton)
        self.Tighten_Button.setGeometry(QtCore.QRect(870, 570, 161, 81))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Tighten_Button.setFont(font)
        self.Tighten_Button.setObjectName("Tighten_Button")
        
        #Tune Button
        self.Tune_Button = QtWidgets.QPushButton(self.centralwidget)
        self.Tune_Button.clicked.connect(self.tuneButton)
        self.Tune_Button.setGeometry(QtCore.QRect(870, 470, 161, 81))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Tune_Button.setFont(font)
        self.Tune_Button.setObjectName("Tune_Button")

        #Status Browser
        self.Status_Browser = QtWidgets.QTextBrowser(self.centralwidget)
        self.Status_Browser.setGeometry(QtCore.QRect(330, 860, 381, 61))
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Status_Browser.setFont(font)
        self.Status_Browser.setObjectName("Status_Browser")
        
        self.Status_Label = QtWidgets.QLabel(self.centralwidget)
        self.Status_Label.setGeometry(QtCore.QRect(330, 840, 91, 21))
        self.Status_Label.setObjectName("Status_Label")
        
        self.MFT_Logo = QtWidgets.QLabel(self.centralwidget)
        self.MFT_Logo.setGeometry(QtCore.QRect(820, 750, 300, 300))
        self.MFT_Logo.setText("")
        self.MFT_Logo.setPixmap(QtGui.QPixmap("MFT_Logo.png"))
        self.MFT_Logo.setScaledContents(True)
        self.MFT_Logo.setObjectName("MFT_Logo")

        #Output Response View
        self.Output_View = QtWidgets.QLabel(self.centralwidget)
        self.Output_View.setGeometry(QtCore.QRect(320, 400, 471, 341))
        self.Output_View.setText("")
        self.Output_View.setPixmap(QtGui.QPixmap("Filter_Response.png"))
        self.Output_View.setScaledContents(True)
        self.Output_View.setObjectName("Output_View")

        #Macro Zoom Camera View
        self.MZC_View = QtWidgets.QLabel(self.centralwidget)
        self.MZC_View.setGeometry(QtCore.QRect(570, 30, 471, 341))
        self.MZC_View.setText("")
        self.MZC_View.setPixmap(QtGui.QPixmap("temp.png"))
        self.MZC_View.setScaledContents(True)
        self.MZC_View.setObjectName("MZC_View")

        #Wide Angle Camera View
        self.WAC_View = QtWidgets.QLabel(self.centralwidget)
        self.WAC_View.setGeometry(QtCore.QRect(60, 30, 471, 341))
        self.WAC_View.setText("")
        self.WAC_View.setPixmap(QtGui.QPixmap("temp.png"))
        self.WAC_View.setScaledContents(True)
        self.WAC_View.setObjectName("WAC_View")
        
        self.WAC_spinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.WAC_spinBox.setGeometry(QtCore.QRect(200, 740, 42, 22))
        self.WAC_spinBox.setObjectName("WAC_spinBox")
        
        self.MZC_spinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.MZC_spinBox.setGeometry(QtCore.QRect(200, 800, 42, 22))
        self.MZC_spinBox.setObjectName("MZC_spinBox")
        
        self.Serial_spinBox = QtWidgets.QSpinBox(self.centralwidget)
        self.Serial_spinBox.setGeometry(QtCore.QRect(200, 860, 42, 22))
        self.Serial_spinBox.setObjectName("Serial_spinBox")

        #Macro Zoom Camera Slider
        self.MZC_Sensitivity_Slider = QtWidgets.QSlider(self.centralwidget)
        self.MZC_Sensitivity_Slider.setMinimum(10)
        self.MZC_Sensitivity_Slider.setMaximum(60)
        self.MZC_Sensitivity_Slider.setValue(35)
        self.MZC_Sensitivity_Slider.setTickInterval(5)
        #self.MZC_Sensitivity_Slider.TicksBelow()
        self.MZC_Sensitivity_Slider.setGeometry(QtCore.QRect(70, 630, 160, 22))
        self.MZC_Sensitivity_Slider.setOrientation(QtCore.Qt.Horizontal)
        self.MZC_Sensitivity_Slider.setObjectName("MZC_Sensitivity_Slider")
        
        self.MZC_Sensitivity_Label = QtWidgets.QLabel(self.centralwidget)
        self.MZC_Sensitivity_Label.setGeometry(QtCore.QRect(70, 600, 161, 21))
        self.MZC_Sensitivity_Label.setObjectName("MZC_Sensitivity_Label")
        
        self.Screw_Type_List = QtWidgets.QComboBox(self.centralwidget)
        self.Screw_Type_List.setGeometry(QtCore.QRect(70, 490, 161, 22))
        self.Screw_Type_List.setObjectName("Screw_Type_List")
        self.Screw_Type_List.addItem("")
        
        self.Screw_Type_Label = QtWidgets.QLabel(self.centralwidget)
        self.Screw_Type_Label.setGeometry(QtCore.QRect(120, 470, 61, 21))
        self.Screw_Type_Label.setObjectName("Screw_Type_Label")
        
        self.Send_Cmd_Button.raise_()
        self.Enter_Cmd_Line.raise_()
        self.Enter_Cmd_Label.raise_()
        self.WAC_Sensitivity_Label.raise_()
        self.Output_Label.raise_()
        self.WAC_Sensitivity_Slider.raise_()
        self.Settings_Label.raise_()
        self.Version_Label.raise_()
        self.WAC_View_Label.raise_()
        self.MZC_View_Label.raise_()
        self.WAC_Port_Label.raise_()
        self.Serial_Port_Label.raise_()
        self.COM_Port_Label.raise_()
        self.MZC_Port_Label.raise_()
        self.Tighten_Button.raise_()
        self.Tune_Button.raise_()
        self.Status_Browser.raise_()
        self.Status_Label.raise_()
        self.MFT_Logo.raise_()
        self.Output_View.raise_()
        self.MZC_View.raise_()
        self.WAC_View.raise_()
        self.WAC_spinBox.raise_()
        self.MZC_spinBox.raise_()
        self.Serial_spinBox.raise_()
        self.MZC_Sensitivity_Slider.raise_()
        self.MZC_Sensitivity_Label.raise_()
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
        self.WAC_Sensitivity_Label.setText(_translate("MainWindow", "Wide Angle Detection Sensitivity"))
        self.Enter_Cmd_Label.setText(_translate("MainWindow", "Enter Command"))
        self.Version_Label.setText(_translate("MainWindow", "Microwave Filter Tuner GUI v1.0"))
        self.WAC_View_Label.setText(_translate("MainWindow", "Wide Angle Camera View"))
        self.MZC_View_Label.setText(_translate("MainWindow", "Macro Zoom Camera View"))
        self.Output_Label.setText(_translate("MainWindow", "Output Response"))
        self.Settings_Label.setText(_translate("MainWindow", "Settings"))
        self.Send_Cmd_Button.setText(_translate("MainWindow", "Send Command"))
        self.WAC_Port_Label.setText(_translate("MainWindow", "Wide Angle Camera Port"))
        self.Serial_Port_Label.setText(_translate("MainWindow", "Serial COM Port"))
        self.COM_Port_Label.setText(_translate("MainWindow", "COM Ports"))
        self.MZC_Port_Label.setText(_translate("MainWindow", "Macro Zoom Camera Port"))
        self.Tighten_Button.setText(_translate("MainWindow", "Tighten"))
        self.Tune_Button.setText(_translate("MainWindow", "Tune"))
        self.Status_Label.setText(_translate("MainWindow", "Status"))
        self.MZC_Sensitivity_Label.setText(_translate("MainWindow", "Macro Zoom Detection Sensitivity"))
        self.Screw_Type_List.setItemText(0, _translate("MainWindow", "#10-32 Phillips"))
        self.Screw_Type_Label.setText(_translate("MainWindow", "Screw Type"))

    def tuneButton(self):
        sys.exit()

    def tightenButton(self):
        sys.exit()
        
    def cmdButton(self):
        sys.exit()
        
    def sensSlider(self):
        sys.exit()
        

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
