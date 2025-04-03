#include "runMouse.hpp"
#include "graphics/goose.hpp"
#include "utils/cursor.hpp"
#include <qpoint.h>

runMouse::runMouse(mainloop *app)
    : gooe(dynamic_cast<goose *>(app->getGraphic("goose"))) {
  gooe->setSpeed(goose::running);
}

bool runMouse::tick(float currentTime) {
  QPointF target = cursor::getPos();
  gooe->setTarget(target);
  return false;
}
