#include "runOffscreen.hpp"
#include "goose/mainloop.hpp"
#include "utils/math.hpp"
#include <qpoint.h>

runOffscreen::runOffscreen(mainloop *ml)
    : ml(ml), gooe(dynamic_cast<goose *>(ml->getGraphic("goose"))) {
  target = getTarget();
}

bool runOffscreen::tick(float currentTime) {
  gooe->setTarget(target);
  gooe->setSpeed(goose::running);
  return (norm(gooe->getPosition() - target) < 20);
}

QPointF runOffscreen::getTarget() {
  return {random(100) > 50 ? -50. : ml->width() + 50., random(ml->height())};
}
