#ifndef SERVER_H
#define SERVER_H

#include "request.h"
#include "user.h"
#include "statistic.h"
#include "compare.h"

#include <random>

class Server
{
public:
  Server(Statistic& stat);

  User getUser(size_t num);
  requestPointer getRequest(size_t num);
  requestPointer getRequest(size_t num, step& step);

private:
  int userSize_;
  int reqSize_;
  std::vector<User> userVector_;
  std::vector<requestPointer> reqVector_;
};

#endif