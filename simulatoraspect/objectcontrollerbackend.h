#ifndef OBJECTCONTROLLERBACKEND_H
#define OBJECTCONTROLLERBACKEND_H

#include "simulatoraspecttypes.h"
#include "structs.h"

// qt
#include <QMatrix4x4>
#include <Qt3DCore/QBackendNode>

namespace simaspect {
class SimulatorAspect;
void collide(const seconds_type dt);

class ObjectControllerBackend : public Qt3DCore::QBackendNode {
  public:
  ObjectControllerBackend(SimulatorAspect* aspect);
  void updatePositions(AffineSpaceD3 position);
  void checkCollisions(seconds_type dt);
  void translate(seconds_type dt);

  private:
  std::shared_ptr<PhysObject> phys_object;
  SimulatorAspect* aspect;
  QVector3D velocity;

  protected:
  void sceneChangeEvent(const Qt3DCore::QSceneChangePtr& e) override;

  private:
  void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr& change) override;
};

class ObjectControllerBackendMapper : public Qt3DCore::QBackendNodeMapper {
  public:
  explicit ObjectControllerBackendMapper(SimulatorAspect* aspect);

  private:
  SimulatorAspect* aspect;

  public:
  Qt3DCore::QBackendNode* create(const Qt3DCore::QNodeCreatedChangeBasePtr& change) const override;
  Qt3DCore::QBackendNode* get(NodeId id) const override;
  void destroy(NodeId id) const override;
};

} // namespace simaspect

#endif // OBJECTCONTROLLERBACKEND_H
