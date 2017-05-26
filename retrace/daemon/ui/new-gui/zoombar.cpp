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

#include "zoombar.hpp"

using glretrace::ZoomBar;

// From Qt style sheets examples "Customizing QScrollBar"
const char *
ZoomBar::scrollStyleSheet =
    "QToolButton {\n"
    "  height: 20px;\n"
    "  border: 2px;\n"
    "  background: #808080;\n"
    "}\n"
    "\n"
    "QScrollBar {\n"
    "  height: 23px;\n"
    "}\n"
    "QScrollBar::horizontal {\n"
    "  background: #808080;\n"
    "}\n"
    "\n"
    "QScrollBar::handle:horizontal {\n"
    "  background: #363636;\n"
    "}\n"
    "\n"
    "QScrollBar::add-line:horizontal {\n"
    "  background: #808080;\n"
    "}\n"
    "\n"
    "QScrollBar::sub-line:horizontal {\n"
    "  background: #808080;\n"
    "}";


ZoomBar::ZoomBar(QWidget *parent) : QWidget(parent) {
  layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  zoomInButton = new QToolButton(this);
  zoomInButton->setIcon(QIcon(":/images/zoom-in-symbolic.symbolic.png"));
  zoomInButton->setToolTip("Zoom in");
  zoomInButton->setIconSize(QSize(16, 16));
  zoomInButton->setAutoRaise(true);
  connect(zoomInButton, &QToolButton::clicked,
          this, &ZoomBar::zoomIn);
  scroll = new QScrollBar(Qt::Horizontal, this);
  setStyleSheet(scrollStyleSheet);
  zoomOutButton = new QToolButton(this);
  zoomOutButton->setIcon(QIcon(":/images/zoom-out-symbolic.symbolic.png"));
  zoomOutButton->setToolTip("Zoom out");
  zoomOutButton->setIconSize(QSize(16, 16));
  zoomOutButton->setAutoRaise(true);
  connect(zoomOutButton, &QToolButton::clicked,
          this, &ZoomBar::zoomOut);
  layout->addWidget(zoomOutButton);
  layout->addWidget(scroll);
  layout->addWidget(zoomInButton);
}

ZoomBar::~ZoomBar() {
}
