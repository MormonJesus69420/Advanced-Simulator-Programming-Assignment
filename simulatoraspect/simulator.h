#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "simulatoraspecttypes.h"
#include "simulatoraspect.h"
#include "structs.h"

#include <Qt3DCore>

namespace simaspect {

class Simulator {
  public:
  // Calculated local gravity in Narvik. Tool used:
  // https://www.sensorsone.com/local-gravity-calculator/
  const HomoVector gravity_acc{ 0, 0, -9.8251, 0 };

  Simulator(SimulatorAspect* aspect);

  void registerDynamicObject(NodeId node_id, SphereBackendData data);
  void registerStaticObject(NodeId node_id, SphereBackendData data);
  void registerDynamicObject(NodeId node_id, PlaneBackendData data);
  void registerStaticObject(NodeId node_id, PlaneBackendData data);

  void calculateDs(SphereBackendData& data, const seconds_type dt);
  void initDynamicPhysObject(BackendData& data);
  void simulate(seconds_type dt);

  private:
  SimulatorAspect* aspect;

  std::map<NodeId, SphereBackendData> dynamic_spheres;
  std::map<NodeId, SphereBackendData> static_spheres;
  std::map<NodeId, PlaneBackendData> dynamic_planes;
  std::map<NodeId, PlaneBackendData> static_planes;
};

} // namespace simaspect

#endif // SIMULATOR_H
