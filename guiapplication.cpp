#include "guiapplication.h"

// simulatoraspect
#include <simulatoraspecttypes.h>
#include <objectcontroller.h>
#include <simulatoraspect.h>
using namespace simaspect;

// gmlib2
#include <gmlib2/qt/exampleobjects/parametriccurves.h>
#include <gmlib2/qt/exampleobjects/tensorproductsurfaces.h>
using namespace gmlib2::qt;

// qt
#include <QQmlContext>
#include <Qt3DQuick/QQmlAspectEngine>

// stl
#include <exception>

GuiApplication::GuiApplication(int& argc, char** argv)
    : QGuiApplication(argc, argv)
{
  m_view.engine()->aspectEngine()->registerAspect(new SimulatorAspect);

  connect(m_view.engine(), &Qt3DCore::Quick::QQmlAspectEngine::sceneCreated,
      this, &GuiApplication::onSceneCreated);

  m_view.setGeometry(50, 50, 800, 600);
  m_view.setSource(QUrl("qrc:/main.qml"));
  m_view.show();
}

void GuiApplication::initializeScene()
{
  if (not m_scene)
    return;

  auto sphere1 = m_scene->createObject<TPSphere>();
  sphere1->translate(Vector_Type{ 3.9f, 0.0f, 7.0f });

  auto plane1 = m_scene->createObject<TPPlane>(
      Point3{ -5, -5, 0 },
      Vector3{ 10, 0, 0 },
      Vector3{ 0, 10, 0 });

  auto plane2 = m_scene->createObject<TPPlane>(
      Point3{ -5, -5, 0 },
      Vector3{ 0, 10, 0 },
      Vector3{ 10, 0, 0 });
  plane2->translate(Vector_Type{ 0, 0, 12 });

  auto plane3 = m_scene->createObject<TPPlane>(
      Point3{ 0, 0, 0 },
      Vector3{ 0, 10, 0 },
      Vector3{ 0, 0, 10 });
  plane3->translate(Vector_Type{ -5, -5, 0 });

  auto plane4 = m_scene->createObject<TPPlane>(
      Point3{ 0, 0, 0 },
      Vector3{ 0, 0, 10 },
      Vector3{ 0, 10, 0 });
  plane4->translate(Vector_Type{ 5, -5, 0 });

  auto* object_controller_1 = new ObjectController();
  object_controller_1->setVelocity(QVector3D(-6, 0, -5));
  object_controller_1->setType(ObjectType::dynamic_obj);
  object_controller_1->setKind(ObjectKind::sphere);
  sphere1->addComponent(object_controller_1);

  auto* object_controller_2 = new ObjectController();
  object_controller_2->setType(ObjectType::static_obj);
  object_controller_2->setKind(ObjectKind::plane);
  plane1->addComponent(object_controller_2);

  auto* object_controller_3 = new ObjectController();
  object_controller_3->setType(ObjectType::static_obj);
  object_controller_3->setKind(ObjectKind::plane);
  plane2->addComponent(object_controller_3);

  auto* object_controller_4 = new ObjectController();
  object_controller_4->setType(ObjectType::static_obj);
  object_controller_4->setKind(ObjectKind::plane);
  plane3->addComponent(object_controller_4);

  auto* object_controller_5 = new ObjectController();
  object_controller_5->setType(ObjectType::static_obj);
  object_controller_5->setKind(ObjectKind::plane);
  plane4->addComponent(object_controller_5);
}

void GuiApplication::onSceneCreated(QObject* root_object)
{
  qDebug() << "Scene created";
  if (not root_object) {
    qDebug() << "  invalid root object";
    return;
  }

  auto scene_root_entity_name = QString("scene_root_entity");
  auto* scene_root = root_object->findChild<Scene*>(
      scene_root_entity_name, Qt::FindChildrenRecursively);
  if (not scene_root) {
    qWarning() << "  No <Scene> root object defined!!";
    return;
  }

  m_scene = scene_root;

  initializeScene();
}
