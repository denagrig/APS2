#ifndef REQUEST_H
#define REQUEST_H

#include <memory>

class Request
{
public:
	Request(double startTime, int sourceNumber, int requestNumber);
	~Request() = default;

	static std::shared_ptr<Request> getNewRequest(double startTime, int sourceNumber, int requestNumber);

	int getUserNumber();
	int getRequestNumber();
	double getStartTime();
	int getSerialNumber();
	int getSrcNumber();
	void setSerialNumber(int number);

private:
	double reqStartTime_ = 0;
	int reqNumber_ = 0;
	int srcNumber_ = 0;
	int serNumber_ = 0;
};

typedef std::shared_ptr<Request> requestPointer;

#endif