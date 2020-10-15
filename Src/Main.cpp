///
////////////////////////////////////////////////////////////////////////////////

#include <thread>
#include <vector>

#include "Engine/Application.h"
#include "Engine/Logger.h"
#include "Engine/TimingUtilities.h"

#include "DebugOutputWidget.h"
#include "RendererTestWidget.h"



int main(int argc, char** argv)
{
    Project001::Logger::Message("HELLO WORLD");

    Project001::Application* applicationPtr = new Project001::Application("Project001", 800, 600);

    // Project001::Widget* debugOutputWidgetPtr = new Project001::DebugOutputWidget();
    // applicationPtr->AddWidget("DebugOutputWidget", debugOutputWidgetPtr);

    Project001::Widget* rendererTestWidgetPtr = new Project001::RendererTestWidget();
    applicationPtr->AddWidget("RendererTestWidget", rendererTestWidgetPtr);

    applicationPtr->Run();
    
    return 0;
}