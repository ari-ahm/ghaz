#include "mainloop.hpp"
#include "graphics/graphic.hpp"
#include "tasks/wander.hpp"
#include <QScreen>
#include <cmath>
#include <qcursor.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qset.h>

mainloop::mainloop(QWidget *parent) : QWidget(parent) {
  myTask = new wander(this);
  int h = this->screen()->size().height();
  int w = this->screen()->size().width();

  setAttribute(Qt::WA_OpaquePaintEvent, false);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setAttribute(Qt::WA_TransparentForMouseEvents, true);
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  /*setWindowState(Qt::WindowFullScreen);*/

  setFixedHeight(h);
  setFixedWidth(w);

  updateTimer = new QTimer(this);
  connect(updateTimer, &QTimer::timeout, this, &mainloop::onUpdate);
  updateTimer->start(16);

  epochTime.start();
}

void mainloop::registerGraphics(graphic *obj) { graphicObjects.insert(obj); }

void mainloop::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  // painter.fillRect(rect(), Qt::black);

  float currentTime = epochTime.elapsed() / 1000.0f;

  for (graphic *i : graphicObjects) {
    i->draw(&painter, currentTime);
  }
}

void mainloop::onUpdate() {
  float currentTime = epochTime.elapsed() / 1000.0f;
  myTask->tick(currentTime);
  update();
}
