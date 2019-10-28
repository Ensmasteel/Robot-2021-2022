import serial
from PyQt5.QtCore import QObject,pyqtSignal,QThread
from enum import Enum
import time

class MessageId(Enum):
    Empty=0
    Stop=1
    Tirette=2
    PID_T_P_incr_M=3
    PID_T_P_decr_M=4
    PID_T_I_incr_M=5
    PID_T_I_decr_M=6
    PID_T_D_incr_M=7
    PID_T_D_decr_M=8
    PID_R_P_incr_M=9
    PID_R_P_decr_M=10
    PID_R_I_incr_M=11
    PID_R_I_decr_M=12
    PID_R_D_incr_M=13
    PID_R_D_decr_M=14
    add_forward_M=15
    add_backward_M=16
    add_spin_M=17

class State(Enum):
    DEBUG=1
    NAME=2
    VALUE=3
    INFO=4

class Parser(QThread):
    newTelem=pyqtSignal(dict)
    newInfo=pyqtSignal(str)
    newDebug=pyqtSignal(str)

    def __init__(self,mainWindow):
        super().__init__()
        file = open("setup.txt","r")
        port = file.readline()[:-1]
        baudrate = int(file.readline())
        file.close()
        self.debug=""
        self.info=""
        self.name=""
        self.value=""
        self.debug=""
        self.ser=serial.Serial()
        self.ser.port=port
        self.ser.baudrate=baudrate
        self.state=State.DEBUG
        self.stop=False
        mainWindow.sendMessage.connect(self.sendMessage)

    def sendMessage(self,messageID : MessageId):
        self.ser.write(bytes([0,0,0,0,messageID.value,0]))
        #self.ser.write(bytes.fromhex("000000000200"))
        print("send")
    
    def run(self):
        self.connectPort()
        while not self.stop:
            #print(self.state)
            if self.ser.inWaiting()>0:
                x=(self.ser.read()).decode()
                if self.state==State.DEBUG:
                    if x=="@":
                        self.state=State.NAME
                        if self.debug!="":
                            self.newDebug.emit(self.debug)
                        self.debug=""
                    elif x=="#":
                        self.state=State.INFO
                        if self.debug!="":
                            self.newDebug.emit(self.debug)
                        self.debug=""
                    else:
                        self.debug+=x
                elif self.state==State.NAME:
                    if x=="|":
                        self.state=State.VALUE
                    else:
                        self.name+=x
                elif self.state==State.VALUE:
                    if x=="\n":
                        self.newTelem.emit({self.name:self.value})
                        self.state=State.DEBUG
                        self.value=""
                        self.name=""
                    else:
                        self.value+=x
                elif self.state==State.INFO:
                    if x=="\n":
                        self.state=State.DEBUG
                        self.newInfo.emit(self.info)
                        self.info=""
                    else:
                        self.info+=x
    
    def stop(self):
        stop=True
        self.ser.close()

    def connectPort(self):
        try:
            self.ser.open()
        except:
            print("Wrong COM")
            exit()