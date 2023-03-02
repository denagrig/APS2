#include "distributor.h"

Distributor::Distributor(Statistic& stat) :
  size_(stat.getExecAmount())
{
  for (int i = 0; i < size_; i++)
  {
    Executor exec(i, stat.getLambda());
    execVector_.push_back(exec);
  }
}

Executor& Distributor::getExecuter()
{
  Executor& res = execVector_[head_];
  if (head_ == (execVector_.size() - 1))
  {
    head_ = 0;
  }
  else
  {
    head_++;
  }
  return res;
}

Executor& Distributor::getExecuterCheck()
{
  return execVector_[head_];
}