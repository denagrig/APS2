#ifndef STEP_H
#define STEP_H

#include <vector>
#include <string>
#include "request.h"

struct step
{
  double time;
  std::string actionString;
  std::vector<requestPointer> userStateVector;
  std::vector<requestPointer> bufferStateVector;
  std::vector<requestPointer> execStateVector;
  int stepNumber;
};

#endif