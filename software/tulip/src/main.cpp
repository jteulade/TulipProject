/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <QLocale>
#include <QProcess>
#include <QDir>

#include <QApplication>
#include <QMessageBox>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QTcpSocket>

#include <tulip/TulipRelease.h>
#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>
#include <tulip/QuaZIPFacade.h>
#include <tulip/TlpQtTools.h>
//#include <tulip/PluginLister.h>
//#include <tulip/Perspective.h>
//#include <tulip/GlMainWidget.h>
//#include <tulip/MouseInteractors.h>
//#include <tulip/GlBox.h>
//#include <tulip/GlGraphComposite.h>
//#include <tulip/GlGraphRenderingParameters.h>
//#include <tulip/TulipViewSettings.h>
//#include <tulip/CSVImportWizard.h>
//#include <tulip/CSVParser.h>

#include <CrashHandling.h>

#include "TulipMainWindow.h"
#include "PluginLoaderReporter.h"
#include "PluginLoaderDispatcher.h"
#include "TulipSplashScreen.h"
#include "PluginsCenter.h"
#include "FormPost.h"
//#include "TulipPerspectiveProcessHandler.h"
#include <tulip/SystemDefinition.h>
//#include <tulip/ForEach.h>
//#include <tulip/LayoutProperty.h>

#if defined(__APPLE__)
#include <sys/types.h>
#include <signal.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef interface
#undef interface
#endif

//#include <iostream>

//using namespace tlp;
//using namespace std;

void sendUsageStatistics() {
  QNetworkAccessManager* mgr = new QNetworkAccessManager;
  QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));
  mgr->get(QNetworkRequest(QUrl(QString("http://tulip.labri.fr/TulipStats/tulip_stats.php?tulip=") + TULIP_VERSION + "&os=" + OS_PLATFORM)));
}

bool sendAgentMessage(int port, const QString& message) {
  bool result = true;

  QTcpSocket sck;
  sck.connectToHost(QHostAddress::LocalHost,port);
  sck.waitForConnected(1000);

  if (sck.state() == QAbstractSocket::ConnectedState) {
    sck.write(message.toUtf8());
    sck.flush();
  }
  else {
    result = false;
  }

  sck.close();
  return result;
}

