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

#include "shadertab.hpp"

#include <QSizePolicy>

using glretrace::ShaderTab;

// From Qt style sheets examples "Customizing QListView"
const char *
ShaderTab::listStyleSheet =
    "QListView {\n"
    "  background: transparent;\n"
    "  border: 0px;\n"
    "}\n"
    "\n"
    "QListView::item:selected:active {\n"
    "  background: LightSteelBlue;\n"
    "  border: 0px transparent;\n"
    "}\n"
    "\n"
    "QListView::item:selected:!active {\n"
    "  background: LightSteelBlue;\n"
    "  border: 0px transparent;\n"
    "}\n"
    "\n"
    "QListView::item:hover {\n"
    "  background: DarkGray;\n"
    "}";

ShaderTab::ShaderTab(QWidget *parent) : QWidget(parent) {
  layout = new QHBoxLayout(this);
  setLayout(layout);

  // Renders list.
  rendersModel = new QStringListModel(this);
  renderSelection = new QListView(this);
  renderSelection->setModel(rendersModel);
  renderSelection->setStyleSheet(listStyleSheet);
  renderSelection->setSizePolicy(QSizePolicy::Maximum,
                                 QSizePolicy::Expanding);
  layout->addWidget(renderSelection);

  // Tabs
  tabs = new QTabWidget(this);
  vertex = new ShaderEdit(this);
  tabs->addTab(vertex, "Vertex");
  fragment = new ShaderEdit(this);
  tabs->addTab(fragment, "Fragment");
  tesselation = new ShaderEdit(this);
  tabs->addTab(tesselation, "Tesselation");
  geometry = new ShaderEdit(this);
  tabs->addTab(geometry, "Geometry");
  compute = new ShaderEdit(this);
  tabs->addTab(compute, "Compute");
  layout->addWidget(tabs);

  makeConnections();
}

void
ShaderTab::makeConnections() {
  connect(renderSelection, &QListView::clicked,
          this, &ShaderTab::convertActivation);
}

ShaderTab::~ShaderTab() {
}

void
ShaderTab::setRenders(QStringList r) {
  renders = r;
  rendersModel->setStringList(renders);
}

void
ShaderTab::setModel(UiModel* mdl) {
  model = mdl;

  connect(model, &UiModel::renderStrings,
          this, &ShaderTab::setRenders);
  connect(this, &ShaderTab::shaderActivated,
          model, &UiModel::needShaderText);
  connect(model, &UiModel::shaderTextObject,
          this, &ShaderTab::populateTabs);
}

void
ShaderTab::convertActivation(const QModelIndex &index) {
  QVariant varIndex = rendersModel->data(index, Qt::DisplayRole);
  QString stringIndex = varIndex.toString();
  int intIndex = stringIndex.toInt();
  emit printMessage("activated");
  emit shaderActivated(intIndex);
}

void
ShaderTab::populateTabs(RenderShaders *rs) {
  populateEdit(rs, vertex, "vertex");
  populateEdit(rs, fragment, "fragment");
  populateEdit(rs, tesselation, "tesselation");
  populateEdit(rs, geometry, "geometry");
  populateEdit(rs, compute, "compute");
}

void
ShaderTab::populateEdit(RenderShaders *rs, ShaderEdit *edit,
                        QString shaderType) {
  edit->setText("source", rs->getShaderText(shaderType, "source"));
  edit->setText("ir", rs->getShaderText(shaderType, "ir"));
  edit->setText("ssa", rs->getShaderText(shaderType, "ssa"));
  edit->setText("nir", rs->getShaderText(shaderType, "nir"));
  edit->setText("simd8", rs->getShaderText(shaderType, "simd8"));
}
