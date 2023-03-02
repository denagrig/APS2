#ifndef BUFFER_H
#define BUFFER_H

#include "request.h"
#include "Step.h"
#include "statistic.h"

#include <iostream>
#include <memory>
#include <vector>
#include <iterator>


class Buffer
{
public:
  Buffer(Statistic& stat);

  bool isEmpty();
  bool isFull();

  void minusFreeSpace();
  bool addRequest(const requestPointer newRequest, step& step);

  int getBufferSize() const;
  int getFreeSpace() const;

  requestPointer getRequest(step& step);

private:
  std::vector<requestPointer> requestVector_;
  size_t tail_ = 0;
  int bufferSize_ = 1;
  int freeSpace_;
};


#endif