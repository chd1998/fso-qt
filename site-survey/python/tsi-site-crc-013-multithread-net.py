import serial           # import the module
import time
import binascii
import threading
import socket

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

def send2client():
  global tsivalue
  while True:
    client_sock, client_addr = server_sock.accept()
    srvthread = Thread(target=send2client)
    srvthread.start()
    print("Sending %4d to %s" %(tsivalue,client_addr))
    print("====================================================")
    client_sock.send(str(tsivalue).encode())
    client_sock.close()

class ThreadSocket(object):
  """

  """
  def __init__(self, host, port):
    self.host = host
    self.port = port
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    #self.sock.setblocking(0)
    self.sock.bind((self.host, self.port))

  def listen(self):
    self.sock.listen(5)
    while True:
      client, address = self.sock.accept()
      client.settimeout(60)
      threading.Thread(target=self.handleClientRequest, args=(client, address)).start()

  def handleClientRequest(self, client, address):
    global tsivalue
    #while True:
    #  try:
    client.send(str(tsivalue).encode())
    print("Sending %4d to %s..." %(tsivalue,address))
    time.sleep(1)
    client.close()
    #  except:
    #    client.close()

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
    while True:
      No = comPort.send_data(data)
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
      
if __name__=='__main__':
  cmdstring='01 03 00 00 00 01 84 0a'
  data=bytes.fromhex(cmdstring)
  print("====================================================")
  comPort=tsisp('/dev/ttyUSB1',4800)
  t1=threading.Thread(target=comPort.read_data)
  t1.start()
  
  while True:
    server=ThreadSocket('',7071)
    server.listen()

 
    
    
    



    
    
    
    
    

    



