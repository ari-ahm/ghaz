#include "mudTrack.hpp"
#include "goose/mainloop.hpp"
#include "tasks/runOffscreen.hpp"

mudTrack::mudTrack(mainloop *ml)
    : ml(ml), gooe(dynamic_cast<goose *>(ml->getGraphic("goose"))) {}

bool mudTrack::tick(float currentTime) {
  if (!init) {
    ml->setTask(new runOffscreen(ml));
    init = true;
    return false;
  }
  gooe->setMuddyFeet(currentTime);
  return true;
}
