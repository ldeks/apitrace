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


#ifndef _APITAB_HPP_
#define _APITAB_HPP_

#include <QHBoxLayout>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QWidget>

#include "uimodel.hpp"
#include "shadertextwidget.hpp"

namespace glretrace {

class ApiTab : public QWidget {
  Q_OBJECT
 public:
  explicit ApiTab(QWidget *parent = 0);
  virtual ~ApiTab();

  UiModel* getModel() { return model; }
  void setModel(UiModel* mdl);

 public slots:
  void setRenders(QStringList r);
  void convertActivation(const QModelIndex &index);
  void activateShader(int index);
  void populate(QStringList textList);

 signals:
  void shaderActivated(int index);
  void printMessage(QString msg);

 protected:
  QHBoxLayout *layout;
  QStringList renders;
  QStringListModel *rendersModel;
  QListView *renderSelection;
  static const char *listStyleSheet;
  ShaderTextWidget *text;

  // Model
  UiModel* model;

 private:
  void makeConnections();
};

}  // namespace glretrace

#endif  // _APITAB_HPP_
