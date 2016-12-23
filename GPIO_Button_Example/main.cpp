/*
 * This program prompts the user to press the button. When the button is pressed, the pin
 * connected to the LED will be set to HIGH.
 */

#include<iostream>
#include<unistd.h>

#include "GPIO.h"

using namespace std;

int main()
{
	GPIO gpioOut(49);	// LED
	GPIO gpioIn(115);	// Button

	gpioOut.setPinAttributes("direction", "out");	// Set the LED as an output

	gpioIn.setPinAttributes("direction", "in");		// Set the button as an input
	gpioIn.setPinAttributes("edge", "rising");		// Set trigger on the rising edge

	cout << "Waiting on button press: " << endl;

	gpioIn.edgeListen();							// Wait for ever until a trigger is detected
	gpioOut.setPinAttributes("value", "1");			// Turn LED on, on edge trigger
	sleep(1);

	return 0;
}
