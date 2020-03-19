import serial
from OpenCVWide import *
from OpenCVZoom import *


comPort = 'COM7'
global ser
global screwLocationsList
screwLocationsList = []
global rotationPosition


def initAutomaticTune():
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

    # home machine
    line = b''
    ser.write(b'g28')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

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
    screwsDetected = wideAngleCamera(30)  # testing

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
    file = open('SharedTuningFile.txt')
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


def automaticTune():
    # read tuning file
    file = open('SharedTuningFile.txt')
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
                    zoomOffsetsAngles = zoomCamera(30)
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
                    rotationPosition[i] = abs(rotationPosition[i])
                    print(rotationPosition[i])
                    while rotationPosition[i] >= 90:
                        rotationPosition[i] = rotationPosition[i] - 90
                    print(rotationPosition[i])

                    rotateAmount = round(rotationPosition[i], 1)
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


initAutomaticTune()
automaticTune()
automaticTune()
stopAutomaticTune()
#zoomCamera(30)
#wideAngleCamera(30)
#screwAssignment()
