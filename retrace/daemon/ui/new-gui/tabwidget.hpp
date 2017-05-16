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


#ifndef _TABWIDGET_HPP_
#define _TABWIDGET_HPP_

#include <QBitArray>
#include <QStringList>
#include <QTabWidget>
#include <QVector>
#include <QWidget>

namespace glretrace {

class TabWidget : public QTabWidget {
  Q_OBJECT
 public:
  explicit TabWidget(QWidget *parent = 0);
  virtual ~TabWidget();

  int addTab(QWidget *page, const QString &label);
  void hideTab(QWidget *tab);
  void showTab(QWidget *tab);
  bool isHidden(QWidget *tab);

 protected:
  QVector<int> indices;
  QVector<QWidget *> tabs;
  QStringList names;
  QBitArray mask;
  int tabCount;
};

}  // namespace glretrace

#endif  // _TABWIDGET_HPP_
