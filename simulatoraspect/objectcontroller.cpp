#include "objectcontroller.h"

// gmlib2
#include <gmlib2/qt/scene.h>
using namespace gmlib2::qt;

namespace simaspect {

ObjectController::ObjectController(Qt3DCore::QNode* parent)
    : Qt3DCore::QComponent(parent)
{
}

void ObjectController::sceneChangeEvent(const Qt3DCore::QSceneChangePtr& change)
{
  if (change->type() == Qt3DCore::PropertyUpdated) {
    const auto e = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(change);
    if (e->propertyName() == QByteArrayLiteral("position_matrix")) {
      const auto updatedPosition = e->value().value<QMatrix4x4>();
      const auto was_blocked = blockNotifications(true);
      emit positionUpdated(updatedPosition);
      blockNotifications(was_blocked);
      return;
    }
  }

  QComponent::sceneChangeEvent(change);
}

Qt3DCore::QNodeCreatedChangeBasePtr ObjectController::createNodeCreationChange() const
{
  // try to cast parent that is a scene object to TPSphere pointer; s is evaluated, if nullptr then false.
  if (auto* s = dynamic_cast<TPSphere*>(parent()); s) {
    Qt3DCore::QNodeCreatedChangePtr<SphereObject> creationChange = Qt3DCore::QNodeCreatedChangePtr<SphereObject>::create(this);
    SphereObject& data = creationChange->data;
    auto pos = s->globalPositionPoint3();
    auto dir = s->globalDirectionVector3();
    auto up = s->globalUpVector3();
    data.pos = QVector3D(pos[0], pos[1], pos[2]);
    data.dir = QVector3D(dir[0], dir[1], dir[2]);
    data.up = QVector3D(up[0], up[1], up[2]);
    data.type = type;
    data.radius = s->m_radius;
    data.ds = velocity;
    connect(this, &ObjectController::positionUpdated, s, &SceneObject::setLocalFromGlobalFrame);

    return creationChange;
  }
  else if (auto* p = dynamic_cast<TPPlane*>(parent()); p) {
    Qt3DCore::QNodeCreatedChangePtr<PlaneObject> creationChange = Qt3DCore::QNodeCreatedChangePtr<PlaneObject>::create(this);
    PlaneObject& data = creationChange->data;
    auto pos = p->globalPositionPoint3();
    auto dir = p->globalDirectionVector3();
    auto up = p->globalUpVector3();
    data.pos = QVector3D(pos[0], pos[1], pos[2]);
    data.dir = QVector3D(dir[0], dir[1], dir[2]);
    data.up = QVector3D(up[0], up[1], up[2]);
    data.type = type;
    // 0UL, unsinged shlong
    Vector3 normal = blaze::normalize(blaze::cross(blaze::subvector(p->m_u, 0UL, 3UL), blaze::subvector(p->m_v, 0UL, 3UL)));
    data.normal = QVector3D(normal[0], normal[1], normal[2]);
    connect(this, &ObjectController::positionUpdated, p, &SceneObject::globalFrameChanged);

    return creationChange;
  }

  Qt3DCore::QNodeCreatedChangePtr<PhysObject> creationChange = Qt3DCore::QNodeCreatedChangePtr<PhysObject>::create(this);
  auto& data = creationChange->data;
  data = PhysObject();
  return creationChange;
}

const QVector3D& ObjectController::getVelocity() const
{
  return velocity;
}

void ObjectController::setVelocity(const QVector3D& velocity)
{
  this->velocity = velocity;
  emit velocityChanged(velocity);
}

bool ObjectController::isDynamic() const
{
  return type == ObjectType::dynamic_obj;
}

void ObjectController::setType(ObjectType type)
{
  this->type = type;
}

ObjectKind ObjectController::getKind() const
{
  return kind;
}

void ObjectController::setKind(ObjectKind kind)
{
  this->kind = kind;
}

} // namespace simaspect
