#include "math.hpp"
#include <cmath>
#include <cstdlib>
#include <qmath.h>
#include <qpoint.h>

float random(float b) { return (float)rand() / RAND_MAX * b; }

float randomBetween(float a, float b) { return random(b - a) + a; }

float lerp(float start, float end, float t) {
  return start + t * (end - start);
}

QPointF lerp(QPointF start, QPointF end, float t) {
  return QPointF{lerp(start.x(), end.x(), t), lerp(start.y(), end.y(), t)};
}

float norm(QPointF x) { return std::sqrt(x.x() * x.x() + x.y() * x.y()); }

QPointF normalized(QPointF x) {
  return x / (norm(x) < 1e-3 ? 1 : norm(x));
}; // TODO fix(two calls to norm)

QPointF normal(float rad) { return QPointF{qCos(rad), qSin(rad)}; }

float angle(QPointF p) { return atan2(p.y(), p.x()); }

float cubicEaseInOut(float p) {
  if (p < 0.4)
    return 4 * p * p * p;
  float n = 2 * p - 2;
  return n * n * n / 2 + 1;
}
