'''
purpose:        acquire tsi data and put it on ip:port for others
usage:          python  scripts-name serialport baudrate ipport
example:        python  scripts-name com1 4800 7071

change log:     
                20211221    0.1.0     first prototype 
                20211222    0.1.1     add crc16-modbus 
                            0.1.2     threading serialport 
                            0.1.3     add single blocking network server
                20211223    0.1.4     change to non-blocking socket
                            0.1.5     using fire to pass arguments
                            0.1.6     add date+time to network send data


'''
import serial           # import the module
import time
import binascii
import threading
import socket
import select
import fire
import sys

tsivalue=0
cmdstring=''
cmddata=bytes.fromhex(cmdstring)

def tsi4site(devport,devbaud,ipport):
  global cmdstring
  global cmddata
  cmdstring='01 03 00 00 00 01 84 0a'
  cmddata=bytes.fromhex(cmdstring)
  print("====================================================")
  #comPort=tsisp('com1',4800)
  try:
    comPort=tsisp(devport,devbaud)
    t1=threading.Thread(target=comPort.read_data)
    server=ThreadSocket('',ipport)
  except:
      print("Error in opening %d or  %s, pls check" %(ipport,devport))
      print("====================================================")
      sys.exit(1)
  t1.start()
  server.listen()
        

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
          try:
            client, address = event.accept()
            inputs.append(client)
            print("Connected from {}".format(address))
          except BlockingIOError:
            pass
        else:
          try:
            tmpdata = event.recv(1024)
          except:
            pass
          if tmpdata:
            event.send((time.strftime("%Y-%m-%d", time.localtime())+","+time.strftime("%H:%M:%S", time.localtime())+","+str(tsivalue)).encode())
            print("Sending %4d to %s..." %(tsivalue,address))
          #time.sleep(1)
            event.close()
          else:
            inputs.remove(event)
          #client.settimeout(60)



class tsisp:
  tsinow=0
  global cmdstring
  global cmddata
  def __init__(self,port,baud):
    super(tsisp, self).__init__()
    self.port=serial.Serial(port,baud)
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
        No = self.send_data(cmddata)
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
            print("Wrong data received from TSI Device...")
          self.port.flush()   
        else:
          print("No data received from TSI Device...")
        time.sleep(1)
    except KeyboardInterrupt:
      pass
      
if __name__=='__main__':
  fire.Fire(tsi4site)



