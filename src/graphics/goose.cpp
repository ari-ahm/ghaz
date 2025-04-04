#include "goose.hpp"
#include "utils/math.hpp"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QVector>
#include <algorithm>
#include <cmath>
#include <qbrush.h>
#include <qcolor.h>
#include <qeasingcurve.h>
#include <qlogging.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qvector2d.h>

inline void fillCircleFromCenter(QPainter *painter, const QBrush &brush,
                                 const QPointF &center, int radius) {
  painter->save();
  painter->setBrush(brush);
  painter->setPen(Qt::NoPen);
  painter->drawEllipse(center, radius, radius);
  painter->restore();
}

inline void fillEllipseFromCenter(QPainter *painter, const QBrush &brush,
                                  const QPointF &center, int xRadius,
                                  int yRadius) {
  painter->save();
  painter->setBrush(brush);
  painter->setPen(Qt::NoPen);

  painter->drawEllipse(QRectF(center.x() - xRadius, center.y() - yRadius,
                              xRadius * 2, yRadius * 2));
  painter->restore();
}

goose::goose() {
  QImage shadowBitmap(2, 2, QImage::Format_ARGB32);
  shadowBitmap.fill(Qt::transparent);

  shadowBitmap.setPixelColor(0, 1, QColor(Qt::darkGray));

  shadowBrush = QBrush(QPixmap::fromImage(shadowBitmap));

  shadowPen = QPen();
  shadowPen.setBrush(shadowBrush);
  shadowPen.setCapStyle(Qt::RoundCap);

  drawingPen = QPen(Qt::white);
  drawingPen.setCapStyle(Qt::RoundCap);
}

void goose::updateRig(float currentTime) {
  QPointF a(position.x(), position.y());
  QPointF b(1.3f, 0.4f);
  QPointF fromAngle = normal(direction);

  QPointF a2(0.0f, -1.0f);

  gooseRig.underbodyCenter = a + a2 * 9.0f;
  gooseRig.bodyCenter = a + a2 * 14.0f;

  int num4 = lerp(20.0f, 10.0f, gooseRig.neckLerpPercent);
  int num5 = lerp(3.0f, 16.0f, gooseRig.neckLerpPercent);

  gooseRig.neckCenter = a + a2 * (14.0f + num4);
  gooseRig.neckBase = gooseRig.bodyCenter + fromAngle * 15.0f;
  gooseRig.neckHeadPoint = gooseRig.neckBase + fromAngle * num5 + a2 * num4;
  gooseRig.head1EndPoint =
      gooseRig.neckHeadPoint + fromAngle * 3.0f - a2 * 1.0f;
  gooseRig.head2EndPoint = gooseRig.head1EndPoint + fromAngle * 5.0f;

  gooseRig.neckLerpPercent =
      lerp(gooseRig.neckLerpPercent, (extendingNeck || norm(velocity) >= 200),
           0.075);

  solveFeet(currentTime);
}

