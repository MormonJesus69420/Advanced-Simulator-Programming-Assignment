#include "simulator.h"
#include "collision_algorithms.h"
#include "objectcontrollerbackend.h"
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
  // Init spheres
  for (auto& spherePair : dynamic_spheres) {
    initDynamicPhysObject(spherePair.second);
    calculateDs(spherePair.second, dt);
  }

  collisionCheck(dt);
  sortAndMakeUnique();

  while (!collisions.empty()) {
    auto collision = collisions.back();
    collisions.pop_back();

    if (collision.colliding == CollidingWith::plane) {
      auto& collidingSphere = dynamic_spheres.at(collision.id1);
      auto& collidingPlane = static_planes.at(collision.id2);

      simulateUpToCollision(collidingSphere, collision, dt);

      impactResponse(collidingSphere, collidingPlane, dt);

      newCollisionCheck(collidingSphere, collision, dt);
    }

    else if (collision.colliding == CollidingWith::sphere) {
      auto& sphere1 = dynamic_spheres.at(collision.id1);
      auto& sphere2 = dynamic_spheres.at(collision.id2);

      simulateUpToCollision(sphere1, collision, dt);
      simulateUpToCollision(sphere2, collision, dt);

      impactResponse(sphere1, sphere2, dt);

      newCollisionCheck(sphere1, collision, dt);
      newCollisionCheck(sphere2, collision, dt);
    }
  }

  for (auto & [ sphere_id, sphere_data ] : dynamic_spheres) {
    sphere_data.position.translate(HomoVectorToVector3(sphere_data.curr_ds));
    sphere_data.velocity = sphere_data.velocity + sphere_data.curr_acc * (dt - sphere_data.t_in_dt).count();
    aspect->objectControllerBackend(sphere_id)->updatePositions(sphere_data.position);
  }
}

void Simulator::collisionCheck(const seconds_type dt)
{
  for (auto pair = dynamic_spheres.begin(); pair != dynamic_spheres.end();) {
    auto& s_id = pair->first;
    auto& s_data = pair->second;

    for (const auto & [ p_id, p_data ] : static_planes) {
      auto collision = detectCollisionDSphereVsSPlane(s_data.position.position(), s_data.radius, s_data.curr_ds, p_data.position.position(), p_data.normal, dt);

      if (collision.type == CollisionType::collision && collision.x > 0 && collision.x <= 1) {
        collision.id1 = s_id;
        collision.id2 = p_id;

        qDebug() << "Created PvB collision for:" << collision.id1 << collision.id2;

        collisions.push_back(collision);
      }
    }

    for (auto pair2 = ++pair; pair2 != dynamic_spheres.end(); ++pair2) {
      const auto& s2_id = pair2->first;
      const auto& s2_data = pair2->second;

      auto collision = detectCollisionDSphereVsDSphere(s_data.position.position(), s_data.radius, s_data.curr_ds, s2_data.position.position(), s2_data.radius, s2_data.curr_ds, dt);

      if (collision.type == CollisionType::collision && collision.x > 0 && collision.x <= 1) {
        collision.id1 = s_id;
        collision.id2 = s2_id;

        qDebug() << "Created BvB collision for:" << collision.id1 << collision.id2;

        collisions.push_back(collision);
      }
    }
  }
}

void Simulator::sortAndMakeUnique()
{
  // Stable sort with comparator as input, order of a and b changed or else the result is wrong.
  // Stable sort preserves relative roder of equivalent values.
  std::stable_sort(collisions.begin(), collisions.end(), [](const auto& a, const auto& b) { return b.time < a.time; });

  std::vector<CollisionObject> duplicates;

  for (auto iterator = collisions.rbegin(); iterator != collisions.rend(); ++iterator) {
    auto next = iterator;
    ++next;

    // Check if not end, check same id, check same x
    if (next != collisions.rend() && next->id1 == iterator->id1 && std::abs(next->x - iterator->x) <= std::exp(-6)) {

      qDebug() << "Duplicate" << next->id1 << next->id2 << iterator->id1 << iterator->id2;
      duplicates.push_back(*next);
    }
  }

  // Delete elements from collisions
  // Remove if uses predicate, uses collisions == operator overload.
  collisions.erase(std::remove_if(collisions.begin(), collisions.end(), [&duplicates](const auto& a) {
    for (const auto& collision : duplicates)
      if (collision == a)
        return true;
    return false;
  }),
      collisions.end());
}

