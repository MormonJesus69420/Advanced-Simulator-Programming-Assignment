#include "collision_algorithms.h"

namespace simaspect::algorithms {

CollisionObject::CollisionObject(CollisionType type, double x, seconds_type time, CollidingWith colliding)
{
  this->colliding = colliding;
  this->time = time * x;
  this->type = type;
  this->x = x;
}

bool CollisionObject::operator==(const CollisionObject& other) const
{
  return id1 == other.id1 && id2 == other.id2 && time == other.time;
}

bool CollisionObject::operator<(const CollisionObject& other) const
{
  return x < other.x;
}

CollisionObject detectCollisionDSphereVsSPlane(const HomoVector& p, double r,
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

CollisionObject detectCollisionDSphereVsDSphere(const HomoVector& p0, double r0,
    const HomoVector& ds0, const HomoVector& p1, double r1, const HomoVector& ds1, seconds_type dt)
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

  return CollisionObject(CollisionType::collision, (-QR - blaze::sqrt(radicant)) / RR, dt, CollidingWith::sphere);
}

HomoVector collisionVectorSphereVsPlane(HomoVector velocity, HomoVector n)
{
  HomoVector result = 2 * blaze::inner(velocity, n) * n;

  return velocity - result;
}

std::pair<HomoVector, HomoVector> collisionVectorsSphereVsSphere(HomoVector p1, HomoVector v1, HomoVector p2, HomoVector v2)
{
  auto m1 = 1, m2 = 1;

  HomoVector d = blaze::normalize(p2 - p1);
  Vector3 strongIndependentVictor = gmlib2::algorithms::linearIndependentVector(HomoVectorToVector3(d));
  HomoVector n = blaze::normalize(Vector3ToHomoVector(blaze::cross(blaze::subvector(d, 0UL, 3UL), strongIndependentVictor)));

  double v1d = blaze::inner(v1, d);
  double v2d = blaze::inner(v2, d);
  double v1n = blaze::inner(v1, n);
  double v2n = blaze::inner(v2, n);

  double v1dmark = ((m1-m2)/(m1+m2))*v1d + ((2*m2)/(m1+m2))*v2d;
  double v2dmark = ((m2-m1)/(m1+m2))*v2d + ((2*m1)/(m1+m2))*v1d;

  HomoVector v1new = v1n * n + v1dmark * d;
  HomoVector v2new = v2n * n + v2dmark * d;
  return std::pair<HomoVector, HomoVector>(v1new, v2new);
}

HomoVector Vector3ToHomoVector(Vector3 vector)
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
