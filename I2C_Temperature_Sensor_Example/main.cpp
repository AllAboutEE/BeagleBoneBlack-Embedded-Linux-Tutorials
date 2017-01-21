/*
 * This program controls the CONFIG and TA registers of the Temperature Sensor (MCP9800)
 * to configure the temperature accuracy and read in the temperature.
 */

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "I2C.h"

using namespace std;
using namespace MCP9800;

void printTempC(unsigned int at);
void printTempF(unsigned int at);
void printRawData(unsigned char* data, unsigned int numOfBytes);

int main()
{
	/*
	 * Jessie:
	 * P9_18 and P9_17 are controlled by bus 1 (BB-I2C1)
	 * P9_20 and P9_19 are controlled by bus 2 (default)
	 *
	 * Wheezy:
	 * P9_18 and P9_17 are controlled by bus 2 (BB-I2C1)
	 * P9_20 and P9_19 are controlled by bus 1 (default)
	 */
	I2C tc77(1, 0x4D);			//For BBB wheezy version use bus 2, see note above e.g. tc77(2, 0x4D)
	unsigned char* data = NULL;
	unsigned int ambientTemp = 0;

	/*
	 * Step 1: Write to the 0x01 (CONFIG) register the value of 0x60.
	 */
	tc77.i2cSet(CONFIG_REG, 0x60);

	/*
	 * Step 2: Write to the 0x00 (TA) register the (dummy) value of 0x00.
	 * Because data is outputted from the previously-specified register,
	 * to read values from the TA register, this register first needs to
	 * be selected. The TA register is selected once something is written
	 * to it. Note: The TA register is only a read register.
	 */
	tc77.i2cSet(TA_REG, 0x00);

	sleep(1);

	for(int i = 0; i < 10; i++)
	{
		/*
		 * Step 3: Store the values read in from the TA register.
		 * The data pointer contains two values (returned by readRegisters) stored in
		 * data[0] (MSB) and data[1] (LSB). According to the data sheet the the 4 least
		 * significant bits are 0 by default and should not be taken into account when
		 * computing the temperature.
		 */

		data = tc77.i2cGet(0x00, 2);		// Read 2 registers starting from 0x00
		printRawData(data, 2);

		/*
		 * Step 4: Combine the 2 bytes stored in data.
		 *
		 * 1) Shift the data[0] to the left by 4 to get the following <0b NNNN NNNN 0000>
		 *
		 * 2)Shift the data[1] to the right by 4 to get the following <0b NNNN>. AND the
		 *   two pieces of data (<0b NNNN NNNN 0000> and <0b NNNN>) to get <0b NNNN NNNN NNNN>
		 */
		ambientTemp = *(data + 0) << 4;
		ambientTemp = (ambientTemp | (*(data + 1) >> 4) );

		printTempC(ambientTemp);
		printTempF(ambientTemp);
		cout << endl;

		sleep(1);
	}

	return 0;
}

/*
 * Print out the ambient temperature in Celsius.
 *
 * ambientTemp: The value to be used to compute the ambient temperature
 */
void printTempC(unsigned int ambientTemp)
{
	cout << "Ambient Temperature (C) = " << dec << (ambientTemp * pow(2,-4)) << endl;
}

/*
 * Print out the ambient temperature in Fahrenheit.
 *
 * ambientTemp: The value to be used to compute the ambient temperature

 */
void printTempF(unsigned int ambientTemp)
{
	cout << "Ambient Temperature (F) = " << dec << ( (ambientTemp * pow(2, -4)) * (9.0/5.0) + 32 ) << endl;
}

/*
 * Print the values in a register(s)
 *
 * data: The values stored in the specified registers, returned by readRegisters() function
 * numOfBytes: The number of bytes that will be read
 */
void printRawData(unsigned char* data, unsigned int numOfBytes)
{
	for(unsigned int i = 0; i < numOfBytes; i++)
	{
		cout << "byte[" <<  i << "]: " << hex << (int)(*(data + i)) << "\n";
	}
}

