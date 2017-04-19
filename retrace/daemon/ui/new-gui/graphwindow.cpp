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

#include "graphwindow.hpp"

using glretrace::BarGraphRenderer;
using glretrace::GraphWindow;

GraphWindow::GraphWindow(UpdateBehavior updateBehavior,
                         QWindow* parent) :
                         QOpenGLWindow(updateBehavior, parent),
                         renderer(NULL) {
}

GraphWindow::~GraphWindow() {
  delete renderer;
}

void
GraphWindow::initializeGL() {
  delete renderer;
  renderer = new BarGraphRenderer(true);
}

void
GraphWindow::paintGL() {
  if (renderer)
    renderer->render();
}

void
GraphWindow::resizeGL(int w, int h) {
  if (renderer)
    renderer->setMouseArea(0, 0, w, h);
}

void
GraphWindow::setBars(QVector<BarMetrics> bars) {
  renderer->setBars(bars.toStdVector());
}