void checkTulipRunning(const QString& perspName, const QString& fileToOpen) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  QFile lockFile(QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip.lck"));
#else
  QFile lockFile(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
#endif

  if (lockFile.exists() && lockFile.open(QIODevice::ReadOnly)) {
    QString agentPort = lockFile.readAll();
    bool ok;
    int n_agentPort = agentPort.toInt(&ok);

    if (ok && sendAgentMessage(n_agentPort,"SHOW_AGENT\tPROJECTS")) {

      QMessageBox::warning(NULL, "Warning", "An instance of Tulip agent is already running,\ndisplaying it...");

      // if a file was passed as argument, forward it to the running instance
      if (!fileToOpen.isNull()) { // open the file passed as argument
        if (!perspName.isNull()) {
          sendAgentMessage(n_agentPort,"OPEN_PROJECT_WITH\t" + perspName + "\t" + fileToOpen);
        }
        else {
          sendAgentMessage(n_agentPort,"OPEN_PROJECT\t" + fileToOpen);
        }
      }
      else if (!perspName.isNull()) { // open the perspective passed as argument
        sendAgentMessage(n_agentPort,"CREATE_PERSPECTIVE\t" + perspName);
      }

      exit(0);
    }
  }

  lockFile.close();
  lockFile.remove();
}

int main(int argc, char **argv) {
  start_crash_handler();

  QApplication tulip_agent(argc, argv);
  QString name("Tulip ");

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    name += TULIP_VERSION;
  else
    name += TULIP_MM_VERSION;

  // the applicationName below is used to identify the location
  // of downloaded plugins, so it must be the same as in
  // tulip_perspective/main.cpp
  tulip_agent.setApplicationName(name);

#if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif

  // Parse arguments
  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QString perspName;
  QString fileToOpen;

  for(int i=1; i<QApplication::arguments().size(); ++i) {
    QString s = QApplication::arguments()[i];

    if (perspectiveRegexp.exactMatch(s)) {
      perspName = perspectiveRegexp.cap(1);
    }
    else {
      fileToOpen = s;
    }
  }

  checkTulipRunning(perspName,fileToOpen);
  sendUsageStatistics();

  PluginLoaderDispatcher *dispatcher = new PluginLoaderDispatcher();
  TulipSplashScreen *splashScreen = new TulipSplashScreen();
  PluginLoaderReporter *errorReport = new PluginLoaderReporter();
  dispatcher->registerLoader(errorReport);
  dispatcher->registerLoader(splashScreen);
  tlp::initTulipSoftware(dispatcher,true);
  delete dispatcher;
  delete splashScreen;

//  QVariantMap *a = new QVariantMap();

//  TulipPerspectiveProcessHandler::instance()->createPerspective("Tulip","",*a);


  // Main window
  TulipMainWindow *mainWindow = TulipMainWindow::instance();
  mainWindow->pluginsCenter()->reportPluginErrors(errorReport->errors());
  delete errorReport;

  mainWindow->show();

  // Treat arguments
  if (!fileToOpen.isNull()) { // open the file passed as argument
    if (!perspName.isNull())
      mainWindow->openProjectWith(fileToOpen,perspName);
    else
      mainWindow->openProject(fileToOpen);
  }
  else if (!perspName.isNull()) { // open the perspective passed as argument
    mainWindow->createPerspective(perspName);
  }
  int result = tulip_agent.exec();

#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  QFile f(QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip.lck"));
#else
  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
#endif
  f.remove();

  return result;
}




/*int main(int argc, char **argv) {
  start_crash_handler();

  QApplication tulip_agent(argc, argv);
  QString name("Tulip ");

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    name += TULIP_VERSION;
  else
    name += TULIP_MM_VERSION;

  // the applicationName below is used to identify the location
  // of downloaded plugins, so it must be the same as in
  // tulip_perspective/main.cpp
  tulip_agent.setApplicationName(name);

#if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif

  PluginLoaderReporter *errorReport = new PluginLoaderReporter();

  TulipMainWindow *mainWindow = TulipMainWindow::instance();
  mainWindow->pluginsCenter()->reportPluginErrors(errorReport->errors());
  delete errorReport;

  // Initialize the library and load all plugins
  tlp::initTulipSoftware();

  Graph* g = newGraph();


////  CSVParser *parser = new CSVSimpleParser("/home/jules/SAMOGWAS/Data/CsvRandomData/nodes.csv",",");


  CSVImportWizard wizard(mainWindow);
  wizard.setGraph(g);


  wizard.accept();
//    applyRandomLayout(g);
  g->push();

  // Creates the main widget that will display our graph
  GlMainWidget* mainWidget = new GlMainWidget(NULL);

  // Adds a layer to the scene
  GlLayer* mainLayer = mainWidget->getScene()->createLayer("Main");

  // Adds the graph to this layer
  mainLayer->addGraph(g,"graph");

//  tlp::openProject("/home/jules/SAMOGWAS/Tulip4.6/TulipProjects/testCSV.tlpx");
   //Sets some rendering parameters on the graph to visualize
  setGraphRenderingParameters(mainWidget->getScene()->getGlGraphComposite());

  // Display the widget
  mainWidget->show();

  // Flush event loop in order to let paint events pass through in order for the scene to be initialized.
  QApplication::processEvents();

  // Center the camera and draw the graph
  mainWidget->centerScene();
  mainWidget->draw();

  // Adds Zoom and pan navigation to the widget
  mainWidget->installEventFilter(new MouseNKeysNavigator);




  int result = tulip_agent.exec();

#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  QFile f(QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip.lck"));
#else
  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
#endif
  f.remove();

  return result;
}*/
