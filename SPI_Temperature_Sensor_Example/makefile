OBJS = main.o SPI.o

executable : $(OBJS)
	g++ -o RUN_ME $(OBJS)

main.o : main.cpp SPI.h
	g++ -std=c++11 -c main.cpp

SPI.o : SPI.h SPI.cpp
	g++ -std=c++11 -c SPI.cpp

.PHONY : clean
clean :
	rm $(OBJS) ./RUN_ME
