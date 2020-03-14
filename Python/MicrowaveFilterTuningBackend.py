import serial

comPort = 'COM5'

ser = serial.open(comPort)
ser.flushInput()

ser.write('M17')
while(ser.readLine() )

ser.write('g28')

