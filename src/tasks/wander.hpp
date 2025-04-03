#ifndef WANDER_HPP
#define WANDER_HPP

#include "goose/mainloop.hpp"
#include "graphics/goose.hpp"
#include "tasks/task.hpp"
#include <qpoint.h>

class wander : public task {
public:
  wander(mainloop *ml);
  bool tick(float currentTime) override;

private:
  QPointF getBudgetTarget(float currentTime);
  float endTime, pauseEnd;
  goose *gooe;
  mainloop *ml;
};

#endif
