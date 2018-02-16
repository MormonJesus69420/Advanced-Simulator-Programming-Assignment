#ifndef SIMULATORJOB_H
#define SIMULATORJOB_H

#include "simulatoraspecttypes.h"

// qt
#include <QSharedPointer>
#include <Qt3DCore/QAspectJob>

namespace simaspect {

class SimulatorAspect;

class SimulatorJob : public Qt3DCore::QAspectJob {
  public:
  SimulatorJob(SimulatorAspect* aspect);

  void setFrameTimeDt(seconds_type dt);

  private:
  SimulatorAspect* aspect;
  seconds_type dt;

  // QAspectJob interface
  public:
  void run() override;
};

using SimulatorJobPtr = QSharedPointer<SimulatorJob>;

} // namespace simaspect

#endif // SIMULATORJOB_H
