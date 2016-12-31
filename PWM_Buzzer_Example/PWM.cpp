#include "PWM.h"

/*
 * Constructor
 */
PWM::PWM(string pinName)
{
	this->pwmOverlay = "bone_pwm_P9_22";
	this->pwmPin = pinName;
	this->pwmPinPath = this->pwmPath + this->pwmPin + "/";

	deployOverlay(this->pwmOverlay);
	deployOverlay(this->amOverlay);

	usleep(250000); // Delay to allow the setup of sysfs
}

/*
 * Destructor
 */
PWM::~PWM()
{
	setPinAttributes("run", 0);
}

/*
 * Sets the:
 * 	- period
 * 	- duty
 * 	- polarity
 * 	- run
 *
 * 	attributes: The pin's properties that will be modified
 * 	value: The value that will be assigned to the pin's properties
 */
void PWM::setPinAttributes(string attributes, unsigned int value)
{
	writeToFile(this->pwmPinPath, attributes, value);
}

/*
 * Gets the:
 * 	- period
 * 	- duty
 * 	- polarity
 * 	- run
 *
 * 	attributes: The pin's properties that will be retreived
 */
int PWM::getPinAttributes(string attributes)
{
	return atoi(readFromFile(this->pwmPinPath, attributes).c_str());	// cat ./period
}

/*
 * Used to write to the PWM files to change the pin's properties
 *
 * path: The path to the PWM Pin
 * filename: The name of the file whose value you want to change
 * value: The value you want to assign to the 'filename'
 */
void PWM::writeToFile(string path, string filename, int value)
{
	ofstream fileStream;
	string fileName = to_string(value);

	fileStream.open((path + filename).c_str());

	if(fileStream.is_open())
	{
		fileStream << fileName;
		fileStream.close();
	}
	else
	{
		perror("open - writeToFile");
		exit(EXIT_FAILURE);
	}
}

/*
 * Used to read from the PWM files
 *
 * path: The path to the PWM pin
 * filename: The name of the file whose value you want to read
 */
string PWM::readFromFile(string path, string filename)
{
	ifstream fileStream;
	string value;

	fileStream.open( (path + filename).c_str());

	if(fileStream.is_open())
	{
		fileStream >> value;
		fileStream.close();
	}
	else
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	return value;
}

void PWM::deployOverlay(string overlay)
{
	ofstream fileStream;

	fileStream.open(this->slotsPath.c_str());

	if(fileStream.is_open())
	{
		fileStream << overlay;
		fileStream.close();
	}
	else
	{
		perror("open - deployOverlay");
		exit(EXIT_FAILURE);
	}
}
