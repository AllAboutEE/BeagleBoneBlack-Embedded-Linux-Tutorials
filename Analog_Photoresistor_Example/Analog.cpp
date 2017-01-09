#include "Analog.h"

using namespace std;

/*
 * Constructor
 */
Analog::Analog(void)
{
	this->overlay = "BB-ADC";

	setAnalog();

	usleep(250000); // Delay to allow the setup of sysfs
}

/*
 * Destructor
 */
Analog::~Analog()
{

}

/*
 * Get the contents of a file associated with the analog pin.
 *
 * fileName: The file who's contents will be read
 */
int Analog::getPinAttributes(string fileName)
{
	return readFromFile(fileName);
}

/*
 * Read the analog input voltage, and return the value read.
 * This is done by reading from the file: devicesPath/iio:device0/in_voltage_<number>_raw
 *
 * fileName: The file who's contents will be read
 */
int Analog::readFromFile(string fileName)
{
	ifstream fileStream;
	string filePath;
	int fileContent;

	filePath = this->devicesPath + "/iio:device0/in_" + fileName + "_raw";
	fileStream.open(filePath.c_str());

	if(fileStream.is_open())
	{
		fileStream >> fileContent;	// Reads in the value stored in the opened file and stores it inside the variable fileContent. Same as: cat in_voltage<number>_raw
		fileStream.close();
	}
	else
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	return fileContent;
}

/*
 * Enable analog inputs.
 * This is done by loading the analog (BB-ADC) DTO.
 */
void Analog::setAnalog()
{
	ofstream fileStream;

	fileStream.open(this->slotsPath.c_str());

	if(fileStream.is_open())
	{
		fileStream << this->overlay;	//same as: echo BB-ADC > /sys/devices/bone_capemgr.9/slots
		fileStream.close();
	}
	else
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}
