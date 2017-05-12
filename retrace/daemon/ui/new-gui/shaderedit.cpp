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

#include "shaderedit.hpp"

#include <QSizePolicy>

using glretrace::ShaderEdit;

// From Qt style sheets examples "Customizing QAbstractScrollArea"
const char *
ShaderEdit::sourceStyleSheet =
    "QTextEdit {\n"
    "  background-color: White;\n"
    "  border: 1px solid DarkGray;\n"
    "  selection-background-color: LightSteelBlue;\n"
    "}";

const char *
ShaderEdit::styleSheet =
    "QTextEdit {\n"
    "  background-color: transparent;\n"
    "  selection-background-color: LightSteelBlue;\n"
    "}";

ShaderEdit::ShaderEdit(QWidget *parent) : QTabWidget(parent) {
  // Source tab.
  source = new QWidget(this);
  sourceLayout = new QVBoxLayout(source);
  source->setLayout(sourceLayout);
  sourceLayout->setSpacing(0);
  sourceText = new QTextEdit(source);
  sourceText->setFontFamily("monospace");
  sourceText->setStyleSheet(sourceStyleSheet);
  sourceLayout->addWidget(sourceText);

  compileArea = new QWidget(source);
  compileLayout = new QHBoxLayout(compileArea);
  compileArea->setLayout(compileLayout);
  compileLayout->setSpacing(0);
  compileButton = new QPushButton("Compile", source);
  compileButton->setEnabled(false);
  compileSpacer = new QWidget(source);
  // Horizontal spacer
  compileSpacer->setSizePolicy(QSizePolicy::MinimumExpanding,
                               QSizePolicy::Maximum);
  compileLayout->addWidget(compileSpacer);
  compileLayout->addWidget(compileButton);
  sourceLayout->addWidget(compileArea);

  addTab(source, "Source");


  //Other tabs.
  initTab(ir, "IR");
  initTab(ssa, "SSA");
  initTab(nir, "NIR");
  initTab(simd8, "Simd8");
}

ShaderEdit::~ShaderEdit() {
}

void
ShaderEdit::initTab(QTextEdit *tab, QString name) {
  tab = new QTextEdit(this);
  tab->setStyleSheet(styleSheet);
  addTab(tab, name);
}

void
ShaderEdit::setText(QString tabname, QString text) {
  if (tabname == "source")
    sourceText->setText(text);
  else if (tabname == "ir")
    ir->setText(text);
  else if (tabname == "ssa")
    ssa->setText(text);
  else if (tabname == "nir")
    nir->setText(text);
  else if (tabname == "simd8")
    simd8->setText(text);
}
