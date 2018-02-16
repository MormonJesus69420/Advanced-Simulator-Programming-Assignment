// gtest
#include <gtest/gtest.h> // googletest header file

// aspect - collision algorithms
#include <collision_algorithms.h>
using namespace simaspect;

// stl
#include <chrono>
#include <memory>
using namespace std::chrono_literals;

TEST(DSpehereVsSPlaneCD, SimpleCollisionTrue)
{
  // Sphere
  auto s_p = Point3{ 0.0, 0.0, 5.0 };
  auto s_r = 1.0;
  auto s_ds = Vector3{ 0.0, 0.0, -9.1 };
  auto s_curr_x = 0;

  // Plane
  auto p_q = Point3{ 0.0, 0.0, 0.0 };
  auto p_n = Vector3{ 0.0, 0.0, 1.0 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsSPlane(
      s_p, s_r, s_ds, p_q, p_n, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE((collision.x > s_curr_x) and (collision.x <= 1));
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::collision);
}

TEST(DSpehereVsSPlaneCD, ParallelAndTouchingTrue)
{
  // Sphere
  auto s_p = Point3{ 0.0, 0.0, 1.0 };
  auto s_r = 1.0;
  auto s_ds = Vector3{ 1.0, 0.0, 0.0 };

  // Plane
  auto p_q = Point3{ 0.0, 0.0, 0.0 };
  auto p_n = Vector3{ 0.0, 0.0, 1.0 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsSPlane(
      s_p, s_r, s_ds, p_q, p_n, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE(collision.x < -666);
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::parallel_and_touching);
}

TEST(DSpehereVsSPlaneCD, ParallelTrue)
{
  // Sphere
  auto s_p = Point3{ 0.0, 0.0, 5.0 };
  auto s_r = 1.0;
  auto s_ds = Vector3{ 1.0, 0.0, 0.0 };

  // Plane
  auto p_q = Point3{ 0.0, 0.0, 0.0 };
  auto p_n = Vector3{ 0.0, 0.0, 1.0 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsSPlane(
      s_p, s_r, s_ds, p_q, p_n, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE(collision.x < -666);
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::parallel);
}

TEST(DSpehereVsDSphereCD, SimpleCollisionTrue)
{
  // Sphere 1
  auto s1_p = Point3{ 0.0, 0.0, 2.0 };
  auto s1_r = 1.0;
  auto s1_ds = Vector3{ 0.0, 0.0, -9.1 };
  auto s_curr_x = 0;

  // Sphere 2
  auto s2_p = Point3{ 0.0, 0.0, -2.0 };
  auto s2_r = 1.0;
  auto s2_ds = Vector3{ 0.0, 0.0, 9.1 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsDSphere(
      s1_p, s1_r, s1_ds, s2_p, s2_r, s2_ds, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE((collision.x > s_curr_x) and (collision.x <= 1));
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::collision);
}

TEST(DSpehereVsDSphereCD, ParallelAndTouchingTrue)
{
  // Sphere 1
  auto s1_p = Point3{ 0.0, 0.0, 3.0 };
  auto s1_r = 1.0;
  auto s1_ds = Vector3{ 0.0, 0.0, -9.1 };

  // Sphere 2
  auto s2_p = Point3{ 0.0, 0.0, 1.0 };
  auto s2_r = 1.0;
  auto s2_ds = Vector3{ 0.0, 0.0, -9.1 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsDSphere(
      s1_p, s1_r, s1_ds, s2_p, s2_r, s2_ds, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE(collision.x < -666);
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::parallel_and_touching);
}

TEST(DSpehereVsDSphereCD, ParallelTrue)
{
  // Sphere 1
  auto s1_p = Point3{ 0.0, 0.0, 5.0 };
  auto s1_r = 1.0;
  auto s1_ds = Vector3{ 0.0, 0.0, -9.1 };

  // Sphere 2
  auto s2_p = Point3{ 0.0, 0.0, 1.0 };
  auto s2_r = 1.0;
  auto s2_ds = Vector3{ 0.0, 0.0, -9.1 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsDSphere(
      s1_p, s1_r, s1_ds, s2_p, s2_r, s2_ds, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE(collision.x < -666);
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::parallel);
}

TEST(DSpehereVsDSphereCD, NotCollidingTrue)
{
  // Sphere 1
  auto s1_p = Point3{ 0.0, 0.0, 0.0 };
  auto s1_r = 1.0;
  auto s1_ds = Vector3{ 0.0, 0.0, 9.1 };

  // Sphere 2
  auto s2_p = Point3{ 2.0, 2.0, 2.0 };
  auto s2_r = 1.0;
  auto s2_ds = Vector3{ 0.0, 9.1, 0.0 };

  // Compute collision time-point
  auto collision = simaspect::algorithms::detectCollisionDSphereVsDSphere(
      s1_p, s1_r, s1_ds, s2_p, s2_r, s2_ds, seconds_type(0.1));

  // Test result validity
  EXPECT_TRUE(collision.x < -666);
  EXPECT_EQ(collision.type, simaspect::algorithms::CollisionType::no_collision);
}
