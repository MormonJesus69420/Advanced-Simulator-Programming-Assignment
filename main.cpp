#include "guiapplication.h"

// gmlib2
#include <gmlib2/qt/integration.h>

// qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Qt3DCore>
#include <Qt3DExtras>

// stl
#include <chrono>
#include <memory>
using namespace std::chrono_literals;

int main(int argc, char** argv) try {

  gmlib2::qt::init::registerQmlTypes();

  GuiApplication app(argc, argv);
  return app.exec();
}
catch (std::runtime_error exception) {
  std::cout << "Runtime exception: " << exception.what() << std::endl;
}
