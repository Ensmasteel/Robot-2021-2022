from mainWindow import MainWindow
from PyQt5 import QtCore,QtWidgets
import sys

app = QtWidgets.QApplication(sys.argv)
mainWin = QtWidgets.QMainWindow()
mainWid = MainWindow()
mainWin.setCentralWidget(mainWid)
mainWin.show()
sys.exit(app.exec_())