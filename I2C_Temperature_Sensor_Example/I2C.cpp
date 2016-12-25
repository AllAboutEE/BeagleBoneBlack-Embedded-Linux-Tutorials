/*
 * I2C.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: calvarez
 */

#include "I2C.h"

using namespace std;

/*
 * Initialize the bus and slave's address.
 */
I2C::I2C(unsigned int bus, unsigned int slaveAddress) : i2cBus(bus), i2cSlaveAddress(slaveAddress), file(-1)
{
	accessI2CAdapter();
}

/*
 * Destructor, calls the close() function
 */
I2C::~I2C()
{
	close();
}

/*
 * Access an I2C adapter. Must include the file linux/i2c-dev.h
 */
void I2C::accessI2CAdapter()
{
	string swDevice;

	if(this->i2cBus == 0)
	{
		swDevice = "/dev/i2c-0";
	}
	else if(this->i2cBus == 1)
	{
		swDevice = "/dev/i2c-1";
	}
	else
	{
		swDevice = "/dev/i2c-2";
		enableI2C1();
	}

	this->file = ::open(swDevice.c_str(), O_RDWR);
	if( this->file  == -1)
	{
		perror("::open");
		exit(EXIT_FAILURE);
	}

	if(ioctl(this->file, I2C_SLAVE, this->i2cSlaveAddress) == -1)
	{
		perror("ioctl");
		exit(EXIT_FAILURE);
	}
}

/*
 * Close the file descriptor
 */
void I2C::close()
{
	::close(this->file);
	this->file = -1;
}

/*
 * Write a byte to a register.
 *
 * Similar to: i2cset -y 1 <SlaveAddress> <RegisterAddress> <Value>
 *
 * registerAddress: The register that will be modified
 * value: The value that will be stored in the specified register
 */
void I2C::i2cSet(unsigned char registerAddress, unsigned char value)
{
	unsigned char data[2] = {registerAddress, value};

	// Write 2 bytes of data into file
	if(::write(this->file, data, 2) != 2)
	{
		perror("::write");
		exit(EXIT_FAILURE);
	}
}

/*
 * Read a number of bytes (numOfBytes) from a register (registerAddress).
 *
 * This is done by calling the i2cGet function to write/indicate to the file descriptor the register
 * that you will be reading from, followed by a call to ::read() to read the value off of the indicated
 * register and store it in a dynamically allocated unsigned char array.
 *
 * Similar to: i2cget -y 1 <SlaveAddress> <RegisterAddress> w
 *
 * registerAddress: The register who's value/s will be read
 * numOfBytes: The number of bytes to read
 */
unsigned char* I2C::i2cGet(unsigned char registerAddress, unsigned int numOfBytes)
{
	unsigned char* data = new unsigned char[numOfBytes];

	// Write 1 byte of registerAddress into file
	if(::write(this->file, &registerAddress, 1) != 1)
	{
		perror("::write");
		exit(EXIT_FAILURE);
	}

	// Read nuOfReg byte/s into data from file
	if(::read(this->file, data, numOfBytes) != numOfBytes)
	{
		perror("::read");
		exit(EXIT_FAILURE);
	}

	return data;
}

/*
 * Enable the second I2C bus (I2C1 "/dev/i2c-2")
 *
 * Similar to: echo BB-I2C1 > /sys/devices/bone_capemgr.9/slots
 */
void I2C::enableI2C1()
{
	ofstream fileStream;
	string overlay = "BB-I2C1";

	fileStream.open(this->slotsPath.c_str());

	if(fileStream.is_open())
	{
		fileStream << overlay;
		fileStream.close();
	}
	else
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}
