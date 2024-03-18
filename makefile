all: predictor

predictor: main.o Predictor.o 
	g++ -g main.o Predictor.o -o predictor

main.o: main.cpp Predictor.h
	g++ -g -c main.cpp

Predictor.o: Predictor.cpp Predictor.h Branch.h
	g++ -g -c Predictor.cpp

clean: 
	rm -f *.o *.txt predictor