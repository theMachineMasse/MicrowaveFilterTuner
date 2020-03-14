import serial

comPort = 'COM7'


def automaticTune():
    ser = serial.Serial(comPort, 9600, timeout=0.01)
    ser.flushInput()
    line = b''

    ser.write(b'm17')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')


    line = b''
    ser.write(b'g28')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')


    line = b''
    ser.write(b'g0 x100 y100')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

    line = b''
    ser.write(b'g0 x135 y200 z50')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')

    line = b''
    ser.write(b'g16 p720 n720')
    ser.write(b'\r')
    while b'k' not in line:
        line = ser.readline()
    print('k')

    line = b''
    ser.write(b'm18')
    ser.write(b'\r')
    while b'k' not in line :
        line = ser.readline()
    print('k')


    ser.close()


automaticTune()
