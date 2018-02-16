#ifndef OBJECTCONTROLLER_H
#define OBJECTCONTROLLER_H
// gmlib2
#include <gmlib2/qt/sceneobject.h>

// qt
#include <QMatrix4x4>
#include <Qt3DCore/QComponent>
#include <Qt3DCore/QTransform>

// Object structs
#include "structs.h"

#include "simulatoraspecttypes.h"

namespace simaspect {

struct ObjectControllerData {
  QVector3D velocity;
  PhysObject phys_object;
};

class ObjectController : public Qt3DCore::QComponent {
  Q_OBJECT
  Q_PROPERTY(QVector3D velocity READ getVelocity WRITE setVelocity NOTIFY velocityChanged)

  public:
  ObjectController(Qt3DCore::QNode* parent = nullptr);
  ~ObjectController() override = default;

  void setVelocity(const QVector3D& velocity);
  void collisionDetection(seconds_type dt);
  const QVector3D& getVelocity() const;
  void setType(ObjectType type);
  void setKind(ObjectKind kind);
  ObjectKind getKind() const;
  bool isDynamic() const;

  private:
  PhysObject phys_object;
  QVector3D velocity;
  ObjectType type;
  ObjectKind kind;

  signals:
  void collisionComputed(const QVariant& time);
  void positionUpdated(const QMatrix4x4& pos);
  void velocityChanged(const QVariant& time);

  // QNode interface
  protected:
  void sceneChangeEvent(const Qt3DCore::QSceneChangePtr& change) override;

  private:
  Qt3DCore::QNodeCreatedChangeBasePtr createNodeCreationChange() const override;
};
} // namespace simaspect

#endif // OBJECTCONTROLLER_H
