#ifndef TYPES_H
#define TYPES_H

// gmlib2
#include <gmlib2/qt/sceneobject.h>

// stl
#include <chrono>

namespace simaspect {

// time types
using seconds_type = std::chrono::duration<double>; // std::ratio<1,1> is implicit
// default and means seconds

// Qt types
using RotatePair = QPair<float, QVector3D>;
using NodeId = Qt3DCore::QNodeId;

// Affine types
using AffineSpaceD3 = gmlib2::bases::AffineSpaceD3<double>;
using HomoMatrix = AffineSpaceD3::HomogeneousMatrix_Type;
using HomoVector = AffineSpaceD3::HomogeneousVector_Type;
using Vector3 =AffineSpaceD3::Vector_Type;
using Point3 = AffineSpaceD3::Point_Type;

enum class ObjectType {
  static_obj,
  dynamic_obj
};

enum class ObjectKind {
  plane,
  sphere,
  torus,
  cylinder,
  finite_plane
};

} // namespace simaspect

Q_DECLARE_METATYPE(simaspect::ObjectType)
Q_DECLARE_METATYPE(simaspect::ObjectKind)

#endif // TYPES_H
