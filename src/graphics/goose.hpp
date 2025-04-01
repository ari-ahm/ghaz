#ifndef GOOSE_HPP
#define GOOSE_HPP

#include "graphics/graphic.hpp"
#include <QWidget>
#include <qpoint.h>
#include <qvectornd.h>

class goose : public graphic {
public:
  goose();
  bool draw(QPainter *painter, float currentTime) override;
  void setPosition(QPointF point);
  void setDirection(float direction);
  float getDirection();
  QPointF getPosition();

private:
  struct FootMark {
    QPointF position;
    float time;
  };

  struct GooseRig {
    QPointF underbodyCenter;
    QPointF bodyCenter;
    QPointF neckCenter;
    QPointF neckBase;
    QPointF neckHeadPoint;
    QPointF head1EndPoint;
    QPointF head2EndPoint;
    float neckLerpPercent = 0.5f;
    QPointF lFootPos, lFootOrig;
    QPointF rFootPos, rFootOrig;
    float lFootTime = -1, rFootTime = -1;
    QVector2D lFootDir, rFootDir;
  };

  float stepTime = 0.1f;
  const float footLiftHeight = 4.f;

  QImage shadowBitmap;
  QBrush shadowBrush;
  QPen shadowPen;
  QPen drawingPen;
  QVector<FootMark> footMarks; // TODO make a dedicated gobj
  QPointF position;
  float direction = 0;
  GooseRig gooseRig;

  void updateRig(float currentTime);
  QPointF getFootHome(bool isRight);
  void solveFeet(float currentTime);
};

#endif
