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

#include "glframe_bargraph_widget.hpp"

using glretrace::BarGraphWidget;

BarGraphWidget::BarGraphWidget(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  setLayout(layout);
  // graph = new BarGraphView(this);
  metricsBoxes = new QWidget(this);

  metricsBoxesLayout = new QHBoxLayout(metricsBoxes);
  metricsBoxes->setLayout(metricsBoxesLayout);
  vertLabel = new QLabel("Vertical metric: ", metricsBoxes);
  vertLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  horizLabel = new QLabel("Horizontal metric: ", metricsBoxes);
  horizLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  vertBox = new QComboBox(metricsBoxes);
  horizBox = new QComboBox(metricsBoxes);
  metricsBoxesLayout->addWidget(vertLabel);
  metricsBoxesLayout->addWidget(vertBox);
  metricsBoxesLayout->addWidget(horizLabel);
  metricsBoxesLayout->addWidget(horizBox);

  // layout->addWidget(graph);
  layout->addWidget(metricsBoxes);
}

BarGraphWidget::~BarGraphWidget() {
}

void
BarGraphWidget::setModel(FrameRetraceModel *m) {
  graph->setModel(m);
}

void
BarGraphWidget::setSelection(QSelection *s) {
  graph->setSelection(s);
}
