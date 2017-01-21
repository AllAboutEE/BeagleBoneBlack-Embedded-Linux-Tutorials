/*
 * I2C.h
 *
 *  Created on: Aug 29, 2016
 *      Author: calvarez
 */

#ifndef H_I2C_H_
#define H_I2C_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

using namespace std;

namespace MCP9800
{
	enum Registers
	{
		TA_REG     = 0x00,
		CONFIG_REG = 0x01,
		THYST_REG  = 0x02,
		TSET_REG   = 0x03
	};
}

class I2C
{
private:
	unsigned int i2cBus;
	unsigned int i2cSlaveAddress;
	int file;
	string slotsPath;

	void checkVersion();

public:
	I2C(unsigned int bus, unsigned int slaveAddress);
	~I2C();
	void accessI2CAdapter();
	void close();
	void i2cSet(unsigned char registerAddress, unsigned char value);
	unsigned char *i2cGet(unsigned char registerAddress, unsigned int numOfBytes);
	void enableI2C1();

	string version;
};

#endif /* H_I2C_H_ */
