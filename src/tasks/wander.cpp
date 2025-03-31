#include "wander.hpp"
#include "graphics/goose.hpp"
#include "utils/cursor.hpp"
#include <qpoint.h>

wander::wander(mainloop *app) : gooe(new goose()) {
  app->registerGraphics(gooe);
}

wander::wander(mainloop *app, goose *gooe) : gooe(gooe) {}

task &wander::tick(float currentTime) {
  QPointF target = cursor::getPos();
  QPointF d = target - gooe->getPosition();
  gooe->setDirection(atan2(d.y(), d.x()) * 180 / M_PI);
  gooe->setPosition(gooe->getPosition() + d * 0.01);

  return *this;
}
