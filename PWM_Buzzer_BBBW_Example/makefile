OBJS = main.o PWM.o

executable : $(OBJS)
	g++ -o RUN_ME $(OBJS)

main.o : main.cpp PWM.h
	g++ -std=c++11 -c main.cpp

PWM.o : PWM.h PWM.cpp
	g++ -std=c++11 -c PWM.cpp

.PHONY : clean
clean :
	rm $(OBJS) ./RUN_ME
