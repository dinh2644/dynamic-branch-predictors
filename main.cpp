/* Example
 * Parsing Branch Traces
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Predictor.h"

using namespace std;

int main(int argc, char *argv[])
{
  Predictor p;

  p.readFile("short_trace1.txt");
  // p.alwaysTaken();
  // p.alwaysNotTaken();
  p.bimodalSingleBit(32);

  return 0;
}
