#include "goose.hpp"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QVector>
#include <qeasingcurve.h>
#include <qpoint.h>
#include <qvector2d.h>

inline QPointF getFromAngleDegrees(float angleDegrees) {
  float rad = qDegreesToRadians(angleDegrees);
  return QPointF(qCos(rad), qSin(rad));
}

inline float lerp(float start, float end, float t) {
  return start + t * (end - start);
}

inline QPoint toIntPoint(const QPointF &pt) {
  return QPoint(qRound(pt.x()), qRound(pt.y()));
}

inline void fillCircleFromCenter(QPainter *painter, const QPointF &center,
                                 int radius, const QColor &color) {
  painter->save();
  painter->setBrush(QBrush(color));
  painter->setPen(Qt::NoPen);
  painter->drawEllipse(center, radius, radius);
  painter->restore();
}

inline void fillEllipseFromCenter(QPainter *painter, const QPointF &center,
                                  int xRadius, int yRadius,
                                  const QBrush &brush) {
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
  QPointF fromAngle = getFromAngleDegrees(direction);

  QPointF a2(0.0f, -1.0f);

  gooseRig.underbodyCenter = a + a2 * 9.0f;
  gooseRig.bodyCenter = a + a2 * 14.0f;

  int num4 = static_cast<int>(lerp(20.0f, 10.0f, gooseRig.neckLerpPercent));
  int num5 = static_cast<int>(lerp(3.0f, 16.0f, gooseRig.neckLerpPercent));

  gooseRig.neckCenter = a + a2 * (14.0f + num4);
  gooseRig.neckBase = gooseRig.bodyCenter + fromAngle * 15.0f;
  gooseRig.neckHeadPoint = gooseRig.neckBase +
                           fromAngle * static_cast<float>(num5) +
                           a2 * static_cast<float>(num4);
  gooseRig.head1EndPoint =
      gooseRig.neckHeadPoint + fromAngle * 3.0f - a2 * 1.0f;
  gooseRig.head2EndPoint = gooseRig.head1EndPoint + fromAngle * 5.0f;

  solveFeet(currentTime);
}

QPointF goose::getFootHome(bool isRight) {
  return position + getFromAngleDegrees(direction + 90) * 6 * (isRight ? 1 : 0);
}

void goose::solveFeet(float currentTime) {
  QPointF footHomeLeft = getFootHome(false);
  QPointF footHomeRight = getFootHome(true);

  auto distance = [](const QPointF &p1, const QPointF &p2) {
    return QVector2D(p2 - p1).length();
  };

  if (gooseRig.lFootTime < 0.0f && gooseRig.rFootTime < 0.0f) {
    if (distance(gooseRig.lFootPos, footHomeLeft) > 5.0f) {
      gooseRig.lFootOrig = gooseRig.lFootPos;
      gooseRig.lFootDir =
          QVector2D(footHomeLeft - gooseRig.lFootPos).normalized();
      gooseRig.lFootTime = currentTime;
      return;
    }
    if (distance(gooseRig.rFootPos, footHomeRight) > 5.0f) {
      gooseRig.rFootOrig = gooseRig.rFootPos;
      gooseRig.rFootDir =
          QVector2D(footHomeRight - gooseRig.rFootPos).normalized();
      gooseRig.rFootTime = currentTime;
      return;
    }
  } else {
    QEasingCurve easingCurve(QEasingCurve::InOutCubic);

    if (gooseRig.lFootTime > 0.0f) {
      float progress = (currentTime - gooseRig.lFootTime) / stepTime;
      if (progress < 1.0f) {
        float easedProgress = easingCurve.valueForProgress(progress);
        gooseRig.lFootPos =
            gooseRig.lFootOrig + gooseRig.lFootDir.toPointF() * easedProgress *
                                     distance(gooseRig.lFootOrig, footHomeLeft);
        gooseRig.lFootPos.setY(gooseRig.lFootPos.y() -
                               footLiftHeight * sin(easedProgress * M_PI));
      } else {
        gooseRig.lFootPos = footHomeLeft;
        gooseRig.lFootTime = -1.0f;
        /*playFootstepSound();*/
        /*addFootMark(lFootPos);*/
      }
    } else {
      float progress = (currentTime - gooseRig.rFootTime) / stepTime;
      if (progress < 1.0f) {
        float easedProgress = easingCurve.valueForProgress(progress);
        gooseRig.rFootPos = gooseRig.rFootOrig +
                            gooseRig.rFootDir.toPointF() * easedProgress *
                                distance(gooseRig.rFootOrig, footHomeRight);
        gooseRig.rFootPos.setY(gooseRig.rFootPos.y() -
                               footLiftHeight * sin(easedProgress * M_PI));
      } else {
        gooseRig.rFootPos = footHomeRight;
        gooseRig.rFootTime = -1.0f;
        /*playFootstepSound();*/
        /*addFootMark(rFootPos);*/
      }
    }
  }
}

