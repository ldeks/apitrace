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
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include "rendershaders.hpp"
#include "tabwidget.hpp"

namespace glretrace {

class ShaderEdit : public TabWidget {
  Q_OBJECT
 public:
  explicit ShaderEdit(QWidget *parent = 0);
  virtual ~ShaderEdit();

  void setText(QTextEdit *edit, QString text);
  void populate(RenderShaders *rs, QString shaderType);
  bool hasText();

 public slots:
  void findRegExp(const QString &text);
  void findPrevious();
  void findNext();

 private:
  QTextEdit* initTab(QString name);
  void makeConnections();

 protected:
  // Source tab.
  QWidget *source;
  QVBoxLayout *sourceLayout;
  static const char *sourceStyleSheet;
  QTextEdit *sourceText;
  QWidget *compileArea;
  QHBoxLayout *compileLayout;
  QLineEdit *findEdit;
  static const char *findEditNormalStyleSheet;
  static const char *findEditRedStyleSheet;
  static const char *findButtonStyleSheet;
  QToolButton *findUp;
  QToolButton *findDown;
  QPushButton *compileButton;
  QWidget *compileSpacer;

  // Other tabs.
  static const char *styleSheet;
  QTextEdit *ir;
  QTextEdit *ssa;
  QTextEdit *nir;
  QTextEdit *simd8;
  QTextEdit *simd16;
  QTextEdit *simd32;
  QTextEdit *beforeUnification;
  QTextEdit *afterUnification;
  QTextEdit *beforeOptimization;
  QTextEdit *constCoalescing;
  QTextEdit *genIrLowering;
  QTextEdit *layoutState;
  QTextEdit *optimized;
  QTextEdit *pushAnalysis;
  QTextEdit *codeHoisting;
  QTextEdit *codeSinking;
};

}  // namespace glretrace

#endif  // _SHADEREDIT_HPP_
