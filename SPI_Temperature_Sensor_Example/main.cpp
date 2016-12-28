#include <iostream>
#include <unistd.h>
#include <math.h>

#include "SPI.h"

using namespace std;

void printTemperature(SPI device, unsigned char* tempMessage, unsigned int tempMessageSize);
void printManufacturerID(SPI device, unsigned char* mIdMessage, unsigned int idMessageSize);
void printRawData(unsigned char* rawTempData, unsigned int numOfBytes);
void printTempC(unsigned char* rawTempData);
void printTempF(unsigned char* rawTempData);
double getTempC(unsigned char* rawTempData);

int main()
{
	SPI tc77(2,0);

	tc77.format(8,SPI_MODE_0);
	tc77.frequency(100000);

	usleep(400000);

	unsigned char getMIdMessage[] = {0x00, 0x00,	// Step 2: Read 16 bits of temperature data from the Temperature register.
						  	  	    0xFF, 0xFF, 	// Step 3: Write 16 bits of data to the Configuration register to enter Shutdown mode.
									0x00, 0x00, 	// Step 4: Read the 16 bits from the manufacturer's ID register to verify that the sensor is a Microchip device.
									0x00, 0x00};	// Step 5: Write 8 to 16 bits of data (0x0000) to enter Continuous Conversion Mode.
	unsigned int mIdMessageSize = sizeof(getMIdMessage)/sizeof(getMIdMessage[0]);

	unsigned char getTempMessage[] = {0x00, 0x00};
	unsigned int tempMessageSize = sizeof(getTempMessage)/sizeof(getTempMessage[0]);

	printTemperature(tc77, getTempMessage, tempMessageSize);
//	printManufacturerID(tc77, getMIdMessage, mIdMessageSize);

	return 0;
}

/*
 * Print out the ambient temperature.
 *
 * device: The SPI device/object being used
 * mIdMessage: The data that will be sent over the SPI bus to retreive the Manufacturer ID
 * idMessageSize: The number of bytes that will be sent over the SPI bus
 */
void printTemperature(SPI device, unsigned char* tempMessage, unsigned int tempMessageSize)
{
	unsigned char* tempRegisterData = NULL;

	for(;;)
	{
		tempRegisterData = device.writeReadRegister(tempMessage, tempMessageSize);

		while( (tempRegisterData[1] & 0x04) == 0)
		{
			usleep(400000);
			tempRegisterData = device.writeReadRegister(tempMessage, tempMessageSize);
		}

		printRawData(tempRegisterData, tempMessageSize);
		printTempC(tempRegisterData);
		printTempF(tempRegisterData);
		cout << endl;

		sleep(1);
	}

	delete tempRegisterData;
}

/*
 * Print out the manufacturer ID (0x54).
 *
 * device: The SPI device/object being used
 * mIdMessage: The data that will be sent over the SPI bus to retreive the Manufacturer ID
 * idMessageSize: The number of bytes that will be sent over the SPI bus
 */
void printManufacturerID(SPI device, unsigned char* mIdMessage, unsigned int idMessageSize)
{
	unsigned char* mIdRegisterData = device.writeReadRegister(mIdMessage,idMessageSize);

	printRawData(mIdRegisterData, idMessageSize);
	cout << endl;

	delete mIdRegisterData;
}

/*
 * Print out the values in a register(s).
 *
 * rawTempData: The data to print
 * numOfBytes: The number of bytes that will be printed
 */
void printRawData(unsigned char* rawData, unsigned int numOfBytes)
{
	cout << "Register data: ";

	for(int i=0; i<(int)numOfBytes; i++)
	{
		cout << setw(2) << setfill('0') << hex << (int)(*(rawData+i)) << " ";
	}

	cout << endl;
}

/*
 * Print out the ambient temperature in Celsius.
 *
 * rawTempData: The data from the TEMP register that will be used to print the temperature in Celcius.
 */
void printTempC(unsigned char* rawTempData)
{
	cout << "Ambient Temperature (C) = " << getTempC(rawTempData) << endl;
}

/*
 * Print out the ambient temperature in Fahrenheit.
 *
 * rawTempData: The data from the TEMP register that will be used to print the temperature in Fahrenheit.
 */
void printTempF(unsigned char* rawTempData)
{
	double tempC = getTempC(rawTempData);

	cout << "Ambient Temperature (F) = " << dec << ( tempC * (9.0/5.0) + 32 ) << endl;
}

/*
 * Get the temperature in celcius by converting the raw data.
 *
 * rawTempData: The data from the TEMP register that will be used to calculate the temperature in Celcius.
 */
double getTempC(unsigned char* rawTempData)
{
	unsigned int temperature = ((rawTempData[0] << 8) | rawTempData[1]) >> 3;

	return (temperature*0.0625f);
}
