/**************************************************************************
 *
 * Copyright 2017 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Laura Ekstrand <laura@jlekstrand.net>
 **************************************************************************/

#include <QList>
#include <QQmlError>
#include <QStatusBar>
#include <QUrl>

#include "glframe_mainwindow.hpp"

using glretrace::MainWindow;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  quickWidget = new QQuickWidget(this);
  quickWidget->setSource(QUrl("qrc:///qml/mainwin.qml"));

  connect(quickWidget, SIGNAL(statusChanged(QQuickWidget::Status)),
          this, SLOT(quickStatusChanged(QQuickWidget::Status)));
  connect(quickWidget, SIGNAL(sceneGraphError(
              QQuickWindow::SceneGraphError, const QString&)),
          this, SLOT(quickSceneGraphError(
              QQuickWindow::SceneGraphError, const QString&)));

  setCentralWidget(quickWidget);

  setGeometry(0, 0, 1000, 700);
  setWindowTitle("Frame Retrace");
  statusBar()->showMessage("");
}

MainWindow::~MainWindow() {
    // Nothing to delete because quickWidget is parented.
}

void
MainWindow::quickStatusChanged(QQuickWidget::Status status) {
  if (status == QQuickWidget::Error) {
    QList<QQmlError> errors = quickWidget->errors();
    QString errorMessage;

    for (int i = 0; i < errors.size(); i++) {
      errorMessage.append(errors.at(i).toString());
      if (i < errors.size() - 1) {
        errorMessage.append(", ");
      }
    }

    statusBar()->showMessage(errorMessage);
  }
}

void
MainWindow::quickSceneGraphError(QQuickWindow::SceneGraphError error,
                              const QString &message) {
  statusBar()->showMessage(message);
}
