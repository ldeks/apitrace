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

#include "shaderdisplay.hpp"

#include <QIcon>
#include <QSizePolicy>
#include <QRegExp>

using glretrace::ShaderDisplay;
using glretrace::RenderShaders;

// From Qt style sheets examples "Customizing QAbstractScrollArea"
const char *
ShaderDisplay::sourceStyleSheet =
    "QTextEdit {\n"
    "  background-color: White;\n"
    "  border: 1px solid DarkGray;\n"
    "  selection-background-color: LightSteelBlue;\n"
    "}";

const char *
ShaderDisplay::findButtonStyleSheet =
    "QToolButton {\n"
    "  border: 1px solid DarkGray;\n"
    "  border-radius: 0px;\n"
    "  width: 23px;\n"
    "  height: 23px;\n"
    "}";

const char *
ShaderDisplay::findEditNormalStyleSheet =
    "QLineEdit {\n"
    "  border: 1px solid gray;\n"
    "  border-radius: 0px;\n"
    "  background-color: white;\n"
    "  color: Black;\n"
    "  width: 300px;\n"
    "  height: 26px;\n"
    "}";

const char *
ShaderDisplay::findEditRedStyleSheet =
    "QLineEdit {\n"
    "  border: 1px solid gray;\n"
    "  border-radius: 0px;\n"
    "  background-color: Salmon;\n"
    "  color: White;\n"
    "  width: 300px;\n"
    "  height: 26px;\n"
    "}";

const char *
ShaderDisplay::styleSheet =
    "QTextEdit {\n"
    "  background-color: transparent;\n"
    "  selection-background-color: LightSteelBlue;\n"
    "}";

ShaderDisplay::ShaderDisplay(QWidget *parent) : TabWidget(parent) {
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
  findEdit = new QLineEdit(source);
  findEdit->setClearButtonEnabled(true);
  findEdit->setPlaceholderText("Find in shader");
  findEdit->setStyleSheet(findEditNormalStyleSheet);
  findUp = new QToolButton(findEdit);
  findUp->setToolTip("Find previous");
  findUp->setArrowType(Qt::UpArrow);
  findUp->setStyleSheet(findButtonStyleSheet);
  findUp->setEnabled(false);
  findDown = new QToolButton(findEdit);
  findDown->setToolTip("Find next");
  findDown->setArrowType(Qt::DownArrow);
  findDown->setStyleSheet(findButtonStyleSheet);
  findDown->setEnabled(false);
  compileButton = new QPushButton("Compile", source);
  compileButton->setEnabled(false);
  compileSpacer = new QWidget(source);
  // Horizontal spacer
  compileSpacer->setSizePolicy(QSizePolicy::MinimumExpanding,
                               QSizePolicy::Maximum);
  compileLayout->addWidget(findEdit);
  compileLayout->addWidget(findUp);
  compileLayout->addWidget(findDown);
  compileLayout->addWidget(compileSpacer);
  compileLayout->addWidget(compileButton);
  sourceLayout->addWidget(compileArea);

  addTab(source, "Source");

  //Other tabs.
  ir = initTab("IR");
  ssa = initTab("SSA");
  nir = initTab("NIR");
  simd8 = initTab("Simd8");
  simd16 = initTab("Simd16");
  simd32 = initTab("Simd32");
  beforeUnification = initTab("Before Unification");
  afterUnification = initTab("After Unification");
  beforeOptimization = initTab("Before Optimization");
  constCoalescing = initTab("Const Coalescing");
  genIrLowering = initTab("Gen IR Lowering");
  layoutState = initTab("Layout");
  optimized = initTab("Optimized");
  pushAnalysis = initTab("Push Analysis");
  codeHoisting = initTab("Code Hoisting");
  codeSinking = initTab("Code Sinking");

  makeConnections();
}

ShaderDisplay::~ShaderDisplay() {
}

QTextEdit*
ShaderDisplay::initTab(QString name) {
  QTextEdit *tab = new QTextEdit(this);
  tab->setStyleSheet(styleSheet);
  tab->setFontFamily("monospace");
  addTab(tab, name);
  setTabVisible(tab, false); // Hide tab unless text.

  return tab;
}

void
ShaderDisplay::makeConnections() {
  connect(findEdit, &QLineEdit::textEdited,
          this, &ShaderDisplay::findRegExp);
  connect(findUp, &QToolButton::pressed,
          this, &ShaderDisplay::findPrevious);
  connect(findDown, &QToolButton::pressed,
          this, &ShaderDisplay::findNext);
}

