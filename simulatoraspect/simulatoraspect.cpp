#include "simulatoraspect.h"

namespace simaspect {

SimulatorAspect::SimulatorAspect(QObject* parent)
    : Qt3DCore::QAbstractAspect(parent)
    , simulator_worker{ SimulatorJobPtr::create(this) }
{
  auto object_mapper = QSharedPointer<ObjectControllerBackendMapper>::create(this);
  registerBackendType<ObjectController>(object_mapper);

  simulator = std::make_unique<Simulator>(this);
}

void SimulatorAspect::addObjectControllerBackend(NodeId id, ObjectControllerBackend* object_controller_backend)
{
  object_controller_backends.insert(id, object_controller_backend);
}

ObjectControllerBackend* SimulatorAspect::objectControllerBackend(NodeId id)
{
  // Value finds and returns value, not found then nullptr.
  return object_controller_backends.value(id, nullptr);
}

ObjectControllerBackend* SimulatorAspect::takeObjectControllerBackend(NodeId id)
{
  // Take takes and removes object.
  return object_controller_backends.take(id);
}

const QHash<NodeId, ObjectControllerBackend*>& SimulatorAspect::objectControllerBackends() const
{
  return object_controller_backends;
}

void SimulatorAspect::registerDynamicObject(NodeId id, SphereBackendData data)
{
  simulator->registerDynamicObject(id, data);
}

void SimulatorAspect::registerStaticObject(NodeId id, SphereBackendData data)
{
  simulator->registerStaticObject(id, data);
}

void SimulatorAspect::registerStaticObject(NodeId id, PlaneBackendData data)
{
  simulator->registerStaticObject(id, data);
}

void SimulatorAspect::simulate(seconds_type dt)
{
  simulator->simulate(dt);
}

QVector<Qt3DCore::QAspectJobPtr> SimulatorAspect::jobsToExecute(qint64 time)
{
  static constexpr auto time_resolution = 1000000000.0;

  const auto time_dt = time - last_time;
  last_time = time;
  const seconds_type dt = seconds_type(time_dt / time_resolution);

  simulator_worker->setFrameTimeDt(dt);

  return { simulator_worker };
}

} // namespace simaspect
