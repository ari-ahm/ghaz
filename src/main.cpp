#include "goose/mainloop.hpp"
#include <QApplication>
#include <cstdlib>
#include <ctime>
#include <qwidget.h>

int main(int argc, char *argv[]) {
  srand(time(0));
  QApplication app(argc, argv);
  mainloop goose;
  goose.show();
  return app.exec();
}
