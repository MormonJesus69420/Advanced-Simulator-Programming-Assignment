#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

// gmlib2
#include <gmlib2/qt/scene.h>

// qt
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Qt3DQuickExtras/Qt3DQuickWindow>

// stl
#include <memory>

class GuiApplication : public QGuiApplication {
  public:
  using Vector_Type = gmlib2::bases::AffineSpaceD3<>::Vector_Type;

  GuiApplication(int& argc, char** argv);

  void initializeScene();

  private:
  Qt3DExtras::Quick::Qt3DQuickWindow m_view;
  gmlib2::qt::Scene* m_scene{ nullptr };

  private slots:
  void onSceneCreated(QObject* root_object);
};

#endif // GUIAPPLICATION_H
