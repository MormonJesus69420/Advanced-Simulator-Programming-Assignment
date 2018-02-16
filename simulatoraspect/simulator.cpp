#include "simulator.h"
#include "collision_algorithms.h"
#include "objectcontrollerbackend.h"
#include "queue"
#include "simulatoraspecttypes.h"

namespace simaspect {
using namespace simaspect::algorithms;

Simulator::Simulator(SimulatorAspect* aspect)
{
  this->aspect = aspect;
}

void Simulator::registerDynamicObject(NodeId id, SphereBackendData data)
{
  dynamic_spheres.emplace(id, data);
}

void Simulator::registerStaticObject(NodeId id, SphereBackendData data)
{
  static_spheres.emplace(id, data);
}

void Simulator::registerDynamicObject(NodeId id, PlaneBackendData data)
{
  dynamic_planes.emplace(id, data);
}

void Simulator::registerStaticObject(NodeId id, PlaneBackendData data)
{
  static_planes.emplace(id, data);
}

void Simulator::simulate(seconds_type dt)
{
  for (auto& spherePair : dynamic_spheres) {
    initDynamicPhysObject(spherePair.second);
    calculateDs(spherePair.second, dt);
  }

  std::deque<CollisionObject> collisions;

  for (auto & [ sphere_id, sphere_data ] : dynamic_spheres) {
    for (const auto & [ plane_id, plane_data ] : static_planes) {
      auto collision = detectCollisionDSphereVsSPlane(sphere_data.position.position(), sphere_data.radius, sphere_data.curr_ds, plane_data.position.position(), plane_data.normal, dt);

      if (collision.type == CollisionType::collision && collision.x > 0 && collision.x <= 1) {
        collision.id1 = sphere_id;
        collision.id2 = plane_id;
        collisions.push_back(collision);
      }
    }
  }

  std::stable_sort(collisions.begin(), collisions.end());

  while (!collisions.empty()) {
    auto collision = collisions.front();
    collisions.pop_front();

    auto& col_obj_1 = dynamic_spheres.at(collision.id1);
    auto& col_obj_2 = static_planes.at(collision.id2);

    col_obj_1.t_in_dt = collision.x * dt;

    auto transvector = col_obj_1.curr_ds * ((dt - col_obj_1.t_in_dt) / dt);
    col_obj_1.position.translate(HomoVectorToVector3(transvector));

    col_obj_1.velocity = collisionVectorSphereVsPlane(col_obj_1.velocity, col_obj_2.normal);
    calculateDs(col_obj_1, dt - col_obj_1.t_in_dt);

    auto& data = col_obj_1;
    for (auto & [ plane_id, plane_data ] : static_planes) {
      if (plane_id == collision.id2)
        continue;
      auto new_collision = detectCollisionDSphereVsSPlane(data.position.position(), data.radius, data.curr_ds, plane_data.position.position(), plane_data.normal, dt);
      if (new_collision.type == CollisionType::collision && new_collision.x > 0 && new_collision.x <= 1) {
        new_collision.id1 = collision.id1;
        new_collision.time = new_collision.time + col_obj_1.t_in_dt;
        new_collision.x = (dt - new_collision.time)/dt;
        collisions.push_back(new_collision);
      }
    }
  }
  for (auto & [ sphere_id, sphere_data ] : dynamic_spheres) {
    sphere_data.position.translate(HomoVectorToVector3(sphere_data.curr_ds));
    sphere_data.velocity = sphere_data.velocity + sphere_data.curr_acc * dt.count();
    aspect->objectControllerBackend(sphere_id)->updatePositions(sphere_data.position);
  }
}

void Simulator::calculateDs(SphereBackendData& data, const seconds_type dt)
{
  data.curr_acc = gravity_acc;
  data.curr_ds = (data.velocity + 0.5 * data.curr_acc * dt.count()) * dt.count();
}

void Simulator::initDynamicPhysObject(BackendData& data)
{
  data.t_in_dt = seconds_type(0);
}
} // namespace simaspect
