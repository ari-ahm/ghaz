#ifndef RUNMOUSE_HPP
#define RUNMOUSE_HPP

#include "goose/mainloop.hpp"
#include "graphics/goose.hpp"
#include "tasks/task.hpp"

class runMouse : public task {
public:
  runMouse(mainloop *app);
  bool tick(float currentTime) override;

private:
  goose *gooe;
};

#endif
