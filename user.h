#ifndef USER_H
#define USER_H

#include "request.h"

#include <random>

class User
{
public:
  User(int number, double speed, double alpha, double beta);

  int getUserNumber();
  int getGeneratedTasks();
  double getRequestTime();
  requestPointer generation();

private:
  int userNumber_;
  int generatedTasks_ = 0;
  double sourceSpeed_ = 0.0;
  double newRequestTime_ = 0.0;
  double alpha_ = 0.0;
  double beta_ = 0.0;
};

#endif