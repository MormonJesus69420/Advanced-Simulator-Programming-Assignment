#ifndef COLLISION_ALGORIHMS_H
#define COLLISION_ALGORIHMS_H

#include "simulatoraspecttypes.h"

//
// Needed to move methods to cpp file, error most probably caused by linker? See LinkerError.png in project.
//

namespace simaspect::algorithms {

enum class CollisionType {
  parallel_and_touching,
  no_collision,
  collision,
  parallel
};

enum class CollidingWith {
  sphere,
  plane
};

static const double NOTHING = std::pow(10, -5);

// Based on recommendation by Chris Hagerup, gives more than a simple touple.
struct CollisionObject {
  CollidingWith colliding;
  CollisionType type;
  seconds_type time;
  NodeId id1;
  NodeId id2;
  double x;

  public:
  CollisionObject(CollisionType type, double x = -666.69, seconds_type time = seconds_type(0),
      CollidingWith colliding = CollidingWith::plane);

  bool operator==(const CollisionObject& other) const;
  bool operator<(const CollisionObject& other) const;
};

CollisionObject detectCollisionDSphereVsSPlane(const HomoVector& p, double r,
    const HomoVector ds, const HomoVector q, const HomoVector& n, seconds_type dt);

CollisionObject detectCollisionDSphereVsDSphere(const HomoVector& p0, double r0,
    const HomoVector& ds0, const HomoVector& p1, double r1, const HomoVector& ds1, seconds_type dt);

HomoVector collisionVectorSphereVsPlane(HomoVector velocity, HomoVector n);
std::pair<HomoVector, HomoVector> collisionVectorsSphereVsSphere(HomoVector p1, HomoVector v1, HomoVector p2, HomoVector v2);

Vector3 HomoVectorToVector3(HomoVector vector);
Point3 HomoVectorToPoint(HomoVector vector);
HomoVector Vector3ToHomoVector(Vector3 vector);

} // namespace simaspect::algorithms

#endif // COLLISION_ALGORIHMS_H
