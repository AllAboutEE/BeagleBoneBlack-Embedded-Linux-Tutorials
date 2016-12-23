/*
 * This program switches the value of a gpio pin between HIGH and LOW.
 */

#include<iostream>
#include<unistd.h>

#include "GPIO.h"

using namespace std;

int main()
{
	GPIO led(49);

	led.setPinAttributes("direction", "out");

	for (int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			led.setPinAttributes("value", "1");
			usleep(100000); // Sleep for 0.1 seconds

			led.setPinAttributes("value", "0");
			usleep(100000); // Sleep for 0.1 seconds
		}

		cout << "Value: " << led.getPinAttributes("value") << endl;
		led.setPinAttributes("value", "1");
		cout << "Value: " << led.getPinAttributes("value") << endl << endl;

		sleep(1);	// Sleep for 1 second
	}

	   return 0;
}
