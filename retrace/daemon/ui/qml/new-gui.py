#!/usr/bin/python3.5

import sys
from PyQt5.QtWidgets import (QApplication, QMainWindow, QAction, QActionGroup,
                             QFontComboBox, QComboBox, QWidget, QVBoxLayout,
                             QGridLayout, QLabel, QLineEdit, QCompleter,
                             QDirModel, QSizePolicy, QToolButton,
                             QFileDialog, QSpinBox, QDialogButtonBox)
from PyQt5.QtCore import (Qt, QUrl, QObject, pyqtSignal, QSize,
                          QCoreApplication)
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
        self.controls.fileLabel.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.controls.layout.addWidget(self.controls.fileLabel, 0, 0)
        self.controls.lineEdit = QLineEdit(self.controls)
        self.controls.lineEditCompleter = QCompleter(self.controls.lineEdit)
        self.controls.lineEditCompleter.setCompletionMode(
            QCompleter.UnfilteredPopupCompletion)
        self.controls.lineEditCompleter.setModel(
            QDirModel(self.controls.lineEditCompleter))
        self.controls.lineEdit.setCompleter(self.controls.lineEditCompleter)
        self.controls.layout.addWidget(self.controls.lineEdit, 0, 1)
        self.controls.fileButton = QToolButton(self.controls)
        self.controls.fileButton.setIcon(QIcon(
            "images/Adwaita/document-open.png"))
        self.controls.fileButton.setToolTip("Open trace")
        self.controls.fileButton.setIconSize(QSize(24, 24))
        self.controls.fileButton.clicked.connect(self.getFilename)
        self.controls.layout.addWidget(self.controls.fileButton, 0, 2)

        # Frame area.
        self.controls.frameLabel = QLabel("Frame:", self.controls)
        self.controls.frameLabel.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.controls.layout.addWidget(self.controls.frameLabel, 1, 0)
        self.controls.frameBox = QSpinBox(self.controls)
        self.controls.frameBox.setMinimum(0)
        self.controls.frameBox.setMaximum(1000)
        self.controls.frameBox.setValue(100)
        self.controls.frameBox.setAlignment(Qt.AlignRight)
        self.controls.frameBox.setSizePolicy(QSizePolicy.Maximum,
                                             QSizePolicy.Maximum)
        self.controls.layout.addWidget(self.controls.frameBox, 1, 1)

        # Host area.
        self.controls.hostLabel = QLabel("Host:", self.controls)
        self.controls.hostLabel.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.controls.layout.addWidget(self.controls.hostLabel, 2, 0)
        self.controls.hostEdit = QLineEdit("localhost", self.controls)
        self.controls.layout.addWidget(self.controls.hostEdit, 2, 1, 1, -1)

        # Dialog bottom.
        self.layout.addWidget(self.controls)
        self.layout.addWidget(VSpacer())
        self.dialogButtons = QDialogButtonBox(QDialogButtonBox.Ok |
                                              QDialogButtonBox.Cancel)
        self.dialogButtons.rejected.connect(QCoreApplication.instance().quit)
        self.dialogButtons.accepted.connect(self.openFile)
        self.layout.addWidget(self.dialogButtons)


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

    def getFilename(self):
        currentFname = self.controls.lineEdit.text()
        if currentFname == '':
            currentFname = "/home"
        fname = QFileDialog.getOpenFileName(self, "Open File", currentFname,
                                                  "Trace files (*.trace)")
        self.controls.lineEdit.setText(fname[0])

    def openFile(self):
        self.hide()


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
