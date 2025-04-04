#include "wander.hpp"
#include "graphics/goose.hpp"
#include "utils/math.hpp"
#include <qlogging.h>
#include <qpoint.h>

float getRandomWanderDuration() { return randomBetween(20, 40); }
float getRandomWalkDuration() { return randomBetween(1, 6); }
float getRandomPauseDuration() { return randomBetween(1, 2); }

QPointF wander::getBudgetTarget(float currentTime) {
  float distance = getRandomWalkDuration() * gooe->getTopSpeed();
  QPointF target{random(ml->width()), random(ml->height())};
  target -= gooe->getPosition();
  float targetLength = norm(target);
  return gooe->getPosition() + target * std::min(targetLength, distance) /
                                   (targetLength < 1e-3 ? 1 : targetLength);
}

wander::wander(mainloop *ml)
    : ml(ml), gooe(dynamic_cast<goose *>(ml->getGraphic("goose"))),
      endTime(ml->getCurrentTime() + getRandomWanderDuration()), pauseEnd(-1),
      target(getBudgetTarget(ml->getCurrentTime())) {
  gooe->setSpeed(goose::walking);
}

bool wander::tick(float currentTime) {
  gooe->setTarget(target);
  if (pauseEnd > 0) {
    gooe->setSpeed(goose::stopped);
    if (pauseEnd < currentTime) {
      gooe->setSpeed(goose::walking);
      target = getBudgetTarget(currentTime);
      pauseEnd = -1;
    }
  } else if (norm(gooe->getTarget() - gooe->getPosition()) < 20.f) {
    pauseEnd = currentTime + getRandomPauseDuration();
  }
  return currentTime > endTime;
}
