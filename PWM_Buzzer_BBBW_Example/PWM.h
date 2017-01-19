#ifndef H_PWM_H_
#define H_PWM_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

/* PWM Device Tree Overlays
 * BB-PWM0
 * BB-PWM1
 * BB-PWM2
 */

class PWM
{
private:
	string pwmPin;
	string pwmPinPath;
	const string pwmPath = "/sys/class/pwm/";
	string slotsPath = "/sys/devices/platform/bone_capemgr/slots";

	void writeToFile(string path, string filename, int value);
	string readFromFile(string path, string filename);
	void deployOverlay(string overlay);

public:

	PWM(string dto, string pinName);
	~PWM();
	void setPinAttributes(string attributes, unsigned int value);
	int getPinAttributes(string attributes);
};

#endif /* H_PWM_H_ */
