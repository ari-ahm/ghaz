#ifndef MUDTRACK_HPP
#define MUDTRACK_HPP

#include "goose/mainloop.hpp"
#include "graphics/goose.hpp"
#include "tasks/task.hpp"
#include <qwidget.h>

class mudTrack : public task {
public:
  mudTrack(mainloop *ml);
  bool tick(float currentTime) override;

private:
  mainloop *ml;
  goose *gooe;
  QWidget *window;
  bool init = false;
};

#endif
