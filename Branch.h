#include <iostream>
#include <iostream>
#include <string>

using namespace std;

/**
    Class to represent each individual line (branch)
    programAddr, behavior, targetAddr
        (0x421c40 NT 0x421c54)
**/

class Branch
{
public:
    // Constructor
    Branch(unsigned long long programAddr, int behavior, unsigned long long targetAddr)
        : programAddr(programAddr), behavior(behavior), targetAddr(targetAddr) {}

    // Getters
    unsigned int getProgramAddr() const { return programAddr; }
    int getBehavior() const { return behavior; }
    unsigned int getTargetAddr() const { return targetAddr; }

private:
    unsigned long long programAddr;
    int behavior;
    unsigned long long targetAddr;
};