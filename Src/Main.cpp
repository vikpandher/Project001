///
////////////////////////////////////////////////////////////////////////////////

#include "Engine/Application.h"

#include "TestScene001.h"
#include "TestScene002.h"
#include "TestScene003.h"
#include "TestScene004.h"



int main(int argc, char** argv)
{
    Project001::Application* applicationPtr = new Project001::Application("Project001", 240 * 6, 160 * 6);

    // 
    TestScene004* testScene004Ptr = new TestScene004();
    applicationPtr->AddScene(testScene004Ptr);

    // tests shape generation
    TestScene001* testScene001Ptr = new TestScene001();
    applicationPtr->AddScene(testScene001Ptr);

    // tests 3d shape generation
    TestScene002* testScene002Ptr = new TestScene002();
    applicationPtr->AddScene(testScene002Ptr);

    // tests sound
    TestScene003* testScene003Ptr = new TestScene003();
    applicationPtr->AddScene(testScene003Ptr);

    applicationPtr->Run();

    delete applicationPtr;
    delete testScene001Ptr;
    delete testScene002Ptr;
    delete testScene003Ptr;
    delete testScene004Ptr;

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