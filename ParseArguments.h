#pragma once
#include <getopt.h>

// Parser class for parsing command line arguments.
class Parser{
    public:
        void printHelp();
        void parseArguments(int argc, char **argv);
        int getLevel() {return level;}
        char getLeftRotationKey() {return leftRotationKey;}
        char getRightRotationKey() {return rightRotationKey;}
        
    private:
        int level = 0;
        char leftRotationKey = 'a';
        char rightRotationKey = 's';
};