#ifndef NABMOUSE_HPP
#define NABMOUSE_HPP

#include "goose/mainloop.hpp"
#include "graphics/goose.hpp"
#include "tasks/task.hpp"
#include <qpoint.h>

class nabMouse : public task {
public:
  nabMouse(mainloop *app);
  bool tick(float currentTime) override;

private:
  mainloop *ml;
  goose *gooe;
  int stage = 0;
  float stageStartTime;
  QPointF target;

  QPointF getTarget();
};

#endif
