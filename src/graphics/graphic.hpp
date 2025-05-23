#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <QPainter>

class graphic {
public:
  graphic(int zIndex = 0, float creationTime = 0, int rnd = rand());
  virtual bool draw(QPainter *painter, float currentTime) = 0;
  virtual bool operator<(const graphic &other) const final;
  static bool pointerCmp(const graphic *lhs, const graphic *rhs);

private:
  int zIndex;
  float creationTime;
  int rnd;
};

#endif