bool goose::draw(QPainter *painter, float currentTime) {
  for (int i = 0; i < footMarks.size(); ++i) {
    if (footMarks[i].time != 0.0f) {
      float tOffset = footMarks[i].time + 8.5f;

      float delta = qBound(0.0f, currentTime - tOffset, 1.0f);

      float radius = 3.0f + delta * (0.0f - 3.0f);
      fillCircleFromCenter(painter, footMarks[i].position,
                           static_cast<int>(radius), QColor(139, 69, 19));
    }
  }

  updateRig(currentTime);

  float angle = direction;
  QPointF posInt(qRound(position.x()), qRound(position.y()));
  QPointF b(1.3, 0.4);
  QPointF fromAngle = getFromAngleDegrees(angle);
  fromAngle.setX(fromAngle.x() * b.x());
  fromAngle.setY(fromAngle.y() * b.y());
  QPointF fromAngle90 = getFromAngleDegrees(angle + 90);
  fromAngle90.setX(fromAngle90.x() * b.x());
  fromAngle90.setY(fromAngle90.y() * b.y());
  QPointF a(0, -1);

  drawingPen.setColor(Qt::white);

  fillCircleFromCenter(painter, gooseRig.lFootPos, 4, Qt::darkYellow);
  fillCircleFromCenter(painter, gooseRig.rFootPos, 4, Qt::darkYellow);

  fillEllipseFromCenter(painter, posInt, 20, 15, shadowBrush);

  drawingPen.setColor(Qt::lightGray);
  drawingPen.setWidthF(24.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.bodyCenter + fromAngle * 11.0),
                    toIntPoint(gooseRig.bodyCenter - fromAngle * 11.0));

  drawingPen.setWidthF(15.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.neckBase),
                    toIntPoint(gooseRig.neckHeadPoint));

  drawingPen.setWidthF(17.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.neckHeadPoint),
                    toIntPoint(gooseRig.head1EndPoint));

  drawingPen.setWidthF(12.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.head1EndPoint),
                    toIntPoint(gooseRig.head2EndPoint));

  drawingPen.setColor(Qt::lightGray);
  drawingPen.setWidthF(15.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.underbodyCenter + fromAngle * 7.0),
                    toIntPoint(gooseRig.underbodyCenter - fromAngle * 7.0));

  drawingPen.setColor(Qt::white);
  drawingPen.setWidthF(22.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.bodyCenter + fromAngle * 11.0),
                    toIntPoint(gooseRig.bodyCenter - fromAngle * 11.0));

  drawingPen.setWidthF(13.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.neckBase),
                    toIntPoint(gooseRig.neckHeadPoint));

  drawingPen.setWidthF(15.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.neckHeadPoint),
                    toIntPoint(gooseRig.head1EndPoint));

  drawingPen.setWidthF(10.0);
  painter->setPen(drawingPen);
  painter->drawLine(toIntPoint(gooseRig.head1EndPoint),
                    toIntPoint(gooseRig.head2EndPoint));

  drawingPen.setWidthF(9.0);
  QColor orangeColor(255, 165, 0);
  drawingPen.setColor(orangeColor);
  painter->setPen(drawingPen);
  QPointF vector2 = gooseRig.head2EndPoint + fromAngle * 3.0;
  painter->drawLine(toIntPoint(gooseRig.head2EndPoint), toIntPoint(vector2));

  QPointF scaledFromAngle90(fromAngle90.x() * b.x(), fromAngle90.y() * b.y());
  QPointF pos = gooseRig.neckHeadPoint + a * 3.0 - scaledFromAngle90 * 5.0 +
                fromAngle * 5.0;
  QPointF pos2 = gooseRig.neckHeadPoint + a * 3.0 + scaledFromAngle90 * 5.0 +
                 fromAngle * 5.0;
  fillCircleFromCenter(painter, pos, 2, Qt::black);
  fillCircleFromCenter(painter, pos2, 2, Qt::black);

  return false;
}

void goose::setDirection(float direction) { this->direction = direction; }
void goose::setPosition(QPointF position) { this->position = position; }
float goose::getDirection() { return direction; }
QPointF goose::getPosition() { return position; }
