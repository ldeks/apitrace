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

#include "tabwidget.hpp"

using glretrace::TabWidget;

TabWidget::TabWidget(QWidget *parent) : QWidget(parent) {
}

TabWidget::~TabWidget() {
}

int
TabWidget::addTab(QWidget *page, const QString &label) {
  // Check to see if this widget is already in here.
  // If it's in there, it's unhidden.
  idx = QTabWidget::indexOf(page);
  if (idx != -1)
    return idx;

  // Add the widget
  idx = QTabWidget::addTab(page, label);
  indices.append(idx);
  tabs.append(page);
  names.append(label);

  return idx;
}

void
TabWidget::hideTab(QWidget *tab) {
  if (isHidden(tab))
    return;

  // nidx - "native index" as opposed to index according to QTabWidget
  int nidx = tabs.indexOf(tab);
  if (nidx == -1)
    return;

  QTabWidget::removeTab(indices[nidx]);
}

void
TabWidget::showTab(QWidget *tab) {
  if (!isHidden(tab))
    return;

  // nidx - "native index" - native to this inherited class
  int nidx = tabs.indexOf(tab);
  if (nidx == -1)
    return;

  // idx - index according to QTabWidget
  int idx = QTabWidget::insertTab(indices[nidx], page,
                                  names.at(nidx));
  indices[nidx] = idx;
}

bool
TabWidget::isHidden(QWidget *tab) {
  if (-1 == QTabWidget::indexOf(tab))
    return true;
  else
    return false;
}
