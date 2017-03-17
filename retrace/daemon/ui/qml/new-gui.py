#!/usr/bin/python3.5

import sys
from PyQt5.QtWidgets import (QApplication, QMainWindow, QAction, QActionGroup,
                             QFontComboBox, QComboBox, QWidget, QVBoxLayout)
from PyQt5.QtCore import Qt, QUrl, QObject, pyqtSignal
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QFontDatabase, QIcon, QFont

class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):

        # View widget stuff

        self.view = TextView()
        self.centralWidget = QWidget(self)
        self.layout = QVBoxLayout(self.centralWidget)
        self.view.statusChanged.connect(self.quickViewStatusChanged)
        self.view.sceneGraphError.connect(self.sceneGraphError)

        self.container = QWidget.createWindowContainer(self.view)
        self.container.setMinimumSize(self.view.size())
        self.container.setFocusPolicy(Qt.TabFocus)
        self.layout.addWidget(self.container)

        self.setCentralWidget(self.centralWidget)
        self.statusBar().showMessage('Ready')

        # Menu stuff

        self.toolbar = self.addToolBar('Text Effects')

        # Set up the text effects tools
        actionGroup = QActionGroup(self)

        noneAction = QAction(QIcon("none.png"), "&Clear", self)
        noneAction.setStatusTip("Clear Effects")
        actionGroup.addAction(noneAction)

        blurAction = QAction(QIcon("blur.png"), "&Blur", self)
        blurAction.setStatusTip("Blur Text")
        actionGroup.addAction(blurAction)

        opacityAction = QAction(QIcon("opacity.png"), "&Transparency", self)
        opacityAction.setStatusTip("Fade Text")
        actionGroup.addAction(opacityAction)

        shadowAction = QAction(QIcon("shadow.png"), "&Drop Shadow", self)
        shadowAction.setStatusTip("Drop-shadow Text")
        actionGroup.addAction(shadowAction)

        self.toolbar.addActions(actionGroup.actions())
        self.toolbar.addSeparator()

        # Set up the font selection tools
        boldAction = QAction(QIcon("bold.png"), "&Bold", self)
        boldAction.setStatusTip("Bold Text")
        boldAction.setCheckable(True)
        boldAction.setChecked(False)
        self.toolbar.addAction(boldAction)

        italicAction = QAction(QIcon("italic.png"), "&Italic", self)
        italicAction.setStatusTip("Italic Text")
        italicAction.setCheckable(True)
        self.toolbar.addAction(italicAction)

        self.fontBox = QFontComboBox(self)
        self.toolbar.addWidget(self.fontBox)

        self.fontSizeBox = QComboBox(self)
        self.fontSizeBox.setEditable(True)
        strlist = []
        intlist = QFontDatabase.standardSizes()
        for item in intlist:
            strlist.append(str(item))
        self.fontSizeBox.addItems(strlist)
        self.toolbar.addWidget(self.fontSizeBox)

        self.setGeometry(300, 300, 600, 500)
        self.setWindowTitle('Renderer')
        self.show()


    def quickViewStatusChanged(status):
        if status is QQuickView.Error:
            errors = []
            for error in self.quickView.errors():
                errors.append(str(error))
            self.statusBar().showmessage((', ').join(errors))


    def sceneGraphError(error, message):
        self.statusBar.showMessage(message)


class TextView(QQuickView):

    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):
        self.setResizeMode(QQuickView.SizeRootObjectToView)
        self.setSource(QUrl("imagebox.qml"))


if __name__ == '__main__':

    app = QApplication(sys.argv)
    mwindow = MainWindow()
    sys.exit(app.exec_())
