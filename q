diff --git a/software/tulip/src/main.cpp b/software/tulip/src/main.cpp
index df109c1..a3145bf 100644
--- a/software/tulip/src/main.cpp
+++ b/software/tulip/src/main.cpp
@@ -39,6 +39,16 @@
 #include <tulip/PluginManager.h>
 #include <tulip/QuaZIPFacade.h>
 #include <tulip/TlpQtTools.h>
+#include <tulip/PluginLister.h>
+#include <tulip/Perspective.h>
+#include <tulip/GlMainWidget.h>
+#include <tulip/MouseInteractors.h>
+#include <tulip/GlBox.h>
+#include <tulip/GlGraphComposite.h>
+#include <tulip/GlGraphRenderingParameters.h>
+#include <tulip/TulipViewSettings.h>
+#include <tulip/CSVImportWizard.h>
+#include <tulip/CSVParser.h>
 
 #include <CrashHandling.h>
 
@@ -48,7 +58,10 @@
 #include "TulipSplashScreen.h"
 #include "PluginsCenter.h"
 #include "FormPost.h"
+#include "TulipPerspectiveProcessHandler.h"
 #include <tulip/SystemDefinition.h>
+#include <tulip/ForEach.h>
+#include <tulip/LayoutProperty.h>
 
 #if defined(__APPLE__)
 #include <sys/types.h>
@@ -63,6 +76,11 @@
 #undef interface
 #endif
 
+#include <iostream>
+
+using namespace tlp;
+using namespace std;
+
 void sendUsageStatistics() {
   QNetworkAccessManager* mgr = new QNetworkAccessManager;
   QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));
@@ -175,6 +193,11 @@ int main(int argc, char **argv) {
   delete dispatcher;
   delete splashScreen;
 
+//  QVariantMap *a = new QVariantMap();
+
+//  TulipPerspectiveProcessHandler::instance()->createPerspective("Tulip","",*a);
+
+
   // Main window
   TulipMainWindow *mainWindow = TulipMainWindow::instance();
   mainWindow->pluginsCenter()->reportPluginErrors(errorReport->errors());
@@ -192,7 +215,6 @@ int main(int argc, char **argv) {
   else if (!perspName.isNull()) { // open the perspective passed as argument
     mainWindow->createPerspective(perspName);
   }
-
   int result = tulip_agent.exec();
 
 #ifdef MEMORYCHECKER_ON
@@ -208,3 +230,96 @@ int main(int argc, char **argv) {
 
   return result;
 }
+
+
+
+
+/*int main(int argc, char **argv) {
+  start_crash_handler();
+
+  QApplication tulip_agent(argc, argv);
+  QString name("Tulip ");
+
+  // show patch number only if needed
+  if (TULIP_INT_VERSION % 10)
+    name += TULIP_VERSION;
+  else
+    name += TULIP_MM_VERSION;
+
+  // the applicationName below is used to identify the location
+  // of downloaded plugins, so it must be the same as in
+  // tulip_perspective/main.cpp
+  tulip_agent.setApplicationName(name);
+
+#if defined(__APPLE__)
+  // allows to load qt imageformats plugin
+  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
+#endif
+
+  PluginLoaderReporter *errorReport = new PluginLoaderReporter();
+
+  TulipMainWindow *mainWindow = TulipMainWindow::instance();
+  mainWindow->pluginsCenter()->reportPluginErrors(errorReport->errors());
+  delete errorReport;
+
+  // Initialize the library and load all plugins
+  tlp::initTulipSoftware();
+
+  Graph* g = newGraph();
+
+
+////  CSVParser *parser = new CSVSimpleParser("/home/jules/SAMOGWAS/Data/CsvRandomData/nodes.csv",",");
+
+
+  CSVImportWizard wizard(mainWindow);
+  wizard.setGraph(g);
+
+
+  wizard.accept();
+//    applyRandomLayout(g);
+  g->push();
+
+  // Creates the main widget that will display our graph
+  GlMainWidget* mainWidget = new GlMainWidget(NULL);
+
+  // Adds a layer to the scene
+  GlLayer* mainLayer = mainWidget->getScene()->createLayer("Main");
+
+  // Adds the graph to this layer
+  mainLayer->addGraph(g,"graph");
+
+//  tlp::openProject("/home/jules/SAMOGWAS/Tulip4.6/TulipProjects/testCSV.tlpx");
+   //Sets some rendering parameters on the graph to visualize
+  setGraphRenderingParameters(mainWidget->getScene()->getGlGraphComposite());
+
+  // Display the widget
+  mainWidget->show();
+
+  // Flush event loop in order to let paint events pass through in order for the scene to be initialized.
+  QApplication::processEvents();
+
+  // Center the camera and draw the graph
+  mainWidget->centerScene();
+  mainWidget->draw();
+
+  // Adds Zoom and pan navigation to the widget
+  mainWidget->installEventFilter(new MouseNKeysNavigator);
+
+
+
+
+  int result = tulip_agent.exec();
+
+#ifdef MEMORYCHECKER_ON
+  memory_checker_print_report();
+#endif // MEMORYCHECKER_ON
+
+#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
+  QFile f(QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip.lck"));
+#else
+  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
+#endif
+  f.remove();
+
+  return result;
+}*/
