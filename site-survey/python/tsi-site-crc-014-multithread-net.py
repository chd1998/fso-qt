import serial           # import the module
import time
import binascii
import threading
import socket
import select
import os
import sys
import signal

tsivalue=0
def calc_crc(string):
    data = bytes.fromhex(string)
    crc = 0xFFFF
    for pos in data:
        crc ^= pos
        for i in range(8):
            if ((crc & 1) != 0):
                crc >>= 1
                crc ^= 0xA001
            else:
                crc >>= 1
    return hex(((crc & 0xff) << 8) + (crc >> 8))

class ThreadSocket(object):
  """

  """
  def __init__(self, host, port):
    self.host = host
    self.port = port
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    self.sock.setblocking(0)
    self.sock.bind((self.host, self.port))
    print("listenning on %s:%d" %(host,port))

  def listen(self):
    self.sock.listen(5)
    inputs= [self.sock,]
    while True:
      r_list,w_list,e_list = select.select(inputs,[],[],1)
      for event in r_list:
        if event == self.sock:
          client, address = event.accept()
          inputs.append(client)
          print("Connected from {}".format(address))
        else:
          try:
            tmpdata = event.recv(1024)
          except:
            pass
          if tmpdata:
            event.send(str(tsivalue).encode())
            print("Sending %4d to %s..." %(tsivalue,address))
          #time.sleep(1)
            event.close()
          else:
            inputs.remove(event)
          #client.settimeout(60)



class tsisp:
  tsinow=0
  def __init__(self,port,buand):
    super(tsisp, self).__init__()
    self.port=serial.Serial(port,buand)
    self.port.close()
    if not self.port.isOpen():
      self.port.open()
  def port_open(self):
    if not self.port.isOpen():
      self.port.open()
  def port_close(self):
    self.port.close()
  def send_data(self,cmdstr):
      n=self.port.write(cmdstr)
      return n
  def read_data(self):
    global tsivalue
    try:
      while True:
        No = self.send_data(data)
        len = self.port.inWaiting()
      #print(len)
        if len>0:
          dataIn = self.port.read(len)        # Wait and read data
          #print(dataIn)
          recev_data=binascii.b2a_hex(dataIn).decode('utf-8')
          #print(recev_data)
          crc_in="0x"+recev_data[10:14]
          crc_data=recev_data[0:10]
          print("CRC from received data: ",crc_in)
          crc_out=calc_crc(crc_data)
          print("CRC from calculation:   ",crc_out)
          if crc_in == crc_out:
            tsinow=int(recev_data[6:10],16)
            print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
            print("TSI: %04d W/m^2" % tsinow)                      # print the received data
            tsivalue=tsinow
            print("====================================================")                      
          else:
            print("Wrong data received...")
          self.port.flush()   
        else:
          print("No data received...")
        time.sleep(1)
    except KeyboardInterrupt:
      pass
      
if __name__=='__main__':
  cmdstring='01 03 00 00 00 01 84 0a'
  data=bytes.fromhex(cmdstring)
  print("====================================================")
  #comPort=tsisp('com1',4800)
  comPort=tsisp('/dev/ttyUSB1',4800)
  t1=threading.Thread(target=comPort.read_data)
  t1.start()
  server=ThreadSocket('',7071)
  server.listen()
 


