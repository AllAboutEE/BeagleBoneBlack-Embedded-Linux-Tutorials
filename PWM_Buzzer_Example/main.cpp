#include <iostream>
#include <unistd.h>
#include <math.h>

#include "PWM.h"

using namespace std;

int main()
{
	PWM buzzer("pwm_test_P9_22.12");

	buzzer.setPinAttributes("run", 1);
	buzzer.setPinAttributes("duty", (unsigned int) 50 * 100);

	for(int i = 0; i < 8000; i = i+100)
	{
		buzzer.setPinAttributes("period", (unsigned int) ((1.0/float(i)) * pow(10,9)));
		usleep((0.1)*1000000);
	}

	cout << "  Period: " << buzzer.getPinAttributes("period") << endl;
	cout << "    Duty: " << buzzer.getPinAttributes("duty") << endl;
	cout << "Polarity: " << buzzer.getPinAttributes("polarity") << endl;
	cout << "     Run: " << buzzer.getPinAttributes("run") << endl;

	buzzer.setPinAttributes("run", 0);

	return 0;
}
