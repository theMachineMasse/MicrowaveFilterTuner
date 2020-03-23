import serial
import os
import time
from OpenCVWide import *
from OpenCVZoom import *


global ser
global screwLocationsList
screwLocationsList = []
global rotationPosition
fileName = 'SharedTuningFile.txt'


def initAutomaticTune(tuningSettings):
    screwType = tuningSettings[0]
    wideSens = tuningSettings[1]
    zoomSens = tuningSettings[2]
    widePort = tuningSettings[3]
    ZoomPort = tuningSettings[4]
    comPort = 'COM' + str(tuningSettings[5])
    global ser
    global screwLocationsList
    ser = serial.Serial(comPort, 9600, timeout=0.01)
    ser.flushInput()
    line = b''
    # enable motors
    ser.write(b'm17')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')
    '''
    # home machine
    line = b''
    ser.write(b'g28')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')
    '''
    # move to center position
    line = b''
    ser.write(b'g0 y200 x135 z0')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

    # turn on overall light
    line = b''
    ser.write(b'm42 p1 s1')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

    # turn on ring light
    #line = b''
    #ser.write(b'm42 p2 s1')
    #ser.write(b'\r')
    #while b'k' not in line :
    #    line = ser.readline()
    #print('k')


    # take wide angle photos
    screwsDetected = wideAngleCamera(tuningSettings)  # testing

    print(screwsDetected)
    if screwsDetected != 0:
        screwLocationsList = screwAssignment()  # testing

    # turn off ring light
    line = b''
    ser.write(b'm42 p2 s0')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

    # read tuning file
    file = open(fileName)
    text = file.read()
    file.close()
    text = text.splitlines()  # split up text file by lines
    text = list(filter(None, text))  # remove blank lines
    GUIString = text[0]
    del text[0]
    for i in range(len(text)):
        text[i] = text[i].split()  # split up by white space

    # create list of turned amounts of each screw
    global rotationPosition
    rotationPosition = []
    for i in range(len(text)):
        rotationPosition.append(float(0))


def automaticTune(tuningSettings):
    screwType = tuningSettings[0]
    wideSens = tuningSettings[1]
    zoomSens = tuningSettings[2]
    widePort = tuningSettings[3]
    ZoomPort = tuningSettings[4]
    comPort = 'COM' + str(tuningSettings[5])

    # wait for file to exist
    while not os.path.isfile(fileName):
        time.sleep(0.5)

    # read tuning file
    file = open(fileName)
    text = file.read()
    file.close()
    text = text.splitlines()    # split up text file by lines
    text = list(filter(None, text)) # remove blank lines
    GUIString = text[0]
    del text[0]
    for i in range(len(text)):
        text[i] = text[i].split()         # split up by white space

    print(GUIString)
    print(text)



    # move to all locations in tuning file (for loop)
    for i in range(len(text)):
        if text[i][1] != '0':
            for j in range(len(screwLocationsList)):
                if screwLocationsList[j][1] == i:
                    print(screwLocationsList[j])
                    # move to location
                    xPos = screwLocationsList[j][2]
                    yPos = screwLocationsList[j][3]

                    xPos = round(xPos,1)
                    yPos = round(yPos,1)
                    xPos = str(xPos)
                    yPos = str(yPos)
                    gCode = 'g0 x'+xPos+' y'+yPos
                    # move to screw location
                    line = b''
                    ser.write(gCode.encode('UTF-8'))
                    # ser.write(bytes(gCode, 'utf-8'))
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')

                    # take zoom photo
                    zoomOffsetsAngles = zoomCamera(tuningSettings)
                    xPos = screwLocationsList[j][2]
                    yPos = screwLocationsList[j][3]
                    xPos = xPos + zoomOffsetsAngles[0]
                    yPos = yPos + zoomOffsetsAngles[1]
                    xPos = round(xPos, 1)
                    yPos = round(yPos, 1)
                    xPos = str(xPos)
                    yPos = str(yPos)
                    gCode = 'g0 x' + xPos + ' y' + yPos
                    # move to screw location
                    line = b''
                    ser.write(gCode.encode('UTF-8'))
                    # ser.write(bytes(gCode, 'utf-8'))
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')

                    # zoomCamera(30)  # for testing only

                    # home phi axis
                    line = b''
                    ser.write(b'g15')
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')

                    # rotate to align with screw
                    if rotationPosition[i] > 0:
                        rotateDirection = 'p'
                    else:
                        rotateDirection = 'n'

                    rotateAmount = abs(rotationPosition[i])

                    while rotateAmount >= 90:
                        rotateAmount = rotateAmount - 90

                    rotateAmount = round(rotateAmount, 1)
                    rotateAmount = str(rotateAmount)
                    gCode = 'g16 ' + rotateDirection + rotateAmount
                    print(gCode)
                    line = b''
                    ser.write(gCode.encode('UTF-8'))
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')

                    # move down to screw
                    zPos = zoomOffsetsAngles[2]
                    print(zPos)
                    zPos = round(zPos, 1)
                    zPos = str(zPos)
                    print(zPos)
                    gCode = 'g30 z' + zPos
                    print(gCode)
                    # move to screw location
                    line = b''
                    ser.write(gCode.encode('UTF-8'))
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')

                    # convert string from file to float
                    rotateAmount = float(text[i][1])
                    if rotateAmount > 0:
                        rotateDirection = 'p'
                    else:
                        rotateDirection = 'n'

                    # rotate desired amount
                    moveAmount = abs(rotateAmount)
                    moveAmount = round(moveAmount, 1)
                    moveAmount = str(moveAmount)
                    gCode = 'g16 ' + rotateDirection + moveAmount
                    print(gCode)
                    line = b''
                    ser.write(gCode.encode('UTF-8'))
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')
                    rotationPosition[i] = rotationPosition[i] + rotateAmount

                    # move back to upper Z position
                    line = b''
                    ser.write(b'g0 z0')
                    ser.write(b'\r')
                    while b'k' not in line:
                        line = ser.readline()
                    print('k')

    # remove tuning list file
    if os.path.exists(fileName):
        os.remove(fileName)
    else:
        print("The file does not exist")


def stopAutomaticTune():
    # disable motors
    line = b''
    ser.write(b'm18')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

    # turn off lights
    line = b''
    ser.write(b'm42 p1 s0')
    ser.write(b'\r')
    while b'k' not in line:
        line = ser.readline()
    print('k')

    ser.close()


def AutomaticModeLoop(tuningSettings):
    initAutomaticTune(tuningSettings)

    tuningTrue = 1
    while tuningTrue == 1:
        # wait for file to exist
        while not os.path.isfile(fileName):
            time.sleep(0.5)
        # read tuning file
        file = open(fileName)
        text = file.read()
        file.close()
        text = text.splitlines()  # split up text file by lines
        text = list(filter(None, text))  # remove blank lines
        GUIString = text[0]
        del text[0]
        for i in range(len(text)):
            text[i] = text[i].split()  # split up by white space

        tuningTrue = 0
        for i in range(len(text)):
            if text[i][1] != '0':
                tuningTrue = 1

        if tuningTrue == 1:
            automaticTune(tuningSettings)

    stopAutomaticTune()


#initAutomaticTune()
#automaticTune()
#automaticTune()
#stopAutomaticTune()
#screwAssignment()

