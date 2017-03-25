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

#ifndef _GLFRAME_QUICKVIEW_UI_HPP_
#define _GLFRAME_QUICKVIEW_UI_HPP_

#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#include <string>

namespace glretrace {

class QQuickViewWidget : public QWidget {
  Q_OBJECT

 public:
  explicit QQuickViewWidget(const QUrl &url, QWidget *parent = 0);
  virtual ~QQuickViewWidget();
  inline QQmlEngine *engine() { return view->engine(); }
  inline QQuickItem *rootObject() { return view->rootObject(); }

 signals:
  void statusChanged(QQuickWidget::Status);
  void sceneGraphError(QQuickWindow::SceneGraphError, const QString &);

 protected:
  QQuickWidget *view;
  QVBoxLayout *layout;
  QWidget *container;
};


}  // namespace glretrace

#endif  // _GLFRAME_QUICKVIEW_UI_HPP_
