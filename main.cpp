/* Example
 * Parsing Branch Traces
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

#include "Predictor.h"

using namespace std;

int main(int argc, char *argv[]) {
  Predictor p;

  p.readFile("test_input.txt");
  
  return 0;
}
