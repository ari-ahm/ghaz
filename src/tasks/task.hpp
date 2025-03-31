#ifndef TASK_HPP
#define TASK_HPP

class task {
public:
  virtual task &tick(float currentTime) = 0;
};

#endif