void goose::solveFeet(float currentTime) {
  while (!mudPath.empty() && mudPath.front().time + 10 < currentTime)
    mudPath.pop_front();

  QPointF lfootTarget = position + normal(direction - M_PI_2) * 3;
  QPointF rfootTarget = position + normal(direction + M_PI_2) * 3;

  if (gooseRig.lFootTime < 0 && gooseRig.rFootTime < 0) {
    float lfootDist = norm(gooseRig.lFootPos - lfootTarget);
    float rfootDist = norm(gooseRig.rFootPos - rfootTarget);

    if (lfootDist > rfootDist && lfootDist > 5) {
      gooseRig.lFootOrig = gooseRig.lFootPos;
      gooseRig.lFootDir = normalized(lfootTarget - gooseRig.lFootPos);
      gooseRig.lFootTime = currentTime;
    } else if (rfootDist > 5) {
      gooseRig.rFootOrig = gooseRig.rFootPos;
      gooseRig.rFootDir = normalized(rfootTarget - gooseRig.rFootPos);
      gooseRig.rFootTime = currentTime;
    }
  } else if (gooseRig.lFootTime >= 0) {
    QPointF diff = lfootTarget + gooseRig.lFootDir * 2;
    if (currentTime > gooseRig.lFootTime + stepTime) {
      gooseRig.lFootPos = diff;
      gooseRig.lFootTime = -1;
      if (currentTime - muddyFeetTime < 15)
        mudPath.push_back({diff, currentTime});
    } else {
      gooseRig.lFootPos =
          lerp(gooseRig.lFootOrig, diff,
               cubicEaseInOut((currentTime - gooseRig.lFootTime) / stepTime));
    }
  } else {
    QPointF diff = rfootTarget + gooseRig.rFootDir * 2;
    if (currentTime > gooseRig.rFootTime + stepTime) {
      gooseRig.rFootPos = diff;
      gooseRig.rFootTime = -1;
      if (currentTime - muddyFeetTime < 15)
        mudPath.push_back({diff, currentTime});
    } else {
      gooseRig.rFootPos =
          lerp(gooseRig.rFootOrig, diff,
               cubicEaseInOut((currentTime - gooseRig.rFootTime) / stepTime));
    }
  }
}

void goose::drawRig(QPainter *painter, float currentTime) {

  auto multiplyScalar = [](const QPointF &pt, float scalar) -> QPointF {
    return QPointF(pt.x() * scalar, pt.y() * scalar);
  };

  auto multiplyPoint = [](const QPointF &a, const QPointF &b) -> QPointF {
    return QPointF(a.x() * b.x(), a.y() * b.y());
  };

  QPointF vector2(1.3, 0.4);
  QPointF forward = normal(direction);
  QPointF right = normal(direction + M_PI_2);
  QPointF vecUp(0, -1);

  int num5 = 2;

  QBrush brushGooseWhite(Qt::white);
  QBrush brushGooseOrange(QColor(255, 165, 0));
  QColor gooseOutlineColor(QColor(211, 211, 211));
  QBrush brushMudStain(QColor(139, 69, 19));

  for (mudStain i : mudPath) {
    float p = currentTime - i.time - 9;
    p = std::max(0.f, p);
    p = std::min(1.f, p);
    fillCircleFromCenter(painter, brushMudStain, i.position, lerp(3, 0, p));
  }

  QPen pen = drawingPen;
  pen.setColor(brushGooseWhite.color());
  painter->setPen(pen);

  fillCircleFromCenter(painter, brushGooseOrange, gooseRig.lFootPos, 4);
  fillCircleFromCenter(painter, brushGooseOrange, gooseRig.rFootPos, 4);

  fillEllipseFromCenter(painter, shadowBrush, position, 20, 15);

  pen.setColor(gooseOutlineColor);
  pen.setWidth(22 + num5);
  painter->setPen(pen);
  QPointF bodyLineStart = gooseRig.bodyCenter + multiplyScalar(forward, 11);
  QPointF bodyLineEnd = gooseRig.bodyCenter - multiplyScalar(forward, 11);
  painter->drawLine(bodyLineStart, bodyLineEnd);

  pen.setWidth(13 + num5);
  painter->setPen(pen);
  painter->drawLine(gooseRig.neckBase, gooseRig.neckHeadPoint);

  pen.setWidth(15 + num5);
  painter->setPen(pen);
  painter->drawLine(gooseRig.neckHeadPoint, gooseRig.head1EndPoint);

  pen.setWidth(10 + num5);
  painter->setPen(pen);
  painter->drawLine(gooseRig.head1EndPoint, gooseRig.head2EndPoint);

  pen.setWidth(15);
  painter->setPen(pen);
  QPointF underbodyStart =
      gooseRig.underbodyCenter + multiplyScalar(forward, 7);
  QPointF underbodyEnd = gooseRig.underbodyCenter - multiplyScalar(forward, 7);
  painter->drawLine(underbodyStart, underbodyEnd);

  pen.setColor(brushGooseWhite.color());
  pen.setWidth(22);
  painter->setPen(pen);
  bodyLineStart = gooseRig.bodyCenter + multiplyScalar(forward, 11);
  bodyLineEnd = gooseRig.bodyCenter - multiplyScalar(forward, 11);
  painter->drawLine(bodyLineStart, bodyLineEnd);

  pen.setWidth(13);
  painter->setPen(pen);
  painter->drawLine(gooseRig.neckBase, gooseRig.neckHeadPoint);

  pen.setWidth(15);
  painter->setPen(pen);
  painter->drawLine(gooseRig.neckHeadPoint, gooseRig.head1EndPoint);

  pen.setWidth(10);
  painter->setPen(pen);
  painter->drawLine(gooseRig.head1EndPoint, gooseRig.head2EndPoint);

  int num6 = 9;
  int num7 = 3;
  pen.setWidth(num6);
  pen.setColor(brushGooseOrange.color());
  painter->setPen(pen);
  QPointF headTip = gooseRig.head2EndPoint + multiplyScalar(forward, num7);
  painter->drawLine(gooseRig.head2EndPoint, headTip);

  QPointF rightScaled = multiplyPoint(right, vector2);
  QPointF pos1 = gooseRig.neckHeadPoint + multiplyScalar(vecUp, 3) -
                 multiplyScalar(rightScaled, 5) + multiplyScalar(forward, 5);
  QPointF pos2 = gooseRig.neckHeadPoint + multiplyScalar(vecUp, 3) +
                 multiplyScalar(rightScaled, 5) + multiplyScalar(forward, 5);

  fillCircleFromCenter(painter, QBrush(Qt::black), pos1, 2);
  fillCircleFromCenter(painter, QBrush(Qt::black), pos2, 2);
}

