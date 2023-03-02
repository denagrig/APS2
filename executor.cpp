#include "executor.h"

Executor::Executor(int execNumber, double lambda) :
  execNumber_(execNumber),
  lambda_(lambda)
{
}

bool Executor::isAvaliable(Statistic& statistic, step& step)
{
  if (currentRequest_ == nullptr)
  {
    return true;
  }
  else {
    fillStatistic(statistic, step);
    currentRequest_ = nullptr;
    taskStartTime_ = taskEndTime_;
    return true;
  }
}

bool Executor::isAvaliable(double lastRequestTime, Statistic& statistic, step& step)
{
  if (currentRequest_ == nullptr)
  {
    return true;
  }

  if (taskEndTime_ < lastRequestTime)
  {
    fillStatistic(statistic, step);
    currentRequest_ = nullptr;
    taskStartTime_ = taskEndTime_;
    return true;
  }
  return false;
}

void Executor::getRequest(const requestPointer newRequest, step& step)
{
  currentRequest_ = newRequest;
  step.execStateVector[execNumber_] = newRequest;

  if ((taskStartTime_ == 0) || (taskStartTime_ < newRequest->getStartTime() + 0.00002))
  {
    taskStartTime_ = newRequest->getStartTime() + 0.00002;
  }
  double temp = rand();
  taskEndTime_ = taskStartTime_ + (lambda_ * pow(MATH_E, -lambda_ * temp / RAND_MAX)); //((log(rand() + 1) - log(RAND_MAX) / -lambda_));

  step.time = taskStartTime_;
  step.execStateVector[execNumber_] = newRequest;// std::to_string(currentRequest_->getSourceNumber() + 1)+ "." +
  step.actionString = "Request " + std::to_string(newRequest->getUserNumber() + 1) + "." + std::to_string(newRequest->getRequestNumber())
    + " is on device " + std::to_string(getExecutorNumber() + 1);
  step.stepNumber++;
}

void Executor::fillStatistic(Statistic& statistic, step& step)
{
  step.time = taskEndTime_;
  step.execStateVector[execNumber_] = nullptr;//+ std::to_string(currentRequest_->getSourceNumber() + 1)+ "."
  step.actionString = "Request " + std::to_string(currentRequest_->getUserNumber() + 1) + "." + std::to_string(currentRequest_->getRequestNumber())
    + " processed. Executor " + std::to_string(getExecutorNumber() + 1) + " is free.";
  statistic.addStep(step);

  statistic.incrementProcessed(currentRequest_->getUserNumber());
  statistic.addSimulationTime(currentRequest_->getUserNumber(), taskEndTime_ - currentRequest_->getStartTime() + 0.00002);
  statistic.addExecWorkingTime(execNumber_, taskEndTime_ - taskStartTime_);
  statistic.addWaitingTime(currentRequest_->getUserNumber(), taskStartTime_ - currentRequest_->getStartTime());
  statistic.addServicingTime(currentRequest_->getUserNumber(), taskEndTime_ - taskStartTime_);
  statistic.setTotalSimulationTime(taskEndTime_);
}

int Executor::getExecutorNumber() const
{
  return execNumber_;
}

double Executor::getTaskStartTime() const
{
  return taskStartTime_;
}