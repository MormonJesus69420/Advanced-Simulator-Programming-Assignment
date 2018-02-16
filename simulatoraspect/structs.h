#ifndef STRUCTS_H
#define STRUCTS_H

// gmlib2
#include <gmlib2.h>

#include "simulatoraspecttypes.h"

namespace simaspect {

struct PhysObject {
  simaspect::ObjectType type;
  simaspect::ObjectKind kind;
  QVector3D pos;
  QVector3D dir;
  QVector3D up;
  double mass;

  PhysObject() {}

  PhysObject(simaspect::ObjectType type, simaspect::ObjectKind kind, Point3 position = Point3{ 1.0, 1.0, 1.0 })
  {
    pos = QVector3D(position[0], position[1], position[2]);
    this->kind = kind;
    this->type = type;
  }
};

struct PlaneObject : PhysObject {
  QVector3D normal;

  PlaneObject()
      : PhysObject()
  {
  }
};

struct SphereObject : PhysObject {
  double radius;
  QVector3D ds;

  SphereObject()
      : PhysObject()
  {
  }

  SphereObject(simaspect::ObjectType type, Point3 position = Point3{ 1.0, 1.0, 1.0 }, double radius = 1.0)
      : PhysObject(type, ObjectKind::sphere, position)
  {
    this->radius = radius;
  }
};

struct BackendData {
  AffineSpaceD3 position;
  seconds_type t_in_dt;
  HomoVector curr_acc;
  HomoVector curr_ds;
};

struct SphereBackendData : BackendData {
  double radius;
  HomoVector velocity;
};

struct PlaneBackendData : BackendData {
  HomoVector velocity;
  HomoVector normal;
};
} // namespace simaspect

#endif // STRUCTS_H
