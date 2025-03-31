#include "cursor.hpp"
#include <QDebug>
#include <QPoint>
#include <QPointF>
#include <QProcess>
#include <QString>
#include <QStringList>

// TODO implement other compositors than hyprland

QPointF cursor::getPos() {
  QProcess process;
  process.start("hyprctl", QStringList() << "cursorpos");
  if (!process.waitForFinished()) {
    qWarning() << "hyprctl cursorpos command timed out";
    return QPoint();
  }
  QString output = process.readAllStandardOutput().trimmed();
  QStringList parts = output.split(",");
  if (parts.size() < 2) {
    qWarning() << "Unexpected output from hyprctl cursorpos:" << output;
    return QPoint();
  }
  bool ok1, ok2;
  int x = parts.at(0).trimmed().toInt(&ok1);
  int y = parts.at(1).trimmed().toInt(&ok2);
  if (!ok1 || !ok2) {
    qWarning() << "Failed to parse coordinates from:" << output;
    return QPoint();
  }
  return QPoint(x, y);
}
void cursor::setPos(QPointF point) {
  QString posArg = QString("%1 %2")
                       .arg(static_cast<int>(point.x()))
                       .arg(static_cast<int>(point.y()));
  QProcess process;
  process.start("hyprctl", QStringList()
                               << "dispatch" << "movecursor" << posArg);
  if (!process.waitForFinished()) {
    qWarning() << "hyprctl dispatch movecursor command timed out";
    return;
  }
}
