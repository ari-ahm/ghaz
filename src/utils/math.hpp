#ifndef MATH_HPP
#define MATH_HPP

#include <qpoint.h>
float randomBetween(float a, float b);
float random(float b = 1);
float lerp(float start, float end, float t);
QPointF lerp(QPointF start, QPointF end, float t);
float norm(QPointF x);
QPointF normalized(QPointF x);
QPointF normal(float rad);
float angle(QPointF p);
float cubicEaseInOut(float p);

#endif
