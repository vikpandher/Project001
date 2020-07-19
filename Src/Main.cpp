///
////////////////////////////////////////////////////////////////////////////////

#include <thread>

#include "Application.h"
#include "Logger.h"



int main(int argc, char** argv)
{
    Project001::Logger::Message("HELLO WORLD");
    
    Project001::Application* applicationPtr = new Project001::Application();
    applicationPtr->Run();
    
    return 0;
}