#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "controller/controller.h"
#include "gui/mainwindow.h"
#include "image_processor/imageprocessor.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "Photolab_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  ImageProcessor imgProc;
  Controller ctrl(imgProc);
  MainWindow gui(ctrl);
  gui.show();

  return a.exec();
}