void Simulator::simulateUpToCollision(SphereBackendData& sphere, const CollisionObject& collision, const seconds_type dt)
{
  auto teim = sphere.t_in_dt;
  sphere.t_in_dt = collision.x * dt;

  auto transvector = sphere.curr_ds * ((dt - sphere.t_in_dt) / dt);
  sphere.position.translate(HomoVectorToVector3(transvector));

  // Take into consideration timestep, or else lose energy.
  sphere.velocity += sphere.curr_acc * (sphere.t_in_dt - teim).count();
}

void Simulator::impactResponse(SphereBackendData& sphere, PlaneBackendData& plane, const seconds_type dt)
{
  sphere.velocity = collisionVectorSphereVsPlane(sphere.velocity, plane.normal);
  calculateDs(sphere, dt - sphere.t_in_dt);
}

void Simulator::impactResponse(SphereBackendData& sphere1, SphereBackendData& sphere2, const seconds_type dt)
{
  auto velocities = collisionVectorsSphereVsSphere(sphere1.position.position(), sphere1.velocity, sphere2.position.position(), sphere2.velocity);
  sphere1.velocity = velocities.first;
  sphere2.velocity = velocities.second;

  calculateDs(sphere1, dt - sphere1.t_in_dt);
  calculateDs(sphere2, dt - sphere2.t_in_dt);
}

void Simulator::newCollisionCheck(SphereBackendData& sphere, CollisionObject& collision, const seconds_type dt)
{
  collisions.erase(std::remove_if(collisions.begin(), collisions.end(), [&collision](const auto& a) {
    return collision == a;
  }),
      collisions.end());

  for (const auto & [ p_id, p_data ] : static_planes) {
    if (p_id == collision.id2 && collision.colliding == CollidingWith::plane)
      continue;

    auto new_collision = detectCollisionDSphereVsSPlane(sphere.position.position(), sphere.radius, sphere.curr_ds, p_data.position.position(), p_data.normal, dt - sphere.t_in_dt);

    if (new_collision.type == CollisionType::collision && new_collision.x > 0 && new_collision.x <= 1) {
      new_collision.id1 = collision.id1;
      new_collision.id2 = p_id;
      new_collision.time = new_collision.time + sphere.t_in_dt;
      new_collision.x = (dt - new_collision.time) / dt;

      qDebug() << "Created new PvB collision for:" << new_collision.id1 << new_collision.id2;
      collisions.push_back(new_collision);
    }
  }

  for (auto j = dynamic_spheres.begin(); j != dynamic_spheres.end(); ++j) {
    const auto& s2_id = j->first;
    const auto& s2_data = j->second;

    if (s2_id == collision.id1 || (s2_id == collision.id2 && collision.colliding == CollidingWith::sphere))
      continue;

    auto new_collision = detectCollisionDSphereVsDSphere(sphere.position.position(), sphere.radius, sphere.curr_ds, s2_data.position.position(), s2_data.radius, s2_data.curr_ds, dt - sphere.t_in_dt);

    if (new_collision.type == CollisionType::collision && new_collision.x > 0 && new_collision.x <= 1) {
      new_collision.id1 = collision.id1;
      new_collision.id2 = s2_id;
      new_collision.time = new_collision.time + sphere.t_in_dt;
      new_collision.x = (dt - new_collision.time) / dt;

      qDebug() << "Created new BvB collision for:" << new_collision.id1 << new_collision.id2;
      collisions.push_back(new_collision);
    }
  }

  sortAndMakeUnique();
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
