/*
 * SPI.h
 *
 *  Created on: Sep 10, 2016
 *      Author: calvarez
 */

#ifndef H_SPI_H_
#define H_SPI_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <fstream>


using namespace std;

/*
 * BB-SPIDEV0
 * CS   - P9_17
 * MOSI - P9_18
 * MISO - P9_21
 * CLK  - P9_22
 *
 * BB-SPIDEV1
 * CS   - P9_28
 * MOSI - P9_30
 * MISO - P9_29
 * CLK  - P9_31
 */
class SPI
{
private:
	unsigned int spiBus;
	unsigned int spiChipSelect;
	int spiFileDescriptor;
	int mode;
	uint8_t bitsPerWord;
	uint32_t clkFreq;
	uint16_t delay;
	const string slotsPath = "/sys/devices/bone_capemgr.9/slots";


	void openSpiDevice(string fileName);
	void close();
	void transferData(unsigned char* outgoingData, unsigned char* incomingData, int lenght); // Note: if unsigned char receive = 0x00 is passed as a parameter a segfault will occur unless this function is dclared as virtual
	void deployOverlay(string overlay);

public:
	SPI(unsigned int bus, unsigned int chipSeclect);
	~SPI();
	unsigned char* writeReadRegister(unsigned char* send, unsigned int sizeOfMsg);
	void frequency(uint32_t hz);
	void format(uint8_t bits, int mode);
};

#endif /* H_SPI_H_ */
