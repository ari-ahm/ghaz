#ifndef GOOSE_HPP
#define GOOSE_HPP

#include "graphics/graphic.hpp"
#include <QWidget>
#include <qpainter.h>
#include <qpoint.h>
#include <qvectornd.h>

class goose : public graphic {
public:
  goose();
  bool draw(QPainter *painter, float currentTime) override;
  enum speedTier { stopped, walking, running, charging };
  void setSpeed(speedTier tier);
  void setTarget(QPointF target);
  QPointF getTarget();
  QPointF getPosition();
  float getTopSpeed();

private:
  struct GooseRig {
    QPointF underbodyCenter;
    QPointF bodyCenter;
    QPointF neckCenter;
    QPointF neckBase;
    QPointF neckHeadPoint;
    QPointF head1EndPoint;
    QPointF head2EndPoint;
    float neckLerpPercent = 0;
    QPointF lFootPos, lFootOrig;
    QPointF rFootPos, rFootOrig;
    float lFootTime = -1, rFootTime = -1;
    QPointF lFootDir, rFootDir;
  };

  float stepTime;

  QImage shadowBitmap;
  QBrush shadowBrush;
  QPen shadowPen;
  QPen drawingPen;
  QPointF position{-50, -50}, target;
  float direction = 0;
  float topSpeed, acceleration;
  float lastUpdate = 0;
  QPointF velocity;
  GooseRig gooseRig;
  speedTier speed;
  bool extendingNeck = false;

  void updateRig(float currentTime);
  void drawRig(QPainter *painter);
  void updatePos(float currentTime);
  void solveFeet(float currentTime);
};

#endif
