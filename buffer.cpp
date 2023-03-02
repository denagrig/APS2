#include "Buffer.h"

Buffer::Buffer(Statistic& stat) :
  bufferSize_(stat.getBufferSize()),
  freeSpace_(stat.getBufferSize())
{
  requestVector_ = std::vector<requestPointer>(stat.getBufferSize());
  requestVector_.reserve(stat.getBufferSize());
}

bool Buffer::addRequest(const requestPointer newRequest, step& step)
{
  if (freeSpace_ != 0)
  {
    int iterNum = 0;
    int i = tail_;
    while (iterNum != bufferSize_)
    {
      if (requestVector_[i] == nullptr)
      {
        tail_ = i;
        break;
      }
      if (i == bufferSize_ - 1)
      {
        i = 0;
      }
      else
      {
        i++;
      }
      iterNum++;
    }
    requestVector_[tail_] = newRequest;
    step.time = newRequest->getStartTime() + 0.00001;
    step.bufferStateVector[tail_] = newRequest;
    step.actionString = "Request " + std::to_string(newRequest->getUserNumber() + 1) + "." + std::to_string(newRequest->getRequestNumber()) + " generated and added to buffer.";
    step.stepNumber++;
    if (tail_ == bufferSize_ - 1)
    {
      tail_ = 0;
    }
    else
    {
      tail_++;
    }
    minusFreeSpace();
    return true;
  }
  else
  {

    int newNum = tail_-1;
    if (tail_ == 0)
    {
      newNum = requestVector_.size() - 1;
    }

    requestVector_[newNum] = newRequest;
    step.time = newRequest->getStartTime();
    step.bufferStateVector[newNum] = newRequest;
    step.actionString = "Request" + std::to_string(newRequest->getUserNumber() + 1) + "." + std::to_string(newRequest->getRequestNumber()) + " generated and added to buffer. The newest order was knocked out";
    tail_ = newNum;
    return false;
  }

  return false;
}

void Buffer::minusFreeSpace()
{
  freeSpace_--;
}

bool Buffer::isEmpty()
{
  return freeSpace_ == bufferSize_;
}

bool Buffer::isFull()
{
  return freeSpace_ == 0;
}

int Buffer::getBufferSize() const
{
  return bufferSize_;
}

int Buffer::getFreeSpace() const
{
  return freeSpace_;
}

requestPointer Buffer::getRequest(step& step)
{
  requestPointer req; 
  int num = 0;
  for (int i = 0; i < (requestVector_.size()); i++)
  {
    if (requestVector_[i] != nullptr)
    {
      req = requestVector_[i];
      num = i;
      break;
    }
  }

  for (int i = 1; i < (requestVector_.size()); i++)
  {
    if (requestVector_[i] == nullptr)
    {
      continue;
    }
    if (req->getSrcNumber() > requestVector_[i]->getSrcNumber())
    {
      req = requestVector_[i];
      num = i;
    }
  }
  step.bufferStateVector[num] = nullptr;
  requestVector_[num] = nullptr;
  freeSpace_++;

  return req;
}