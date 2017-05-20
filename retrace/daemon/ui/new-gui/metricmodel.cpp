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

#include "metricmodel.hpp"

#include <QList>

using glretrace::MetricModel;
using glretrace::MetricId;

MetricModel::MetricModel(const std::vector<MetricId> &idsVector,
                         const std::vector<std::string> &namesVector,
                         const std::vector<std::string> &descVector,
                         QObject *parent) : QObject(parent) {
  int length = idsVector.size();
  assert(length == namesVector.size());
  assert(length == descVector.size());

  for (int i = 0; i < length; i++) {
    indicesByName.insert(QString::fromStdString(namesVector[i]), i);
    indicesById.insert(hashId(idsVector[i]), i);
    names.append(QString::fromStdString(namesVector[i]));
    ids.append(idsVector[i]);
    descriptions.append(QString::fromStdString(descVector[i]));
  }
}

MetricModel::~MetricModel() {
}

MetricId
MetricModel::getId(QString name) {
  if (!indicesByName.contains(name))
    return MetricId();

  int index = indicesByName.value(name);
  return ids[index];
}

QString
MetricModel::getName(MetricId id) {
  if (!indicesById.contains(hashId(id)))
    return QString();

  int index = indicesById.value(hashId(id));
  return names[index];
}

QVector<QString>
MetricModel::getNames() {
  return names;
}

QStringList
MetricModel::getNamesList() {
  return QStringList(QList<QString>::fromVector(names));
}

QString
MetricModel::hashId(MetricId id) {
  return QString::number(quint64(id()), 2);
}