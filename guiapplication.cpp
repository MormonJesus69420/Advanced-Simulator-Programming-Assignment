#include "guiapplication.h"

// simulatoraspect
#include <simulatoraspecttypes.h>
#include <objectcontroller.h>
#include <simulatoraspect.h>
using namespace simaspect;

// gmlib2
#include <gmlib2/qt/exampleobjects/tensorproductsurfaces.h>
#include <gmlib2/qt/exampleobjects/parametriccurves.h>
using namespace gmlib2::qt;

// qt
#include <Qt3DQuick/QQmlAspectEngine>
#include <QQmlContext>
#include <Qt3DExtras>

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
  sphere1->translate(Vector_Type{ 4, 0, 9 });

  auto sphere2 = m_scene->createObject<TPSphere>();
  sphere2->translate(Vector_Type{ -4, 0, 9 });

  auto sphere3 = m_scene->createObject<TPSphere>();
  sphere3->translate(Vector_Type{ 0, 0, 2 });

  auto plane1 = m_scene->createObject<TPPlane>(Point3{ -5, -5, 0 }, Vector3{ 10, 0.0, 0.0 }, Vector3{ 0, 10, 0 });
  plane1->translate(Vector_Type{ 0, 0, 0 });

  auto plane2 = m_scene->createObject<TPPlane>(Point3{ -5, -5, 0 }, Vector3{ 0, 10, 0 }, Vector3{ 10, 0, 0 });
  plane2->translate(Vector_Type{ 0, 0, 10 });

  auto plane3 = m_scene->createObject<TPPlane>(Point3{ 0, 0, 0 }, Vector3{ 0, 10, 0 }, Vector3{ 0, 0, 10 });
  plane3->translate(Vector_Type{ -5, -5, 0 });

  auto plane4 = m_scene->createObject<TPPlane>(Point3{ 0, 0, 0 }, Vector3{ 0, 0, 10 }, Vector3{ 0, 10, 0 });
  plane4->translate(Vector_Type{ 5, -5, 0 });

  auto plane5 = m_scene->createObject<TPPlane>(Point3{ 0, 0, 0 }, Vector3{ 10, 0, 0 }, Vector3{ 0, 0, 10 });
  plane5->translate(Vector_Type{ -5, 5, 0 });

  auto plane6 = m_scene->createObject<TPPlane>(Point3{ 0, 0, 0 }, Vector3{ 0, 0, 10 }, Vector3{ 10, 0, 0 });
  plane6->translate(Vector_Type{ -5, -5, 0 });

  auto* controller_sphere_1 = new ObjectController();
  controller_sphere_1->setVelocity(QVector3D(-6, 0, -5));
  controller_sphere_1->setType(ObjectType::dynamic_obj);
  controller_sphere_1->setKind(ObjectKind::sphere);
  sphere1->addComponent(controller_sphere_1);

  auto* controller_sphere_2 = new ObjectController();
  controller_sphere_2->setVelocity(QVector3D(6, 0, -5));
  controller_sphere_2->setType(ObjectType::dynamic_obj);
  controller_sphere_2->setKind(ObjectKind::sphere);
  sphere2->addComponent(controller_sphere_2);

  auto* controller_sphere_3 = new ObjectController();
  controller_sphere_3->setVelocity(QVector3D(0, 0, 2));
  controller_sphere_3->setType(ObjectType::dynamic_obj);
  controller_sphere_3->setKind(ObjectKind::sphere);
  sphere3->addComponent(controller_sphere_3);

  auto* controller_plane_1 = new ObjectController();
  controller_plane_1->setType(ObjectType::static_obj);
  controller_plane_1->setKind(ObjectKind::plane);
  plane1->addComponent(controller_plane_1);

  auto* controller_plane_2 = new ObjectController();
  controller_plane_2->setType(ObjectType::static_obj);
  controller_plane_2->setKind(ObjectKind::plane);
  plane2->addComponent(controller_plane_2);

  auto* controller_plane_3 = new ObjectController();
  controller_plane_3->setType(ObjectType::static_obj);
  controller_plane_3->setKind(ObjectKind::plane);
  plane3->addComponent(controller_plane_3);

  auto* controller_plane_4 = new ObjectController();
  controller_plane_4->setType(ObjectType::static_obj);
  controller_plane_4->setKind(ObjectKind::plane);
  plane4->addComponent(controller_plane_4);

  auto* controller_plane_5 = new ObjectController();
  controller_plane_5->setType(ObjectType::static_obj);
  controller_plane_5->setKind(ObjectKind::plane);
  plane5->addComponent(controller_plane_5);

  auto* controller_plane_6 = new ObjectController();
  controller_plane_6->setType(ObjectType::static_obj);
  controller_plane_6->setKind(ObjectKind::plane);
  plane6->addComponent(controller_plane_6);
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
