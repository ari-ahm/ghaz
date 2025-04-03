#include "goose/mainloop.hpp"
#include <QApplication>
#include <cstdlib>
#include <ctime>
#include <qwidget.h>

int main(int argc, char *argv[]) {
  srand(time(0));
  QApplication app(argc, argv);
  QWidget par; // so that it's not shown in the taskbar
  mainloop goose(&par);
  goose.show();
  return app.exec();
}
