///
////////////////////////////////////////////////////////////////////////////////

#include "Engine/Logger.h"
#include "Engine/Application.h"



int main(int argc, char** argv)
{    
    Project001::Logger::Message("HELLO WORLD");
    Project001::Logger::Message("0123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789");

    Project001::Application* applicationPtr = new Project001::Application("Project001", 800, 600);

    applicationPtr->Run();
    
    return 0;
}



// When building to run without a console...
// -----------------------------------------------------------------------------
// #include <windows.h>
// 
// INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
// {
//     return main(0, nullptr);
// }