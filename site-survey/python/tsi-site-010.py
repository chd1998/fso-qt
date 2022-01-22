import serial           # import the module
import time
import binascii

ComPort = serial.Serial('/dev/ttyUSB1',4800) # open COM3
ComPort.baudrate = 4800 # set Baud rate to 9600
ComPort.bytesize = 8    # Number of data bits = 8
ComPort.parity   = 'N'  # No parity
ComPort.stopbits = 1    # Number of Stop bits = 1

#tmp='03 00 00 00 01 84 0a'
#data = bytes.fromhex(tmp)
tmpstring='01 03 00 00 00 01 84 0a'
#data=bytes.fromhex(tmpstring.encode('utf-8'))
#data = struct.pack(hex(tmpstring))
#data=tmpstring.encode('utf-8')
#print(data)
#No = ComPort.write(b'\x03\x00\x00\x00\x01\x84\x0a')
data=bytes.fromhex(tmpstring)
print("====================================================")
#print(data)
while 1:
  No = ComPort.write(data)
  #print(No)
  time.sleep(1)                      # print the data
  len = ComPort.inWaiting()
  #print(len)
  if len>0:
    dataIn = ComPort.read(len)        # Wait and read data
    #print(dataIn)
    recev_data=binascii.b2a_hex(dataIn).decode('utf-8')
    #print(recev_data)
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("TSI: %04d W/m^2" %int(recev_data[6:10],16))                      # print the received data
    print("====================================================")
  else:
    print("No data received...")
  ComPort.flush()
  #time.sleep(1)

ComPort.close()         # Close the Com port 

