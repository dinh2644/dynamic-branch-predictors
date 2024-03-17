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
  // p.bimodalSingleBit(2048);
  // p.bimodal2Bit(1024);
  // p.gshare(2048, 11);
  p.tournament(2048, 2048, 2048, 11);

  return 0;
}
