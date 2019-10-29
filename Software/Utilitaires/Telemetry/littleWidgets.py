from PyQt5 import QtWidgets as Qtw

class LabeledValue(Qtw.QFrame):
    def __init__(self,name,value,parent=None):
        super().__init__(parent)
        self.setFrameShadow(Qtw.QFrame.Plain)
        self.setFrameShape(Qtw.QFrame.StyledPanel)
        self.myLayout=Qtw.QHBoxLayout(self)
        self.setLayout(self.myLayout)
        self.labelName=Qtw.QLabel(name,self)
        self.myLayout.addWidget(self.labelName)
        self.labelValue=Qtw.QLabel(str(value),self)
        self.myLayout.addWidget(self.labelValue)
    
    def setValue(self,value):
        self.labelValue.setText(value)