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


#ifndef _GLFRAME_BARGRAPH_WIDGET_HPP_
#define _GLFRAME_BARGRAPH_WIDGET_HPP_

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "glframe_qbargraph.hpp"
#include "glframe_retrace_model.hpp"
#include "glframe_metrics_model.hpp"

namespace glretrace {

class BarGraphWidget : public QWidget {
  Q_OBJECT

 public:
  explicit BarGraphWidget(QWidget *parent = 0);
  virtual ~BarGraphWidget();

  void setModel(FrameRetraceModel *m);
  void setSelection(QSelection *s);

 protected:
  QVBoxLayout *layout;
  BarGraphView *graph;
  QWidget *metricsBoxes;
  QHBoxLayout *metricsBoxesLayout;
  QLabel *vertLabel;
  QLabel *horizLabel;
  QComboBox *vertBox;
  QComboBox *horizBox;
};

}  // namespace glretrace

#endif  // _GLFRAME_BARGRAPH_WIDGET_HPP_
