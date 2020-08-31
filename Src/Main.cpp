///
////////////////////////////////////////////////////////////////////////////////

#include <thread>
#include <vector>

#include "Engine/Application.h"
#include "Engine/Logger.h"

#include "Platform/OpenGLWindow.h"

#include "DebugLayer.h"



int main(int argc, char** argv)
{
    Project001::Logger::Message("HELLO WORLD");

    Project001::Window* windowPtr = new Project001::OpenGLWindow("Project001", 800, 600);
    Project001::Application* applicationPtr = new Project001::Application(windowPtr);

    Project001::Layer* debugLayerPtr = new Project001::DebugLayer();
    applicationPtr->AddLayer(debugLayerPtr);

    applicationPtr->Run();
    
    return 0;
}