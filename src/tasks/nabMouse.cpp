#include "nabMouse.hpp"
#include "graphics/goose.hpp"
#include "utils/cursor.hpp"
#include "utils/math.hpp"
#include <qpoint.h>

nabMouse::nabMouse(mainloop *ml)
    : ml(ml), gooe(dynamic_cast<goose *>(ml->getGraphic("goose"))),
      stageStartTime(ml->getCurrentTime()), stage(0) {}

bool nabMouse::tick(float currentTime) {
  if (stage == 0) { // charge towards mouse
    gooe->setSpeed(goose::charging);
    QPointF target =
        cursor::getPos() + gooe->getPosition() - gooe->getBeakPos();
    gooe->setTarget(target);

    if (norm(target - gooe->getPosition()) < 15) {
      this->target = getTarget();
      stage = 1;
      stageStartTime = currentTime;
    }

    return currentTime > stageStartTime + 9;
  } else if (stage == 1) {
    gooe->setTarget(target);
    gooe->setSpeed(goose::charging);
    cursor::setPos(gooe->getBeakPos());
    return norm(gooe->getPosition() - gooe->getTarget()) < 30;
  }
  return true;
}

QPointF nabMouse::getTarget() {
  int retries = 20;
  QPointF pos = gooe->getPosition(), ret;
  while (norm((ret = {random(ml->width()), random(ml->height())}) - pos) /
                 gooe->getTopSpeed() <
             1.2 &&
         retries--)
    ;
  return ret;
}