void goose::updatePos(float currentTime) {
  float timeDelta = currentTime - lastUpdate;
  lastUpdate = currentTime;
  float speed = norm(velocity);
  velocity -= (speed > topSpeed ? 1 : 0) * normalized(velocity) * acceleration *
              timeDelta;
  velocity += normalized(target - position) * acceleration * timeDelta / 2;
  position += velocity * timeDelta / 2;
  if (norm(target - position) < 10)
    return;
  float targetAngle = angle(target - position);
  if (abs(direction - targetAngle) > abs(direction - targetAngle - 2 * M_PI))
    direction -= 2 * M_PI;
  if (abs(direction - targetAngle) > abs(direction - targetAngle + 2 * M_PI))
    direction += 2 * M_PI;
  direction = lerp(direction, angle(target - position), 0.25);
}

void goose::setSpeed(speedTier tier) {
  speed = tier;
  switch (tier) {
  case walking:
    topSpeed = 80;
    acceleration = 1300;
    stepTime = 0.2;
    break;
  case running:
    topSpeed = 200;
    acceleration = 1300;
    stepTime = 0.2;
    break;
  case charging:
    topSpeed = 400;
    acceleration = 2300;
    stepTime = 0.1;
    break;
  case stopped:
  default:
    topSpeed = 0;
    acceleration = 1300;
    stepTime = 0.2;
  }
}

void goose::setTarget(QPointF target) { this->target = target; }

QPointF goose::getPosition() { return position; }

QPointF goose::getTarget() { return target; }

float goose::getTopSpeed() { return topSpeed; }

bool goose::draw(QPainter *painter, float currentTime) {
  updatePos(currentTime);
  updateRig(currentTime);
  drawRig(painter, currentTime);

  return false;
}

QPointF goose::getBeakPos() { return gooseRig.head2EndPoint; }

void goose::setMuddyFeet(float currentTime) { muddyFeetTime = currentTime; }
