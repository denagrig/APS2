#include "statistic.h"
#include "Compare.h"

#include <numeric>
#include <cmath>

Statistic::Statistic(int requestAmount, int userAmount, int bufferSize, int execAmount, double lambda, double alpha, double beta)
{
  this->requestAmount = requestAmount;
  this->userAmount = userAmount;
  this->bufferSize = bufferSize;
  this->execAmount = execAmount;
  this->lambda = lambda;
  this->alpha = alpha;
  this->beta = beta;
  totalSimulationTime = 0.0;
  rejectedRequestVector = std::vector<int>(userAmount, 0);
  processedRequestVector = std::vector<int>(userAmount, 0);
  execWorkingTimeVector = std::vector<double>(execAmount, 0.0);
  simulationTimeVector = std::vector<double>(userAmount, 0.0);
  waitingTimeVector = std::vector<std::vector<double>>(userAmount);
  servicingTimeVector = std::vector<std::vector<double>>(userAmount);
}

void Statistic::incrementProcessed(int userNumber)
{
  processedRequestVector[userNumber]++;
}

void Statistic::incrementRejected(int userNumber)
{
  rejectedRequestVector[userNumber]++;
}

void Statistic::addSimulationTime(int userNumber, double time)
{
  simulationTimeVector[userNumber] += time;
}

void Statistic::addWaitingTime(int userNumber, double time)
{
  waitingTimeVector[userNumber].push_back(time);
}

void Statistic::addServicingTime(int userNumber, double time)
{
  servicingTimeVector[userNumber].push_back(time);
}

void Statistic::addExecWorkingTime(int execNumber, double time)
{
  execWorkingTimeVector[execNumber] += time;
}

void Statistic::setTotalSimulationTime(double time)
{
  totalSimulationTime = time;
}

void Statistic::addStep(step& step)
{
  stepVector.push_back(step);
}

int Statistic::getProcessed()
{
  int processed = 0;

  for (int i = 0; i < (int)processedRequestVector.size(); i++)
  {
    processed += processedRequestVector[i];
  }
  return processed;
}

int Statistic::getProcessed(int userNumber)
{
  return processedRequestVector[userNumber];
}

int Statistic::getRejected()
{
  int rejected = 0;

  for (int i = 0; i < (int)rejectedRequestVector.size(); i++)
  {
    rejected += rejectedRequestVector[i];
  }
  return rejected;
}

int Statistic::getRejected(int userNumber)
{
  return rejectedRequestVector[userNumber];
}

int Statistic::getRequestAmount()
{
  return requestAmount;
}

int Statistic::getUserAmount()
{
  return userAmount;
}

int Statistic::getExecAmount()
{
  return execAmount;
}

int Statistic::getBufferSize()
{
  return bufferSize;
}

double Statistic::getRejectProbality(int userNumber)
{
  return (double)rejectedRequestVector[userNumber] /
    (double)(processedRequestVector[userNumber] + rejectedRequestVector[userNumber]);
}

double Statistic::getWaitingTime(int userNumber)
{
  return calculateDoubleVector(waitingTimeVector[userNumber]) /
    (processedRequestVector[userNumber] + rejectedRequestVector[userNumber]);
}

double Statistic::getWaitingTimeDispersion(int userNumber)
{
  int requestAmount = (processedRequestVector[userNumber] + rejectedRequestVector[userNumber]);
  double sum = 0.0, averageWaitingTime = calculateDoubleVector(waitingTimeVector[userNumber]) / requestAmount;

  for (int i = 0; i < (int)waitingTimeVector[userNumber].size(); i++)
  {
    sum += std::pow(waitingTimeVector[userNumber][i] - averageWaitingTime, 2);
  }
  return sum / requestAmount;
}

double Statistic::getServicingTime(int userNumber)
{
  return calculateDoubleVector(servicingTimeVector[userNumber]) /
    (processedRequestVector[userNumber] + rejectedRequestVector[userNumber]);
}

double Statistic::getServicingTimeDispersion(int userNumber)
{
  double sum = 0.0, averageServiceTime = calculateDoubleVector(servicingTimeVector[userNumber]) /
    processedRequestVector[userNumber];

  for (int i = 0; i < (int)servicingTimeVector[userNumber].size(); i++)
  {
    sum += std::pow(servicingTimeVector[userNumber][i] - averageServiceTime, 2);
  }
  return sum / processedRequestVector[userNumber];
}

double Statistic::getSimulationTime(int userNumber)
{
  return simulationTimeVector[userNumber] /
    (processedRequestVector[userNumber] + rejectedRequestVector[userNumber]);
}

double Statistic::getExecUsage(int execNumber)
{
  return execWorkingTimeVector[execNumber] / totalSimulationTime;
}

double Statistic::getLambda()
{
  return lambda;
}

double Statistic::getAlpha()
{
  return alpha;
}

double Statistic::getBeta()
{
  return beta;
}

std::vector<step> Statistic::getStepVector()
{
  return stepVector;
}

double Statistic::calculateDoubleVector(std::vector<double> vector)
{
  return std::accumulate(vector.begin(), vector.end(), 0.0);
}

std::vector<double> Statistic::getUserTimeVect()
{
  return userTimeVector;
}

std::vector<double> Statistic::getBufferTimeVect()
{
  return bufferTimeVector;
}

std::vector<double> Statistic::getExecTimeVect()
{
  return execTimeVector;
}

std::vector<step> Statistic::getUserStepVect()
{
  return userStepVector;
}

std::vector<step> Statistic::getBufferStepVect()
{
  return bufferStepVector;
}

std::vector<step> Statistic::getExecStepVect()
{
  return execStepVector;
}

void Statistic::addUserTime(double time)
{
  userTimeVector.push_back(time);
}

void Statistic::addBufferTime(double time)
{
  bufferTimeVector.push_back(time);
}

void Statistic::addExecTime(double time)
{
  execTimeVector.push_back(time);
}

void Statistic::addUserStep(step step)
{
  userStepVector.push_back(step);
}

void Statistic::addBufferStep(step step)
{
  bufferStepVector.push_back(step);
}

void Statistic::addExecStep(step step)
{
  execStepVector.push_back(step);
}