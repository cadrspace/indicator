import time
import serial
import binascii

out = ''

def listenSerial():
	s = 1
	while s:
		s = ser.readline()
		print (s.decode().rstrip('\r\n')) # Read the newest output from the Arduino
		#time.sleep(.1) # Delay for one tenth of a second

ser = serial.Serial(
    port='/dev/ttyUSB0',
	timeout=5,
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

if ser.isOpen():
    ser.close()
ser.open()
ser.isOpen()

ser.flush()
time.sleep(2)

#ser.write(bytes('b'.encode()))
#ser.write(bytes(str(130).encode() ))

ser.write(binascii.unhexlify('82'))

#ser.write(binascii.unhexlify('01'))
#listenSerial()

am = 64
while am > 0:
	ser.write(binascii.unhexlify('07'))
	am = am - 1
	time.sleep(.0125)
#	listenSerial()


#ser.write(binascii.unhexlify('07'))
#listenSerial()

ser.close()
