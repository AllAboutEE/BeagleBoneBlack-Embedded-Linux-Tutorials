/*
 * This program reads in an analog input signal and prints out its results.
 */

#include<iostream>
#include<unistd.h>

#include "Analog.h"

using namespace std;

int main()
{
	Analog analog;

	cout << "Starting program....." << endl;

	for(int i = 0; i < 5; i++)
	{
		cout << "Analog input: " << analog.getPinAttributes("voltage0") << " out of 4095." << endl;
		sleep(2);
	}

	return 0;
}
