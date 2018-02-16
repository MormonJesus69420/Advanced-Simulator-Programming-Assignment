#include "objectcontrollerbackend.h"
#include "objectcontroller.h"
#include "simulatoraspect.h"

namespace simaspect {

ObjectControllerBackend::ObjectControllerBackend(SimulatorAspect* aspect)
    : Qt3DCore::QBackendNode(Qt3DCore::QBackendNode::ReadWrite)
{
  this->aspect = aspect;
}

void ObjectControllerBackend::translate(seconds_type dt)
{
  const auto dt_teim = float(dt.count());
  const QVector3D tarslation = velocity * dt_teim;

  auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
  e->setPropertyName(QByteArrayLiteral("trans_vector"));
  e->setDeliveryFlags(Qt3DCore::QSceneChange::Nodes);
  e->setValue(QVariant::fromValue(tarslation));
  notifyObservers(e);
}

void ObjectControllerBackend::updatePositions(AffineSpaceD3 position)
{
  auto dir = position.direction();
  auto pos = position.position();
  auto side = position.side();
  auto up = position.up();
  QMatrix4x4 new_pos;

  for (int i = 0; i < 4; ++i) {
    new_pos(i, 0) = float(dir[i]);
    new_pos(i, 1) = float(side[i]);
    new_pos(i, 2) = float(up[i]);
    new_pos(i, 3) = float(pos[i]);
  }

  auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
  e->setPropertyName(QByteArrayLiteral("position_matrix"));
  e->setDeliveryFlags(Qt3DCore::QSceneChange::Nodes);
  e->setValue(QVariant::fromValue(new_pos));
  notifyObservers(e);
}

void ObjectControllerBackend::checkCollisions(seconds_type dt)
{
  const auto dt_teim = float(dt.count());

  auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
  e->setDeliveryFlags(Qt3DCore::QSceneChange::Nodes);
  e->setPropertyName(QByteArrayLiteral("collision"));
  e->setValue(QVariant::fromValue(dt_teim));
  notifyObservers(e);
}

void ObjectControllerBackend::initializeFromPeer(
    const Qt3DCore::QNodeCreatedChangeBasePtr& change)
{
  if (auto pointer = qSharedPointerDynamicCast<Qt3DCore::QNodeCreatedChange<SphereObject>>(change); pointer) {
    auto obj = pointer->data;
    auto data = SphereBackendData();

    HomoVector dir{ obj.dir.x(), obj.dir.y(), obj.dir.z(), 0 };
    HomoVector pos{ obj.pos.x(), obj.pos.y(), obj.pos.z(), 1 };
    HomoVector up{ obj.up.x(), obj.up.y(), obj.up.z(), 0 };
    data.position.setFrame(pos, dir, up);

    data.velocity = HomoVector{ obj.ds.x(), obj.ds.y(), obj.ds.z(), 0 };
    data.radius = obj.radius;

    if (obj.type == ObjectType::dynamic_obj)
      aspect->registerDynamicObject(this->peerId(), data);
    else
      aspect->registerStaticObject(this->peerId(), data);
  }
  if (auto pointer = qSharedPointerDynamicCast<Qt3DCore::QNodeCreatedChange<PlaneObject>>(change); pointer) {
    auto obj = pointer->data;
    auto data = PlaneBackendData();

    HomoVector dir{ obj.dir.x(), obj.dir.y(), obj.dir.z(), 0 };
    HomoVector pos{ obj.pos.x(), obj.pos.y(), obj.pos.z(), 1 };
    HomoVector up{ obj.up.x(), obj.up.y(), obj.up.z(), 0 };
    data.position.setFrame(pos, dir, up);

    QVector3D vec = obj.normal;
    data.normal = HomoVector{ vec.x(), vec.y(), vec.z(), 0 };

    aspect->registerStaticObject(this->peerId(), data);
  }
}

void ObjectControllerBackend::sceneChangeEvent(
    const Qt3DCore::QSceneChangePtr& e)
{

  if (e->type() == Qt3DCore::PropertyUpdated) {
    const auto change = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(e);

    if (change->propertyName() == QByteArrayLiteral("velocity")) {
      const auto newValue = change->value().value<QVector3D>();

      if (velocity != newValue)
        velocity = newValue;

      return;
    }
  }
  QBackendNode::sceneChangeEvent(e);
}

ObjectControllerBackendMapper::ObjectControllerBackendMapper(SimulatorAspect* aspect)
    : aspect(aspect)
{
  Q_ASSERT(aspect);
}

Qt3DCore::QBackendNode* ObjectControllerBackendMapper::create(
    const Qt3DCore::QNodeCreatedChangeBasePtr& change) const
{
  auto backend = new ObjectControllerBackend(aspect);
  aspect->addObjectControllerBackend(change->subjectId(), backend);
  return backend;
}

Qt3DCore::QBackendNode* ObjectControllerBackendMapper::get(NodeId id) const
{
  return aspect->objectControllerBackend(id);
}

void ObjectControllerBackendMapper::destroy(NodeId id) const
{
  delete aspect->takeObjectControllerBackend(id);
}

} // namespace simaspect
