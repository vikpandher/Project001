///
////////////////////////////////////////////////////////////////////////////////

#include "Logger.h"



int main(int argc, char** argv)
{
    Project001::Logger* loggerPtr = new Project001::Logger();
    loggerPtr->message("Decimals: %d %ld", 1977, 650000L);
    
    return 0;
}