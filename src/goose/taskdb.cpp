#include "taskdb.hpp"
#include "tasks/task.hpp"
#include <cstdlib>
#include <functional>

void taskdb::addTask(std::function<task *()> factory) {
  tasks.push_back(factory);
}

task *taskdb::getRandom() { return tasks[rand() % tasks.size()](); }
