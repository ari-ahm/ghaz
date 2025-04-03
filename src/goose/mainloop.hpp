#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

#include "goose/taskdb.hpp"
#include "graphics/graphic.hpp"
#include "tasks/task.hpp"
#include <QStack>
#include <QTimer>
#include <QWidget>
#include <qelapsedtimer.h>
#include <qmap.h>
#include <qset.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <set>

class mainloop : public QWidget {
  Q_OBJECT
public:
  explicit mainloop(QWidget *parent = nullptr);
  void registerGraphics(graphic *obj, std::string name = "");
  graphic *getGraphic(std::string name);
  void setTask(task *task);
  float getCurrentTime();

protected:
  void paintEvent(QPaintEvent *event) override;
private slots:
  void onUpdate();

private:
  QStack<task *> myTask;
  taskdb myTaskdb;
  std::set<graphic *, decltype(graphic::pointerCmp) *> graphicObjects;
  QMap<std::string, graphic *> namedGraphics;
  bool isWander = false;

  QTimer *updateTimer;
  QElapsedTimer epochTime;
};

#endif
