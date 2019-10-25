import serial
from PyQt5.QtCore import QObject,pyqtSignal,QThread
from enum import Enum
import time

class State(Enum):
    DEBUG=1
    NAME=2
    VALUE=3
    INFO=4

class Parser(QThread):
    newTelem=pyqtSignal(dict)
    newInfo=pyqtSignal(str)
    newDebug=pyqtSignal(str)

    def __init__(self):
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
    
    def run(self):
        self.connectPort()
        while not self.stop:
            #print(self.state)
            if self.ser.inWaiting()>0:
                x=(self.ser.read()).decode()
                if self.state==State.DEBUG:
                    if x=="@":
                        self.state=State.NAME
                        self.newDebug.emit(self.debug)
                        self.debug=""
                    elif x=="#":
                        self.state=State.INFO
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