#ifndef COMPARE_H
#define COMPARE_H

#include "request.h"
#include "user.h"

class cmp {
public:
  bool operator() (const requestPointer& a, const requestPointer& b);
  bool operator() (User& a, User& b);
};

#endif