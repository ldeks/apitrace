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
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANEDITILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Laura Ekstrand <laura@jlekstrand.net>
 **************************************************************************/


#ifndef _SHADEREDIT_HPP_
#define _SHADEREDIT_HPP_

#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

namespace glretrace {

class ShaderEdit : public QTabWidget {
  Q_OBJECT
 public:
  explicit ShaderEdit(QWidget *parent = 0);
  virtual ~ShaderEdit();

  void setText(QString tabname, QString text);

 private:
  QTextEdit* initTab(QString name);

 protected:
  // Source tab.
  QWidget *source;
  QVBoxLayout *sourceLayout;
  static const char *sourceStyleSheet;
  QTextEdit *sourceText;
  QWidget *compileArea;
  QHBoxLayout *compileLayout;
  QPushButton *compileButton;
  QWidget *compileSpacer;

  // Other tabs.
  static const char *styleSheet;
  QTextEdit *ir;
  QTextEdit *ssa;
  QTextEdit *nir;
  QTextEdit *simd8;
};

}  // namespace glretrace

#endif  // _SHADEREDIT_HPP_
