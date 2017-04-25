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

#include <algorithm>

using glretrace::BarGraphRenderer;
using glretrace::GraphWindow;

GraphWindow::GraphWindow(UpdateBehavior updateBehavior,
                         QWindow* parent) :
                         QOpenGLWindow(updateBehavior, parent),
                         renderer(NULL),
                         zoom(1.0),
                         translation(0.0),
                         clicked(false),
                         shift(false) {
  startPos = QPointF(-1.0, -1.0);
}

GraphWindow::~GraphWindow() {
  delete renderer;
}

void
GraphWindow::initializeGL() {
  delete renderer;
  renderer = new BarGraphRenderer();
  renderer->subscribe(this);
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
  if (renderer)
    renderer->setBars(bars.toStdVector());
}

void
GraphWindow::onBarSelect(const std::vector<int> selection) {
}

void
GraphWindow::mousePressEvent(QMouseEvent *e) {
  startPos = QPointF(e->pos());
  QPointF winSize = QPointF(width(), height());
  startPos.setX(startPos.x()/winSize.x());
  startPos.setY((winSize.y()-startPos.y())/winSize.y());
  mouseDrag(startPos.x(), startPos.y(),
            startPos.x(), startPos.y());
}

void
GraphWindow::mouseMoveEvent(QMouseEvent *e) {
  if (e->buttons() & Qt::LeftButton) {
      QPointF endPos, mousePos, winSize;
      winSize = QPointF(width(), height());
      mousePos = QPointF(e->pos());
      endPos.setX(mousePos.x()/winSize.x());
      endPos.setY((winSize.y()-mousePos.y())/winSize.y());
      mouseDrag(startPos.x(), startPos.y(),
                endPos.x(), endPos.y());
  }
}

void
GraphWindow::mouseReleaseEvent(QMouseEvent *e) {
  clicked = true;
  shift = e->modifiers() & Qt::ShiftModifier;
  update();
}

void
GraphWindow::wheelEvent(QWheelEvent *e) {
  float wheelx = 1.0;
  wheelx = ((float) e->x())/((float) width());
  mouseWheel(((float) e->angleDelta().y())/5, wheelx);
}

void
GraphWindow::mouseWheel(int degrees, float zoom_point_x) {
  float new_zoom = zoom + degrees / 360.0;
  if (new_zoom < 1.0) {
    new_zoom = 1.0;
  }

  float unzoomed_point = (zoom_point_x - translation) / zoom;
  float new_translate = zoom_point_x - unzoomed_point * new_zoom;
  // float new_translate = zoom_point_x * (1 - new_zoom);

  if (new_translate < 1 - new_zoom)
    new_translate = 1 - new_zoom;
  else if (new_translate > 0)
    new_translate = 0;

  translation = new_translate;
  emit translationChanged(translation);
  zoom = new_zoom;
  emit zoomChanged(zoom);

  if (renderer)
    renderer->setZoom(zoom, translation);
  update();
}

void
GraphWindow::mouseDrag(float x1, float y1, float x2, float y2) {
  if (clicked) {
    if (renderer) {
      renderer->selectMouseArea(shift);
      renderer->setMouseArea(0, 0, 0, 0);
    }
    clicked = false;
    shift = false;
  } else {
    if (renderer) {
      renderer->setMouseArea(std::min(x1, x2),
                             std::min(y1, y2),
                             std::max(x1, x2),
                             std::max(y1, y2));
    }
  }
  update();
}
