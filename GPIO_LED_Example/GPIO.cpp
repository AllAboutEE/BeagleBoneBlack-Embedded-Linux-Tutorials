#include "GPIO.h"

/*
 * Enable the chose gpio pin.
 */
GPIO::GPIO(int pin) : gpioPin(pin)
{
	string gpioPinName = "gpio" + to_string(this->gpioPin);

	this->gpioPinPath = gpioPath + gpioPinName + "/";

	setUnsetGPIO("export");
	sleep(1);
}

/*
 * Disable the gpio pin once the program ends.
 */
GPIO::~GPIO()
{
	setUnsetGPIO("unexport");
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
	ofstream fileStream;
	fileStream.open((path + fileName).c_str());

	if(fileStream.is_open())
	{
		// The file was successfully opened
		fileStream << value;
		fileStream.close();
	}
	else
	{
		// The file was not successfully opened
		perror("open");
		exit(EXIT_FAILURE);
	}
}

/*
 * Read from one of the files located under /sys/class/gpio/
 *
 * path: The location of the file that will be read.
 * fileName: The file that will be read.
 */
string GPIO::readFromFile(string path, string filename)
{
   ifstream fileStream;
   string fileContent;

   fileStream.open((path + filename).c_str());

   if(fileStream.is_open())
   {
	   //The file was successfully opened
	   getline(fileStream,fileContent);
	   fileStream.close();
   }
   else
   {
	   // The file was not successfully opened
	   perror("open");
	   exit(EXIT_FAILURE);
   }

   return fileContent;
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
 * Modify the file associated with the gpio pin (e.g. value, edge, direction).
 *
 * fileName: The file that will be modified.
 * value: The value that will be written into fileName.
 */
void GPIO::setPinAttributes(string fileName, string value)
{
	writeToFile(this->gpioPinPath, fileName, value);
}

/*
 * Get the contents of a file associated with the gpio pin (e.g. value, edge, direction).
 *
 * fileName: The file who's contents will be read
 */
string GPIO::getPinAttributes(string fileName)
{
	return readFromFile(this->gpioPinPath, fileName);
}
