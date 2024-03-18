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
  if (argc != 3)
  {
    cerr << "Must have 3 arguments." << endl;
    return 1;
  }

  string inputFileName = argv[1];
  string outputFileName = argv[2];

  int tableSizes[7] = {16, 32, 128, 256, 512, 1024, 2048};
  int numTableSizes = sizeof(tableSizes) / sizeof(tableSizes[0]);

  Predictor p;
  p.readFile(inputFileName);
  p.alwaysTaken();
  p.alwaysNotTaken();
  for (int i = 0; i < numTableSizes; i++)
  {
    p.bimodalSingleBit(tableSizes[i]);
  }
  for (int i = 0; i < numTableSizes; i++)
  {
    p.bimodal2Bit(tableSizes[i]);
  }
  for (int i = 3; i < 12; i++)
  {
    p.gshare(2048, i);
  }
  p.tournament(2048, 2048, 2048, 11);
  p.branchTargetBuffer(512);
  p.writeFile(outputFileName);

  return 0;
}
