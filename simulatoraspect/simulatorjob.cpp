#include "simulatorjob.h"
#include "simulatoraspect.h"

// qt
#include <QDebug>

namespace simaspect {

SimulatorJob::SimulatorJob(SimulatorAspect* aspect)
    : aspect{ aspect }
{
}

void SimulatorJob::setFrameTimeDt(seconds_type dt) { this->dt = dt; }

void SimulatorJob::run()
{
  auto object_controllers = aspect->objectControllerBackends();

  for (const auto controller : object_controllers)
    if (controller->isEnabled())
      controller->checkCollisions(dt);

  aspect->simulate(dt);
}

} // namespace simaspect
