all: predictor

predictor: main.o Predictor.o 
	g++ main.o Predictor.o -o predictor

main.o: main.cpp Predictor.h
	g++ -c main.cpp

Predictor.o: Predictor.cpp Predictor.h
	g++ -c Predictor.cpp

clean: rm -f *.o predictor