#include "request.h"

Request::Request(double startTime, int sourceNumber, int requestNumber) :
  reqStartTime_(startTime),
  reqNumber_(requestNumber),
  srcNumber_(sourceNumber)
{ }

int Request::getUserNumber()
{
  return srcNumber_;
}

int Request::getRequestNumber()
{
  return reqNumber_;
}

double Request::getStartTime()
{
  return reqStartTime_;
}

int Request::getSerialNumber()
{
  return serNumber_;
}

int Request::getSrcNumber()
{
  return srcNumber_;
}

void Request::setSerialNumber(int number)
{
  serNumber_ = number;
}

std::shared_ptr<Request> Request::getNewRequest(double startTime, int sourceNumber, int requestNumber)
{
  return std::shared_ptr<Request>(new Request(startTime, sourceNumber, requestNumber));
}