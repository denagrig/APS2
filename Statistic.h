#ifndef STATISTIC_H
#define STATISTIC_H

#include "step.h"

#include <vector>

class Statistic
{
public:
	Statistic(int, int, int, int, double, double, double);

	void incrementProcessed(int);
	void incrementRejected(int);

	void addSimulationTime(int, double);
	void addWaitingTime(int, double);
	void addServicingTime(int, double);
	void addExecWorkingTime(int, double);
	void addStep(step& step);

	void addUserTime(double time);
	void addBufferTime(double time);
	void addExecTime(double time);

	void addUserStep(step step);
	void addBufferStep(step step);
	void addExecStep(step step);

	void setTotalSimulationTime(double);

	int getProcessed();
	int getProcessed(int);
	int getRejected();
	int getRejected(int);
	int getRequestAmount();
	int getUserAmount();
	int getExecAmount();
	int getBufferSize();

	double getRejectProbality(int);
	double getWaitingTime(int);
	double getWaitingTimeDispersion(int);
	double getServicingTime(int);
	double getServicingTimeDispersion(int);
	double getSimulationTime(int);
	double getExecUsage(int);
	double getLambda();
	double getAlpha();
	double getBeta();

	std::vector<step> getStepVector();

	std::vector<double> getUserTimeVect();
	std::vector<double> getBufferTimeVect();
	std::vector<double> getExecTimeVect();

	std::vector<step> getUserStepVect();
	std::vector<step> getBufferStepVect();
	std::vector<step> getExecStepVect();

private:
	int requestAmount;
	int userAmount;
	int execAmount;
	int bufferSize;
	double lambda;
	double alpha;
	double beta;
	double totalSimulationTime;

	std::vector<int> processedRequestVector;
	std::vector<int> rejectedRequestVector;
	std::vector<double> simulationTimeVector;
	std::vector<double> execWorkingTimeVector;

	std::vector<double> userTimeVector;
	std::vector<double> bufferTimeVector;
	std::vector<double> execTimeVector;

	std::vector<std::vector<double>> waitingTimeVector;
	std::vector<std::vector<double>> servicingTimeVector;
	std::vector<step> stepVector;

	std::vector<step> userStepVector;
	std::vector<step> bufferStepVector;
	std::vector<step> execStepVector;

	double calculateDoubleVector(std::vector<double>);
};

#endif