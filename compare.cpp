#include "compare.h"

bool cmp::operator() (const requestPointer& a, const requestPointer& b)
{
  return a->getStartTime() < b->getStartTime();
}

bool cmp::operator() (User& a, User& b)
{
  return a.getRequestTime() < b.getRequestTime();
}