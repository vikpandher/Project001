///
////////////////////////////////////////////////////////////////////////////////

#include <thread>
#include <vector>

#include "Engine/Application.h"
#include "Engine/Logger.h"

#include "Platform/WindowImplementation.h"



int main(int argc, char** argv)
{
    Project001::Logger::Message("HELLO WORLD");

    Project001::Window* windowPtr = new Project001::WindowImplementation("Project001", 800, 600);
    Project001::Application* applicationPtr = new Project001::Application(windowPtr);
    applicationPtr->Run();
    
    return 0;
}