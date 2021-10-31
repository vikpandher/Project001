///
////////////////////////////////////////////////////////////////////////////////

#include "Engine/Application.h"

#include "TestScene001.h"
#include "TestScene002.h"



int main(int argc, char** argv)
{
    Project001::Application* applicationPtr = new Project001::Application("Project001", 240 * 6, 160 * 6);

    TestScene001* testScene001Ptr = new TestScene001();
    applicationPtr->AddScene(testScene001Ptr);

    TestScene002* testScene002Ptr = new TestScene002();
    applicationPtr->AddScene(testScene002Ptr);

    applicationPtr->Run();

    delete applicationPtr;
    delete testScene002Ptr;
    delete testScene001Ptr;

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