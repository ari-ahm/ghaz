#ifndef TASKDB_HPP
#define TASKDB_HPP

#include "tasks/task.hpp"
#include <QVector>
#include <functional>

class taskdb {
public:
  void addTask(std::function<task *()>);
  task *getRandom();

private:
  QVector<std::function<task *()>> tasks;
};

#endif
