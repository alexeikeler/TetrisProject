#include "./ParseArguments.h"
#include <iostream>
#include <getopt.h>

void Parser::printHelp()
{
    std::cout <<
    "--level <n>:                       Start game with level n.\n"
    "--leftRotationKey <letter>:        Set left rotation key to <letter>\n"
    "--rightRotationKey <letter>:       Set right rotation key to <letter>\n"
    "--help:                            Show help\n";
    exit(1);
}

void Parser::parseArguments(int argc, char **argv)
{
    // This C-style string tells us that we have 4 arguments.
    // : means that we are awaiting for some values after l, r and b.
    const char* const shortOptions = "l:r:b:h";

    // Short arguments are kind of cryptic, so I've decided to add long arguments.
    const option longOPtions[] = {
        {"level", optional_argument, nullptr, 'b'},
        {"leftRotationKey", optional_argument, nullptr, 'l'},
        {"rightRotationKey", optional_argument, nullptr, 'r'},
        {"help", optional_argument, nullptr, 'h'}
    };

    // Parse all arguments
    while(true)
    {
        // Get current
        const auto option = getopt_long(argc, argv, shortOptions, longOPtions, nullptr);

        // If option == -1 we have parsed everything        
        if(option == -1)
        {
            break;
        }

        switch (option)
        {
            case 'b':
                level = std::stoi(optarg);
                break;
            
            case 'l':
                leftRotationKey = *optarg;
                break;
            case 'r':
                rightRotationKey = *optarg;
                break;
            
            case 'h':
                printHelp();
            
            default:
                break;
        }
    }       

}