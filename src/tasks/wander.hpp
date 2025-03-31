#ifndef WANDER_HPP
#define WANDER_HPP

#include "goose/mainloop.hpp"
#include "graphics/goose.hpp"
#include "tasks/task.hpp"

class wander : public task {
public:
  wander(mainloop *app);
  wander(mainloop *app, goose *gooe);
  task &tick(float currentTime) override;

private:
  goose *gooe;
};

#endif
