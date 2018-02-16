#ifndef COLLISION_ALGORIHMS_H
#define COLLISION_ALGORIHMS_H

#include "simulatoraspecttypes.h"

namespace simaspect::algorithms {

enum class CollisionType {
  parallel_and_touching,
  no_collision,
  collision,
  parallel
};

static const double NOTHING = std::pow(10, -5);

// Based on recommendation by Chris Hagerup, gives more than a simple touple.
struct CollisionObject {
  Qt3DCore::QNodeId id1;
  Qt3DCore::QNodeId id2;
  CollisionType type;
  seconds_type time;
  double x;

  public:
  CollisionObject(CollisionType type, double x = -666.69, seconds_type time = seconds_type(0))
  {
    this->time = time * x;
    this->type = type;
    this->x = x;
  }

  bool operator<(const CollisionObject& other) const
  {
    return x < other.x;
  }
};

auto detectCollisionDSphereVsSPlane(const HomoVector& p, double r,
    const HomoVector ds, const HomoVector q, const HomoVector& n, seconds_type dt)
{
  // Calculate d directly to avvoid creating new variables.
  auto Q = blaze::inner((q + r * n) - p, n);

  // Sphere || plane and touching
  if (Q < NOTHING and Q > -NOTHING)
    return CollisionObject(CollisionType::parallel_and_touching);

  auto R = blaze::inner(ds, n);

  // Sphere || plane
  if (R < NOTHING and R > -NOTHING)
    return CollisionObject(CollisionType::parallel);

  return CollisionObject(CollisionType::collision, Q / R, dt);
}

auto detectCollisionDSphereVsDSphere(const Point3& p0, double r0,
    const Vector3& ds0, const Point3& p1, double r1, const Vector3& ds1, seconds_type dt)
{
  // Calculate constants for inner products
  auto d = r0 + r1;
  auto q = p1 - p0;
  auto r = ds1 - ds0;

  auto QQ = blaze::inner(q, q);
  auto QQ_minus_r = QQ - blaze::pow(d, 2);

  // Sphere || sphere and touching
  if (QQ_minus_r < NOTHING)
    return CollisionObject(CollisionType::parallel_and_touching);

  auto RR = blaze::inner(r, r);

  // Sphere || sphere
  if (RR < NOTHING)
    return CollisionObject(CollisionType::parallel);

  auto QR = blaze::inner(q, r);
  auto radicant = blaze::pow(QR, 2) - RR * QQ_minus_r;

  // No collision
  if (radicant < 0)
    return CollisionObject(CollisionType::no_collision);

  return CollisionObject(CollisionType::collision, (-QR - blaze::sqrt(radicant)) / RR, dt);
}

HomoVector collisionVectorSphereVsPlane(HomoVector velocity, HomoVector n)
{
  HomoVector result = 2 * blaze::inner(velocity, n) * n;
  return velocity - result;
}

HomoVector Vector3ToHVector(Vector3 vector)
{
  return HomoVector{ vector[0], vector[1], vector[2], 0 };
}

Point3 HomoVectorToPoint(HomoVector vector)
{
  return Point3{ vector[0], vector[1], vector[2] };
}

Vector3 HomoVectorToVector3(HomoVector vector)
{
  return Vector3{ vector[0], vector[1], vector[2] };
}
} // namespace simaspect::algorithms

#endif // COLLISION_ALGORIHMS_H
