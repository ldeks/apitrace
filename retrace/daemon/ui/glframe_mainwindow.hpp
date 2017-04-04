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


#ifndef _GLFRAME_MAINWINDOW_HPP_
#define _GLFRAME_MAINWINDOW_HPP_

#include <QMainWindow>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#include "glframe_bargraph_widget.hpp"

namespace glretrace {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  virtual ~MainWindow();
  inline QQmlEngine *engine() { return quickWidget->engine(); }
  void create();  // Call this to make widgets with an OpenGL context.

 signals:
  void setTextInput(const QString &path);

 protected slots:
  void quickStatusChanged(QQuickWidget::Status status);
  void quickSceneGraphError(QQuickWindow::SceneGraphError error,
                             const QString &message);
  void updateTextInput(const QUrl &url);

 protected:
  QQuickWidget *quickWidget;
  QWidget *centralWidget;
  QVBoxLayout *layout;
  BarGraphWidget *graph;
};

}  // namespace glretrace

#endif  // _GLFRAME_MAINWINDOW_HPP_
