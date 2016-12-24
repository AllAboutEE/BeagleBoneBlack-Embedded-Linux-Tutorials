OBJS = main.o Analog.o

executable : $(OBJS)
	g++ -o RUN_ME $(OBJS)

main.o : main.cpp Analog.h
	g++ -std=c++11 -c main.cpp

Analog.o : Analog.h Analog.cpp
	g++ -std=c++11 -c Analog.cpp

.PHONY : clean
clean :
	rm $(OBJS) ./RUN_ME
