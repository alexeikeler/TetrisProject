// Copyright: 2024 by Ioan Oleksii Kelier keleralexei@gmail.com
// Code snippets from the lectures where used

#pragma once
#include <getopt.h>

// Parser class for parsing command line arguments.
class Parser{
    public:
        // Print help
        void printHelp();
        // Method for parsing. Needs argc and **argv from main.
        void parseArguments(int argc, char **argv );
        
        // Get data after parsing arguments.
        int getLevel() {return level;}
        char getLeftRotationKey() {return leftRotationKey;}
        char getRightRotationKey() {return rightRotationKey;}
        
    private:
        // Default values.
        int level = 0;
        char leftRotationKey = 'a';
        char rightRotationKey = 's';
};