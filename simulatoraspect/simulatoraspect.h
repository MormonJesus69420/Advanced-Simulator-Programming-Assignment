#ifndef SIMULATORASPECT_H
#define SIMULATORASPECT_H

#include "objectcontrollerbackend.h"
#include "objectcontroller.h"
#include "simulatorjob.h"
#include "simulator.h"

// qt
#include <Qt3DCore>

namespace simaspect {

class ObjectControllerBackend;
class Simulator;

class SimulatorAspect : public Qt3DCore::QAbstractAspect {
  Q_OBJECT

  public:
  explicit SimulatorAspect(QObject* parent = nullptr);

  void addObjectControllerBackend(NodeId id, ObjectControllerBackend* object_controller_backend);
  ObjectControllerBackend* takeObjectControllerBackend(NodeId id);
  ObjectControllerBackend* objectControllerBackend(NodeId id);

  const QHash<NodeId, ObjectControllerBackend*>& objectControllerBackends() const;

  void registerDynamicObject(NodeId id, SphereBackendData data);
  void registerStaticObject(NodeId id, SphereBackendData data);
  void registerStaticObject(NodeId id, PlaneBackendData data);
  void registerObj(PhysObject data, NodeId id);

  void simulate(seconds_type dt);

  private:
  QHash<NodeId, ObjectControllerBackend*> object_controller_backends;
  QVector<Qt3DCore::QAspectJobPtr> jobsToExecute(qint64 time) override;
  std::unique_ptr<Simulator> simulator;
  SimulatorJobPtr simulator_worker;
  qint64 last_time{ 0 };
};

} // namespace simaspect

#endif // SIMULATORASPECT_H
