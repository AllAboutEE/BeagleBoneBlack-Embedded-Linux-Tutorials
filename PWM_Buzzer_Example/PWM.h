#ifndef H_PWM_H_
#define H_PWM_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

class PWM
{
private:
	string pwmPin;
	string pwmPinPath;
	string pwmOverlay;
	const string slotsPath = "/sys/devices/bone_capemgr.9/slots";
	const string amOverlay = "am33xx_pwm";
	const string pwmPath = "/sys/devices/ocp.3/";

	void writeToFile(string path, string filename, int value);
	string readFromFile(string path, string filename);
	void deployOverlay(string overlay);

public:
	PWM(string pinName);
	~PWM();
	void setPinAttributes(string attributes, unsigned int value);
	int getPinAttributes(string attributes);
};

#endif /* H_PWM_H_ */