void
ShaderDisplay::setText(QTextEdit *edit, QString text) {
  edit->setPlainText(text);
  // If user deletes all text in the shader, we don't want to
  // hide it suddenly.
  if (edit != source)
    setTabVisible(edit, !text.isEmpty());
}

void
ShaderDisplay::populate(RenderShaders *rs, QString shaderType) {
  setText(sourceText, rs->getShaderText(shaderType, "shader"));
  setText(ir, rs->getShaderText(shaderType, "ir"));
  setText(ssa, rs->getShaderText(shaderType, "ssa"));
  setText(nir, rs->getShaderText(shaderType, "nir"));
  setText(simd8, rs->getShaderText(shaderType, "simd8"));
  setText(simd16, rs->getShaderText(shaderType, "simd16"));
  setText(simd32, rs->getShaderText(shaderType, "simd32"));
  setText(beforeUnification,
          rs->getShaderText(shaderType, "beforeUnification"));
  setText(afterUnification,
          rs->getShaderText(shaderType, "afterUnification"));
  setText(beforeOptimization,
          rs->getShaderText(shaderType, "beforeOptimization"));
  setText(constCoalescing,
          rs->getShaderText(shaderType, "constCoalescing"));
  setText(genIrLowering,
          rs->getShaderText(shaderType, "genIrLowering"));
  setText(layoutState, rs->getShaderText(shaderType, "layout"));
  setText(optimized, rs->getShaderText(shaderType, "optimized"));
  setText(pushAnalysis,
          rs->getShaderText(shaderType, "pushAnalysis"));
  setText(codeHoisting,
          rs->getShaderText(shaderType, "codeHoisting"));
  setText(codeSinking,
          rs->getShaderText(shaderType, "codeSinking"));
}

bool
ShaderDisplay::hasText() {
  if (!sourceText->toPlainText().isEmpty())
    return true;
  else if (!ir->toPlainText().isEmpty())
    return true;
  else if (!ssa->toPlainText().isEmpty())
    return true;
  else if (!nir->toPlainText().isEmpty())
    return true;
  else if (!simd8->toPlainText().isEmpty())
    return true;
  else if (!simd16->toPlainText().isEmpty())
    return true;
  else if (!simd32->toPlainText().isEmpty())
    return true;
  else if (!beforeUnification->toPlainText().isEmpty())
    return true;
  else if (!afterUnification->toPlainText().isEmpty())
    return true;
  else if (!beforeOptimization->toPlainText().isEmpty())
    return true;
  else if (!constCoalescing->toPlainText().isEmpty())
    return true;
  else if (!genIrLowering->toPlainText().isEmpty())
    return true;
  else if (!layoutState->toPlainText().isEmpty())
    return true;
  else if (!optimized->toPlainText().isEmpty())
    return true;
  else if (!pushAnalysis->toPlainText().isEmpty())
    return true;
  else if (!codeHoisting->toPlainText().isEmpty())
    return true;
  else if (!codeSinking->toPlainText().isEmpty())
    return true;
  else
    return false;
}

void
ShaderDisplay::findRegExp(const QString &text) {
  if (text.isEmpty()) {
    findUp->setEnabled(false);
    findDown->setEnabled(false);
    findEdit->setStyleSheet(findEditNormalStyleSheet);
    return;
  }

  if (sourceText->find(QRegExp(text))) {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
    findUp->setEnabled(true);
    findDown->setEnabled(true);
  } else {
    findEdit->setStyleSheet(findEditRedStyleSheet);
    findUp->setEnabled(false);
    findDown->setEnabled(false);
  }
}

void
ShaderDisplay::findPrevious() {
  if (!sourceText->find(QRegExp(findEdit->text()),
                        QTextDocument::FindBackward)) {
    findEdit->setStyleSheet(findEditRedStyleSheet);

    //Go to the end of the document and check again.
    sourceText->moveCursor(QTextCursor::End);
    if (sourceText->find(QRegExp(findEdit->text()),
                         QTextDocument::FindBackward)) {
      findEdit->setStyleSheet(findEditNormalStyleSheet);
    }

  } else {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
  }
}

void
ShaderDisplay::findNext() {
  if (!sourceText->find(QRegExp(findEdit->text()))) {
    findEdit->setStyleSheet(findEditRedStyleSheet);

    // Go to the beginning of the document and check again.
    sourceText->moveCursor(QTextCursor::Start);
    if (sourceText->find(QRegExp(findEdit->text())))
      findEdit->setStyleSheet(findEditNormalStyleSheet);
  } else {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
  }
}
