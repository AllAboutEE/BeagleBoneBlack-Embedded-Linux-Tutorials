#include <iostream>
#include <unistd.h>
#include <math.h>

#include "PWM.h"

using namespace std;

void pwmWheezy(PWM pwm);
void pwmJessie(PWM pwm);

int main()
{
//	PWM buzzer("bone_pwm_P9_22", "pwm_test_P9_22.12");	//Uncomment for wheezy

	PWM buzzer("BB-PWM0", "pwm0");	//Uncomment for jessie


	if(buzzer.version.compare("wheezy") == 0)
	{
		pwmWheezy(buzzer);
	}
	else if(buzzer.version.compare("jessie") == 0)
	{
		pwmJessie(buzzer);
	}

	return 0;
}

void pwmWheezy(PWM buzzer)
{
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

	return;
}

void pwmJessie(PWM buzzer)
{

	buzzer.setPinAttributes("enable", 1);
	buzzer.setPinAttributes("period", (unsigned int) 10000000);
	buzzer.setPinAttributes("duty_cycle", (unsigned int) 5000000);

	cout << "  Period: " << buzzer.getPinAttributes("period") << endl;
	cout << "    Duty: " << buzzer.getPinAttributes("duty_cycle") << endl;
	cout << "Polarity: " << buzzer.getPinAttributes("polarity") << endl;
	cout << "  Enable: " << buzzer.getPinAttributes("enable") << endl;

	sleep(5);

	buzzer.setPinAttributes("enable", 0);

	return;
}
