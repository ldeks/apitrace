#!/usr/bin/python3.5

import sys
from PyQt5.QtWidgets import (QApplication, QDialog, QWidget, QVBoxLayout,
                             QGridLayout, QLabel, QLineEdit, QCompleter,
                             QDirModel, QSizePolicy, QToolButton,
                             QFileDialog, QSpinBox, QDialogButtonBox,
                             QMainWindow, QGraphicsBlurEffect, QComboBox,
                             QTabWidget)
from PyQt5.QtCore import (Qt, QObject, pyqtSignal, QSize, QUrl,
                          QCoreApplication, QRect)
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QIcon, QFont, QPixmap, QGuiApplication

class OpenDialog(QDialog):

    def __init__(self, parent):
        super().__init__(parent)

        self.initUI()

    def initUI(self):

        self.layout = QVBoxLayout(self)

        # Image View
        self.view = ImageView()
        self.layout.addWidget(self.view)

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
        self.dialogButtons = QDialogButtonBox(QDialogButtonBox.Cancel |
                                              QDialogButtonBox.Ok)
        self.dialogButtons.rejected.connect(QCoreApplication.instance().quit)
        self.dialogButtons.accepted.connect(self.openFile)
        self.layout.addWidget(self.dialogButtons)


        # Window finalization
        self.setGeometry(300, 300, 600, 500)
        self.setWindowTitle('Frame Retrace Mock-Up GUI')
        self.setModal(True) # defer to main gui.
        self.show()

    def getFilename(self):
        currentFname = self.controls.lineEdit.text()
        if currentFname == '':
            currentFname = "/home"
        fname = QFileDialog.getOpenFileName(self, "Open File", currentFname,
                                                  "Trace files (*.trace)")
        # If user presses cancel, string will be null
        if (fname[0] != ''):
            self.controls.lineEdit.setText(fname[0])

    def openFile(self):
        # No-op for now.
        self.close()


class ImageView(QLabel):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.pixmap = QPixmap("images/retracer_logo.png")
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.setAlignment(Qt.AlignCenter)
        self.setPixmap(self.pixmap.scaled(self.size(), Qt.KeepAspectRatio,
                                          Qt.SmoothTransformation))
        self.blurred = False

    def resizeEvent(self, event):
        self.setPixmap(self.pixmap.scaled(self.size(), Qt.KeepAspectRatio,
                                          Qt.SmoothTransformation))

    def mousePressEvent(self, event):
        if self.blurred is False:
            self.setGraphicsEffect(QGraphicsBlurEffect())
            self.blurred = True
        else:
            self.setGraphicsEffect(None)
            self.blurred = False

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


class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):

        # Placeholder widget for central widget
        self.centralWidget = QWidget(self)
        self.layout = QVBoxLayout(self.centralWidget)

        # Hook up quickview
        self.graph = GraphView()
        self.graph.statusChanged.connect(self.quickViewStatusChanged)
        self.graph.sceneGraphError.connect(self.sceneGraphError)

        self.container = QWidget.createWindowContainer(self.graph)
        self.container.setMinimumSize(self.graph.size())
        self.container.setFocusPolicy(Qt.TabFocus)
        self.layout.addWidget(self.container)

        self.setCentralWidget(self.centralWidget)

        # Tool bar
        self.toolbar = self.addToolBar("Graph Controls")
        self.toolbar.setMovable(False)
        self.toolbar.ylabel = QLabel("Vertical Metric:", self)
        self.toolbar.ylabel.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.toolbar.xlabel = QLabel("Horizontal Metric:", self)
        self.toolbar.xlabel.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.toolbar.metrics = ["No Metric", "Pixel Shader Active Time",
                                "Fragment Shader Active Time"]
        self.toolbar.xComboBox = QComboBox(self)
        self.toolbar.xComboBox.addItems(self.toolbar.metrics)
        self.toolbar.yComboBox = QComboBox(self)
        self.toolbar.yComboBox.addItems(self.toolbar.metrics)

        self.toolbar.filterLabel = QLabel("Metrics Filter:", self)
        self.toolbar.filterLabel.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.toolbar.filter = QLineEdit(self)
        self.toolbar.filterCompleter = QCompleter(
            self.splitStringList(self.toolbar.metrics), self.toolbar.filter)
        self.toolbar.filterCompleter.setCompletionMode(
            QCompleter.UnfilteredPopupCompletion)
        self.toolbar.filter.setCompleter(self.toolbar.filterCompleter)

        self.toolbar.addWidget(self.toolbar.filterLabel)
        self.toolbar.addWidget(self.toolbar.filter)
        self.toolbar.addWidget(HSpacer())
        self.toolbar.addWidget(self.toolbar.ylabel)
        self.toolbar.addWidget(self.toolbar.yComboBox)
        self.toolbar.addWidget(self.toolbar.xlabel)
        self.toolbar.addWidget(self.toolbar.xComboBox)

        # Tab Widget
        self.tabs = QTabWidget(self)
        self.tabs.addTab(QWidget(), "Shaders")
        self.tabs.addTab(QWidget(), "RenderTarget")
        self.tabs.addTab(QWidget(), "API Calls")
        self.tabs.addTab(QWidget(), "Metrics")
        self.layout.addWidget(self.tabs)

        #self.layout.addWidget(VSpacer())

        # Window finalization.
        screenGeometry = QGuiApplication.primaryScreen().geometry()
        screenGeometry.moveTo(0, 0)
        self.setGeometry(screenGeometry)
        self.setWindowTitle('Frame Retrace Mock-Up GUI')
        self.statusBar().showMessage("Ready")

    def quickViewStatusChanged(status):
        if status is QQuickView.Error:
            errors = []
            for error in self.quickView.errors():
                errors.append(str(error))
            self.statusBar().showmessage((', ').join(errors))


    def sceneGraphError(error, message):
        self.statusBar.showMessage(message)

    def splitStringList(self, strlist):
        newlist = []
        for item in strlist:
            sublist = item.split(' ')
            for subitem in sublist:
                newlist.append(subitem)
        return newlist


class GraphView(QQuickView):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setResizeMode(QQuickView.SizeRootObjectToView)
        self.setSource(QUrl("imagebox.qml"))


if __name__ == '__main__':

    app = QApplication(sys.argv)
    mwindow = MainWindow()
    mwindow.show()
    openDialog = OpenDialog(mwindow)
    sys.exit(app.exec_())
