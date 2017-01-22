#include "SPI.h"

using namespace std;

/*
 * Constructor
 * Create the path to the spi file that will be modified (/dev/spidevX.Y).
 *
 * bus: The SPI bus number (X)
 * chipSelect: the SPI chip select (Y)
 */
SPI::SPI(unsigned int bus, unsigned int chipSelect) :
		spiBus(bus),
		spiChipSelect(chipSelect),
		spiFileDescriptor(-1),
		mode(SPI_MODE_0),
		bitsPerWord(8),
		clkFreq(100000),
		delay(0)
{
	string spiPath = "/dev/spidev" + to_string(this->spiBus) + "." + to_string(this->spiChipSelect);

	deployOverlay("BB-SPIDEV1");
	usleep(250000); // Delay to allow the setup of sysfs

	openSpiDevice(spiPath);
}

/*
 * Destructor, calls the close() function
 */
SPI::~SPI()
{
	close();
}

/*
 * Deploys the DTO necessary to work with SPI pins.
 *
 * overlay: The name of the DTO to be deployed.
 */
void SPI::deployOverlay(string overlay)
{
	ofstream mySpiFile;

	mySpiFile.open(this->slotsPath.c_str());

	if(mySpiFile.is_open())
	{
		mySpiFile << overlay;
		mySpiFile.close();
	}
	else
	{
		perror("open - deployOverlay");
		exit(EXIT_FAILURE);
	}
}

/*
 * Opens/Creates a file connection to the SPI device.
 * Create an open file descriptor (fd) for Reading (RD) and Writing (WR), and set up the mode, bits per word and frequency.
 *
 * fileName: The file that will be modified
 */
void SPI::openSpiDevice(string fileName)
{
	this->spiFileDescriptor = ::open(fileName.c_str(), O_RDWR);	// Create an open fd (for RD and WR) that refers to fileName

	if( this->spiFileDescriptor == -1)
	{
		// The fd was not successfully created
		perror("::open");
		exit(EXIT_FAILURE);
	}

	format(this->bitsPerWord, this->mode);
	frequency(this->clkFreq);
}

/*
 * Close the SPI bus by closing the file descriptor.
 */
void SPI::close()
{
	::close(this->spiFileDescriptor);
	this->spiFileDescriptor = -1;
}

/*
 * Writes/Reads data to/from a register.
 * send: An array of unsigned char values that will be sent over the SPI bus
 * sizeofMsg: The number of messages (bytes) that will be sent over the SPI bus
 *
 * return: The data received from the SPI device
 */
unsigned char* SPI::writeReadRegister(unsigned char* send, unsigned int sizeOfMsg)
{
	unsigned char* receive = new unsigned char[sizeOfMsg];
	memset(receive, 0, sizeOfMsg);

	transferData(send, receive, sizeOfMsg);

	return receive;
}

/*
 * Transfer data between the master and the slave device.
 * An array (incomingData) of equal size to that being sent (outgoingData) is returned.
 *
 * outgoingData: the array of data to send to the SPI device
 * incomingData: the array of data to receive from the SPI device
 * length: the length in bytes of the data being sent/received
 */
void SPI::transferData(unsigned char* outgoingData, unsigned char* incomingData, int length)
{
	struct spi_ioc_transfer spiTransfer;

	spiTransfer.tx_buf = (unsigned long long) outgoingData;	// Holds pointer to userspace buffer with transmit data, or null.
	spiTransfer.rx_buf = (unsigned long long) incomingData;	// Holds pointer to userspace buffer for receive data, or null.
	spiTransfer.len = length;								// Length of tx and rx buffers, in bytes.
	spiTransfer.speed_hz = this->clkFreq;					// Temporary override of the device's bitrate.
	spiTransfer.delay_usecs = this->delay;					// If nonzero, how long to delay after the last bit transfer before optionally deselecting the device before the next transfer.
	spiTransfer.bits_per_word = this->bitsPerWord;			// Temporary override of the device's wordsize.

	spiTransfer.cs_change = 1;
	spiTransfer.tx_nbits = 0x08;
	spiTransfer.rx_nbits = 0x08;
	spiTransfer.pad = 0x00;

	bool setupStatus = (ioctl(this->spiFileDescriptor, SPI_IOC_MESSAGE(1), &spiTransfer) == -1);

	if(setupStatus)
	{
		perror("ioctl(Message)");
		exit(EXIT_FAILURE);
	}
}

/*
 * Set the SPI bus clock frequency.
 *
 * hz: The speed in Hz
 */
void SPI::frequency(uint32_t hz)
{
	this->clkFreq = hz;

	bool setupStatus = (ioctl(this->spiFileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &this->clkFreq) == -1) |	// This ioctl() request allows the override of the device's current settings for WR_MAX_SPEED
					   (ioctl(this->spiFileDescriptor, SPI_IOC_RD_MAX_SPEED_HZ, &this->clkFreq) == -1);		// This ioctl() request allows the override of the device's current settings for RD_MAX_SPEED

	if(setupStatus)
	{
		perror("ioctl(Hz)");
		exit(EXIT_FAILURE);
	}
}

/*
 * Set the mode and number of bits per word of the SPI bus.
 *
 * bits: The number of bits per word
 * mode: The mode the SPI bus will be set to
 */
void SPI::format(uint8_t bits, int mode)
{
	this->bitsPerWord = bits;
	this->mode = mode;

	bool setupStatus = (ioctl(this->spiFileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &this->bitsPerWord) == -1) |
					   (ioctl(this->spiFileDescriptor, SPI_IOC_RD_BITS_PER_WORD, &this->bitsPerWord) == -1) |
					   (ioctl(this->spiFileDescriptor, SPI_IOC_WR_MODE,          &this->mode) == -1) |
					   (ioctl(this->spiFileDescriptor, SPI_IOC_RD_MODE,          &this->mode) == -1);

	if(setupStatus)
	{
		perror("ioctl(Bits/Mode)");
		exit(EXIT_FAILURE);
	}
}
