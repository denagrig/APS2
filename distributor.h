#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H

#include "executor.h"
#include "statistic.h"

#include <random>

class Distributor
{
public:
  Distributor(Statistic& stat);

  Executor& getExecuter();
  Executor& getExecuterCheck();
private:
  int size_;
  std::vector<Executor> execVector_;
  int head_ = 0;
};

#endif