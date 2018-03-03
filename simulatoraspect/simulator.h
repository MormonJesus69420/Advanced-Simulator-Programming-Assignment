#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "collision_algorithms.h"
#include "simulatoraspecttypes.h"
#include "simulatoraspect.h"
#include "structs.h"

#include <Qt3DCore>

namespace simaspect {

using CollisionObject = algorithms::CollisionObject;

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

  std::vector<CollisionObject> collisions;

  std::map<NodeId, SphereBackendData> dynamic_spheres;
  std::map<NodeId, SphereBackendData> static_spheres;
  std::map<NodeId, PlaneBackendData> dynamic_planes;
  std::map<NodeId, PlaneBackendData> static_planes;

  void simulateUpToCollision(SphereBackendData& sphere, const CollisionObject& collision, const seconds_type dt);
  void newCollisionCheck(SphereBackendData& sphere, CollisionObject& collision, const seconds_type dt);
  void impactResponse(SphereBackendData& sphere1, SphereBackendData& sphere2, const seconds_type dt);
  void impactResponse(SphereBackendData& sphere, PlaneBackendData& plane, const seconds_type dt);
  void collisionCheck(seconds_type dt);
  void sortAndMakeUnique();
};

} // namespace simaspect

#endif // SIMULATOR_H
