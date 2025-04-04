#ifndef RUNOFFSCREEN_HPP
#define RUNOFFSCREEN_HPP

#include "goose/mainloop.hpp"
#include "graphics/goose.hpp"
#include "tasks/task.hpp"
#include <qpoint.h>

class runOffscreen : public task {
public:
  runOffscreen(mainloop *ml);
  bool tick(float currentTime) override;

private:
  mainloop *ml;
  goose *gooe;
  QPointF target;

  QPointF getTarget();
};

#endif
