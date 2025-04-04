#include "mainloop.hpp"
#include "graphics/goose.hpp"
#include "graphics/graphic.hpp"
#include "tasks/mudTrack.hpp"
#include "tasks/nabMouse.hpp"
#include "tasks/wander.hpp"
#include <GL/gl.h>
#include <QScreen>
#include <cmath>
#include <qcursor.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qregion.h>
#include <qset.h>

mainloop::mainloop(QWidget *parent) : QOpenGLWidget(parent) {
  registerGraphics(new goose(), "goose");
  (dynamic_cast<goose *>(getGraphic("goose")))->setSpeed(goose::stopped);

  myTaskdb.addTask([this]() { return new wander(this); });
  myTaskdb.addTask([this]() { return new nabMouse(this); });
  myTaskdb.addTask([this]() { return new mudTrack(this); });

  int h = this->screen()->size().height();
  int w = this->screen()->size().width();

  setAttribute(Qt::WA_OpaquePaintEvent, false);
  setAttribute(Qt::WA_TranslucentBackground, true);
  // setAttribute(Qt::WA_TransparentForMouseEvents, true);
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  /*setWindowState(Qt::WindowFullScreen);*/

  setFixedHeight(h);
  setFixedWidth(w);

  updateTimer = new QTimer(this);
  connect(updateTimer, &QTimer::timeout, this, &mainloop::onUpdate);
  updateTimer->start(16);

  epochTime.start();
}

void mainloop::registerGraphics(graphic *obj, std::string name) {
  graphicObjects.insert(obj);
  if (name != "")
    namedGraphics[name] = obj;
}

void mainloop::setTask(task *task) { myTask.push(task); }

float mainloop::getCurrentTime() { return epochTime.elapsed() / 1000.0f; }

graphic *mainloop::getGraphic(std::string name) { return namedGraphics[name]; }

void mainloop::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO add nabMouse on click
  // QPointF pos = dynamic_cast<goose *>(getGraphic("goose"))->getPosition();
  // QRegion circleRegion(pos.x() - 50, pos.y() - 50, 100, 100,
  // QRegion::Ellipse); setMask(circleRegion);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // painter.fillRect(rect(), Qt::black);

  float currentTime = epochTime.elapsed() / 1000.0f;

  for (graphic *i : graphicObjects) {
    i->draw(&painter, currentTime);
  }
}

void mainloop::onUpdate() {
  float currentTime = epochTime.elapsed() / 1000.0f;
  if (myTask.empty())
    setTask((isWander = !isWander) ? new wander(this) : myTaskdb.getRandom());
  if (myTask.top()->tick(currentTime))
    myTask.pop();
  update();
}
