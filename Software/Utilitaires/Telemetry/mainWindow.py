from PyQt5 import QtWidgets as Qtw
from PyQt5.QtGui import QColor
from PyQt5 import QtCore as Qt
from littleWidgets import LabeledValue
from parseSerial import Parser
from parseSerial import MessageID

class CinetiqueW(Qtw.QFrame):
    def __init__(self,prefix):
        super().__init__()
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)
        self.myLayout=Qtw.QHBoxLayout(self)
        self.setLayout(self.myLayout)
        self.widgets={prefix+"x":LabeledValue("x",0,self) , prefix+"y":LabeledValue("y",0,self) , prefix+"Th":LabeledValue("theta",0,self) , prefix+"v":LabeledValue("v",0,self) , prefix+"w":LabeledValue("w",0,self)}
        if prefix=="R":
            self.myLayout.addWidget(Qtw.QLabel("Robot:",self))
        else:
            self.myLayout.addWidget(Qtw.QLabel("Ghost:",self))
        for w in self.widgets.values():
            self.myLayout.addWidget(w)
    
    def update(self,dick : dict):
        for k,v in dick.items():
            if k in self.widgets.keys():
                self.widgets[k].setValue(v)

class Sequence(Qtw.QFrame):
    def __init__(self):
        super().__init__()
        self.i="0"
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)

        self.mainLayout=Qtw.QHBoxLayout(self)

        self.insideWidget=Qtw.QWidget(self)
        self.insideWidgetLayout=Qtw.QHBoxLayout(self)

        self.actions={}
        for i in range(500):
            self.actions.update({"A"+str(i):Qtw.QLabel("",self.insideWidget)})
            self.insideWidgetLayout.addWidget(self.actions["A"+str(i)])
            self.actions["A"+str(i)].setVisible(False)
        self.insideWidget.setLayout(self.insideWidgetLayout)

        self.mainLayout.addWidget(self.insideWidget)
        self.setLayout(self.mainLayout)

    
    def update(self,dick : dict):
        if "i" in dick.keys():
            self.i=dick["i"]
        for k,v in dick.items():
            if k[0]=="A":
                self.actions[k].setText(v)
                self.actions[k].setVisible(True)
                self.actions[k].setFrameShadow(Qtw.QFrame.Plain)
                self.actions[k].setFrameShape(Qtw.QFrame.NoFrame)
                self.actions[k].setLineWidth(5)

            if k[0]=="F":
                k_="A"+k[1]
                p = self.actions[k_].palette()
                if v=="1": #fail
                    p.setColor(self.foregroundRole(), QColor(255,0,0))
                else:
                    p.setColor(self.foregroundRole(), QColor(0,0,0))
                self.actions[k_].setPalette(p)

        if self.actions["A"+self.i].isVisible():
            self.actions["A"+self.i].setFrameShadow(Qtw.QFrame.Raised)
            self.actions["A"+self.i].setFrameShape(Qtw.QFrame.Panel)
            self.actions["A"+self.i].setLineWidth(5)

class Comm(Qtw.QFrame):
    def __init__(self):
        super().__init__()
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)
        self.myLayout=Qtw.QHBoxLayout(self)
        self.lab=Qtw.QLabel("Last Message ID : ")
        self.myLayout.addWidget(self.lab)
        self.mess=Qtw.QLabel("")
        self.myLayout.addWidget(self.mess)

    def update(self,dick : dict):
        for k,v in dick.items():
            if k=="mess":
                self.mess.setText(v)

class PidUnit(Qtw.QFrame):
    def __init__(self,translation,which,mainWindowMessageSignal):
        super().__init__()
        self.translation=translation
        self.which=which
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)

        self.myLayout=Qtw.QVBoxLayout(self)
        self.setLayout(self.myLayout)
        self.btnUp=Qtw.QPushButton("++")
        self.lab=Qtw.QLabel("P")
        if which==1:
            self.lab=Qtw.QLabel("I")
        elif which==2:
            self.lab=Qtw.QLabel("D")

        self.btnDown=Qtw.QPushButton("--")
        self.myLayout.addWidget(self.btnUp)
        self.myLayout.addWidget(self.lab)
        self.myLayout.addWidget(self.btnDown)
        self.btnUp.clicked.connect(lambda : mainWindowMessageSignal.emit(MessageID.PID_tweak_M,1,translation,which,0))
        self.btnDown.clicked.connect(lambda : mainWindowMessageSignal.emit(MessageID.PID_tweak_M,0,translation,which,0))

class PidGroup(Qtw.QFrame):
    def __init__(self,translation,mainWindowMessageSignal):
        super().__init__()
        self.translation=translation
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)
        self.myLayout=Qtw.QVBoxLayout(self)
        self.setLayout(self.myLayout)
        if translation:
            self.label=Qtw.QLabel("TRANSLATION")
        else:
            self.label=Qtw.QLabel("ROTATION")
        self.myLayout.addWidget(self.label)

        self.group=Qtw.QWidget(self)
        self.layoutGroup=Qtw.QHBoxLayout(self.group)
        self.group.setLayout(self.layoutGroup)
        self.pWidget=PidUnit(translation,0,mainWindowMessageSignal)
        self.layoutGroup.addWidget(self.pWidget)
        self.iWidget=PidUnit(translation,1,mainWindowMessageSignal)
        self.layoutGroup.addWidget(self.iWidget)
        self.dWidget=PidUnit(translation,2,mainWindowMessageSignal)
        self.layoutGroup.addWidget(self.dWidget)
        self.myLayout.addWidget(self.group)

class PidPanel(Qtw.QFrame):
    def __init__(self,mainWindowMessageSignal):
        super().__init__()
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)
        self.myLayout=Qtw.QHBoxLayout(self)
        self.setLayout(self.myLayout)
        self.translationGroup=PidGroup(True,mainWindowMessageSignal)
        self.myLayout.addWidget(self.translationGroup)
        self.rotationGroup=PidGroup(False,mainWindowMessageSignal)
        self.myLayout.addWidget(self.rotationGroup)



class MainWindow(Qtw.QWidget):
    sendMessage=Qt.pyqtSignal(MessageID,int,int,int,int)
    def __init__(self):
        super().__init__()
        self.mainLayout=Qtw.QVBoxLayout(self)
        self.setLayout(self.mainLayout)
        self.cinetiqueR=CinetiqueW("R")
        self.cinetiqueG=CinetiqueW("G")
        self.sequenceWidget=Sequence()
        self.commWidget=Comm()
        self.pidPanel=PidPanel(self.sendMessage)
        self.mainLayout.addWidget(self.cinetiqueR)
        self.mainLayout.addWidget(self.cinetiqueG)
        self.mainLayout.addWidget(self.sequenceWidget)
        self.mainLayout.addWidget(self.commWidget)
        self.mainLayout.addWidget(self.pidPanel)
        self.parserThread=Parser(self)
        self.parserThread.newTelem.connect(self.cinetiqueR.update)
        self.parserThread.newTelem.connect(self.cinetiqueG.update)
        self.parserThread.newTelem.connect(self.sequenceWidget.update)
        self.parserThread.newTelem.connect(self.commWidget.update)
        self.parserThread.newInfo.connect(print)
        self.parserThread.newDebug.connect(print)
        
        self.parserThread.start()
