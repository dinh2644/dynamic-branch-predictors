#pragma once
#include <iostream>
#include <iostream>
#include <string>

using namespace std;

/**
    Class to hold a branch's program counter,
    outcome (false = NT, true = T), and target address 
    Ex. 0x421c40 NT 0x421c54   
**/

class Branch{
    public:
        // Constructor
         Branch(unsigned int programAddr, bool behavior, unsigned int targetAddr)
        : programAddr(programAddr), behavior(behavior), targetAddr(targetAddr) {}

        // Getters
        unsigned int getProgramAddr() const {return programAddr;}
        bool getBehavior() const {return behavior;}
        unsigned int getTargetAddr() const {return targetAddr;}

    private:
        unsigned int programAddr;
        bool behavior;
        unsigned int targetAddr;

};