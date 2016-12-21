#include "GPIO.h"

/*
 * Enable the chose gpio pin.
 */
GPIO::GPIO(int pin) : gpioPin(pin)
{
	string gpioPinName = "gpio" + to_string(this->gpioPin);

	this->gpioPinPath = gpioPath + gpioPinName + "/";

	setUnsetGPIO("export");
	usleep(1000000);
}

/*
 * Disable the gpio pin once the program ends.
 */
GPIO::~GPIO()
{
	setUnsetGPIO("unexport");
}

/*
 * Modify the files associated with the gpio pin (e.g. value, edge, direction).
 *
 * fileName: The file that will be modified.
 * value: The value that will be written into fileName.
 */
void GPIO::setPinAttributes(string fileName, string value)
{
	writeToFile(this->gpioPinPath, fileName, value);
}

/*
 * Write to the export or unexport files to enable or disable a gpio pin.
 *
 * filename: The file that will be modified (export or unexport).
 */
void GPIO::setUnsetGPIO(string fileName)
{
	writeToFile(this->gpioPath, fileName, to_string(this->gpioPin));
}

/*
 * Write to one of the files located under /sys/class/gpio/
 *
 * path: The location of the file that will be modified.
 * fileName: The file that will be modified.
 * value: The value that will be written into fileName.
 */
void GPIO::writeToFile(string path, string fileName, string value)
{
	ofstream fileSystem;
	fileSystem.open((path + fileName).c_str());

	if(fileSystem.is_open())
	{
		// The file was successfully opened
		fileSystem << value;
		fileSystem.close();
	}
	else
	{
		// The file was not successfully opened
		perror("Error");
	}
}
