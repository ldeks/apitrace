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

#include "glframe_quickview_ui.hpp"

using glretrace::QQuickViewWidget;

QQuickViewWidget::QQuickViewWidget(const QUrl &url,
                                   QWidget *parent) : QWidget(parent) {
  view = new QQuickView();
  view->setResizeMode(QQuickView::SizeRootObjectToView);
  view->setSource(url);
  connect(view, SIGNAL(statusChanged(QQuickView::Status)),
          this, SIGNAL(statusChanged(QQuickView::Status)));
  connect(view, SIGNAL(sceneGraphError(
            QQuickWindow::SceneGraphError, const QString &)),
          this, SIGNAL(sceneGraphError(
            QQuickWindow::SceneGraphError, const QString &)));

  container = QWidget::createWindowContainer(view, this);
  container->setMinimumSize(view->size());
  container->setFocusPolicy(Qt::TabFocus);

  layout = new QVBoxLayout(this);
  this->setLayout(layout);
  layout->addWidget(container);
}

QQuickViewWidget::~QQuickViewWidget() {
  delete view;
}
