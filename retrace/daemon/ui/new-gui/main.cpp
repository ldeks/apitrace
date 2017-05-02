// Copyright (C) Intel Corp.  2017.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Laura Ekstrand <laura@jlekstrand.net>
//  **********************************************************************/

#include <QApplication>

#include "glframe_glhelper.hpp"
#include "glframe_logger.hpp"
#include "glframe_os.hpp"
#include "glframe_socket.hpp"
#include "glretrace.hpp"

#include "mainwindow.hpp"
#include "uimodel.hpp"

using glretrace::GlFunctions;
using glretrace::Logger;
using glretrace::MainWindow;
using glretrace::ServerSocket;
using glretrace::Socket;
using glretrace::UiModel;

int main(int argc, char *argv[]) {
  GlFunctions::Init();
  Logger::Create();
  Logger::SetSeverity(glretrace::WARN);
  Socket::Init();
  Logger::Begin();

  QApplication app(argc, argv);
  app.setOrganizationName("Open Source Technology Center");
  app.setOrganizationDomain("intel.com");
  app.setApplicationName("fr-new-gui");

  // Allows for passing QVector<float> through signals and slots.
  qRegisterMetaType<QVector<float> >("QVector<float>");

  MainWindow mwindow;
  UiModel model;
  mwindow.setModel(&model);
  mwindow.execDialog();
  mwindow.errorMessage("Sample fatal", "I have to close", true);

  int ret = app.exec();
  Logger::Destroy();
  Socket::Cleanup();
  return ret;
}
