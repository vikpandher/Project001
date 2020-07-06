///
////////////////////////////////////////////////////////////////////////////////

#include "Logger.h"



int main(int argc, char** argv)
{
    Project001::Logger* loggerPtr = new Project001::Logger();
    loggerPtr->message("Test: %d %ld", 2020, 999000L);
    
    return 0;
}