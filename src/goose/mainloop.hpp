#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

#include "graphics/graphic.hpp"
#include "tasks/task.hpp"
#include <QTimer>
#include <QWidget>
#include <qelapsedtimer.h>
#include <qset.h>
#include <qtmetamacros.h>
#include <qwidget.h>

class mainloop : public QWidget {
  Q_OBJECT
public:
  explicit mainloop(QWidget *parent = nullptr);
  void registerGraphics(graphic *obj);

protected:
  void paintEvent(QPaintEvent *event) override;
private slots:
  void onUpdate();

private:
  task *myTask;
  QSet<graphic *> graphicObjects;
  QTimer *updateTimer;
  QElapsedTimer epochTime;
};

#endif
