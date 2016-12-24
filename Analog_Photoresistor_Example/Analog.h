#ifndef H_ANALOG_H_
#define H_ANALOG_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::string;


class Analog
{
private:
	string overlay;
	const string devicesPath = "/sys/bus/iio/devices";
	const string slotsPath = "/sys/devices/bone_capemgr.9/slots";

	int readFromFile(string fileName);
	void setAnalog();

public:
	Analog();
	~Analog();

	int getPinAttributes(string fileName);
};

#endif /* H_ANALOG_H_ */
