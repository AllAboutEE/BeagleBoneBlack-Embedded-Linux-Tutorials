#include <iostream>
#include <unistd.h>
#include <math.h>

#include "PWM.h"

using namespace std;

int main()
{
	PWM buzzer("BB-PWM0", "pwm0");

	buzzer.setPinAttributes("enable", 1);
	buzzer.setPinAttributes("period", (unsigned int) 10000000);
	buzzer.setPinAttributes("duty_cycle", (unsigned int) 5000000);

	cout << "  Period: " << buzzer.getPinAttributes("period") << endl;
	cout << "    Duty: " << buzzer.getPinAttributes("duty_cycle") << endl;
	cout << "Polarity: " << buzzer.getPinAttributes("polarity") << endl;
	cout << "  Enable: " << buzzer.getPinAttributes("enable") << endl;

	sleep(5);

	buzzer.setPinAttributes("enable", 0);

	return 0;
}
