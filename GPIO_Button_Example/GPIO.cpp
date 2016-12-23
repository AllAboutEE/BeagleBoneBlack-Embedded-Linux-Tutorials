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
	   perror("GPIO: read failed to open file ");
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

/*
 * Listen for an event (that causes a change in edge) to occur.
 */
void GPIO::edgeListen()
{
	setPinAttributes("direction", "in");

	int fd;
	int epfd;
	struct epoll_event event;

	// Create an epoll instance and return a file descriptor referring to that instance
	epfd = epoll_create(1);
    if(epfd == -1)
    {
	   perror("epoll_create");
	   exit(EXIT_FAILURE);
    }

    // Create a new open file description, and return a new file descriptor for it.
    //  - The open file description records the file offset and the file status flags.
    //  - The file description is a reference to an open file description.
    fd = open((this->gpioPinPath + "value").c_str(), O_RDONLY | O_NONBLOCK);	// An application that employs EPOLLET flag should use nonblocking fds.
    if(fd == -1)
    {
       perror("open");
       exit(EXIT_FAILURE);
    }

    event.events = EPOLLIN | EPOLLET;	// Make the associated file available for read, and set the Edge Triggered behavior.
    event.data.fd = fd;	// Attach the file descriptor

    // Control an epoll file descriptor (epfd), by requesting that the operation (EPOLL_CTL_ADD) be
    // performed on the target file descriptor (fd).
    //  - EPOLL_CTL_ADD: Add the target file descriptor (fd) to the epoll file descriptor (epfd) and associate
    //                   the event (event) with the internal file linked to the file descriptor (fd).
    //  - The event describes the object linked to the file descriptor (fd)
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
       perror("epoll_ctl");
       exit(EXIT_FAILURE);
    }

    int triggerCount = 0;
    int timeout = -1;		// Setting timeout = -1 causes epoll_wait to wait indefinitely.
    int maxEvents = 1;		// Lenght of the struct epoll_events array/variable pointed to by *events

	while(triggerCount <= 1)
	{
		// Wait for events on the epoll file descriptor (epfd) for a maximum time of (timeout) milliseconds.
		if(epoll_wait(epfd, &event, maxEvents, timeout) == -1)
		{
			perror("epoll_wait");
			close(fd);
			exit(EXIT_FAILURE);
		}
		else
		{
			triggerCount++;		// Increment rising edge trigger count
		}
	}

    close(fd);
}
