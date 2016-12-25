OBJS = main.o I2C.o

executable : $(OBJS)
	g++ -o RUN_ME $(OBJS)

main.o : main.cpp I2C.h
	g++ -std=c++11 -c main.cpp

I2C.o : I2C.h I2C.cpp
	g++ -std=c++11 -c I2C.cpp

.PHONY : clean
clean :
	rm $(OBJS) ./RUN_ME
