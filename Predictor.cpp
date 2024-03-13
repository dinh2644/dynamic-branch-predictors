#include <iostream>
#include<fstream>
#include<sstream>
#include<string> 

#include "Predictor.h"

using namespace std;

void Predictor::readFile(string fileName){
// Temporary variables
  unsigned long long addr;
  string behavior, line;
  unsigned long long target;

  // Open file for reading
  ifstream infile(fileName);
  if(!infile){
    cout << "No file found!" << endl;
    exit(1);
  }

  // The following loop will read a line at a time
  while(getline(infile, line)) {
    // Now we have to parse the line into it's two pieces
    stringstream s(line);
    s >> std::hex >> addr >> behavior >> std::hex >> target;
    // Now we can output it
    cout << addr;
    if(behavior == "T") {
      cout << " -> taken, ";
    }else {
      cout << " -> not taken, ";
    }
	cout << "target=" << target << endl;
  }
}

void Predictor::alwaysTaken(){
    // implement code
}

void Predictor::alwaysNotTaken(){
    // implement code
}