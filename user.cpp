#include "user.h"

User::User(int number, double speed, double alpha, double beta) :
  userNumber_(number),
  sourceSpeed_(speed),
  alpha_(alpha),
  beta_(beta)
{ }

int User::getUserNumber()
{
  return userNumber_;
}

int User::getGeneratedTasks()
{
  return generatedTasks_;
}

double User::getRequestTime()
{
  return newRequestTime_;
}

requestPointer User::generation()
{
  generatedTasks_++;
  newRequestTime_ += (alpha_ + (beta_ - alpha_)*rand()/RAND_MAX);
  return Request::getNewRequest(newRequestTime_, userNumber_, generatedTasks_);
}