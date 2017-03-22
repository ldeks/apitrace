#!/usr/bin/python3.5

import sys
from PyQt5.QtWidgets import (QApplication, QMainWindow, QAction, QActionGroup,
                             QFontComboBox, QComboBox, QWidget, QVBoxLayout,
                             QGridLayout, QLabel, QLineEdit, QCompleter,
                             QDirModel, QSizePolicy)
from PyQt5.QtCore import Qt, QUrl, QObject, pyqtSignal
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QFontDatabase, QIcon, QFont

class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):

        # Create a placeholder widget for the layout
        self.centralWidget = QWidget(self)
        self.layout = QVBoxLayout(self.centralWidget)

        # Image View (in Quick)
        self.view = ImageView()
        self.view.statusChanged.connect(self.quickViewStatusChanged)
        self.view.sceneGraphError.connect(self.sceneGraphError)
        self.container = QWidget.createWindowContainer(self.view)
        self.container.setMinimumSize(self.view.size())
        self.container.setFocusPolicy(Qt.TabFocus)
        self.layout.addWidget(self.container)

        # Controls area
        self.controls = QWidget(self)
        self.controls.layout = QGridLayout(self.controls)
        self.controls.setLayout(self.controls.layout)

        # Filename area
        self.controls.fileLabel = QLabel("Filename:", self.controls)
        self.controls.layout.addWidget(HSpacer(), 0, 0)
        self.controls.layout.addWidget(self.controls.fileLabel, 0, 1)
        self.controls.lineEdit = QLineEdit(self.controls)
        self.controls.lineEditCompleter = QCompleter(self.controls.lineEdit)
        self.controls.lineEditCompleter.setModel(
            QDirModel(self.controls.lineEditCompleter))
        self.controls.lineEdit.setCompleter(self.controls.lineEditCompleter)
        self.controls.layout.addWidget(self.controls.lineEdit, 0, 2)
        self.controls.layout.addWidget(HSpacer(), 0, 3)

        self.layout.addWidget(self.controls)
        self.layout.addWidget(VSpacer())

        # Window finalization
        self.setCentralWidget(self.centralWidget)
        self.statusBar().showMessage('Ready')
        self.setGeometry(300, 300, 600, 500)
        self.setWindowTitle('Frame Retrace Mock-Up GUI')
        self.show()


    def quickViewStatusChanged(status):
        if status is QQuickView.Error:
            errors = []
            for error in self.quickView.errors():
                errors.append(str(error))
            self.statusBar().showmessage((', ').join(errors))


    def sceneGraphError(error, message):
        self.statusBar.showMessage(message)


class ImageView(QQuickView):

    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):
        self.setResizeMode(QQuickView.SizeRootObjectToView)
        self.setSource(QUrl("imagebox.qml"))

class VSpacer(QWidget):

    def __init__(self):
        super().__init__()
        self.resize(0, 0)
        self.setSizePolicy(QSizePolicy.Maximum, QSizePolicy.MinimumExpanding)

class HSpacer(QWidget):

    def __init__(self):
        super().__init__()
        self.resize(0, 0)
        self.setSizePolicy(QSizePolicy.MinimumExpanding, QSizePolicy.Maximum)


if __name__ == '__main__':

    app = QApplication(sys.argv)
    mwindow = MainWindow()
    sys.exit(app.exec_())
