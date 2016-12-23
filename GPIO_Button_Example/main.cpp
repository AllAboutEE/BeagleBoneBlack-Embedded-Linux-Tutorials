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
	GPIO led(49);
	GPIO button(115);

	/* Configure LED */
	led.setPinAttributes("direction", "out");		// Set the LED as an output

	/* Configure button */
	button.setPinAttributes("direction", "in");		// Set the button as an input
	button.setPinAttributes("edge", "rising");		// Set trigger on the rising edge

	cout << "Press the button: " << endl;
	button.edgeListen();							// Wait for ever until a trigger is detected
	led.setPinAttributes("value", "1");				// Turn LED on, on edge trigger
	sleep(1);

	return 0;
}
