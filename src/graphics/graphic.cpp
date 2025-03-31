#include "graphic.hpp"

graphic::graphic(int zIndex, float creationTime, int rnd)
    : zIndex(zIndex), creationTime(creationTime), rnd(rnd) {}

bool graphic::operator<(const graphic &other) {
  if (zIndex != other.zIndex)
    return zIndex < other.zIndex;
  if (creationTime != other.creationTime)
    return creationTime < other.creationTime;
  return rnd < other.rnd;
}
