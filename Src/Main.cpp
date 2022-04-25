///
////////////////////////////////////////////////////////////////////////////////

#include "Engine/Application.h"

#include "TestScene001.h"
#include "TestScene002.h"
#include "TestScene003.h"
#include "TestScene004.h"
#include "TestScene005.h"
#include "TestScene006.h"
#include "TestScene007.h"
#include "TestScene008.h"



int main(int argc, char** argv)
{
    Project001::Application* applicationPtr = new Project001::Application("Project001", 240 * 6, 160 * 6);

    // tests more shape generation
    TestScene003* testScene003Ptr = new TestScene003();
    applicationPtr->AddScene(testScene003Ptr);

    // tests sound
    TestScene004* testScene004Ptr = new TestScene004();
    applicationPtr->AddScene(testScene004Ptr);

    // tests 2d shape overlap
    TestScene005* testScene005Ptr = new TestScene005();
    applicationPtr->AddScene(testScene005Ptr);

    // tests 2d shape overlap
    TestScene006* testScene006Ptr = new TestScene006();
    applicationPtr->AddScene(testScene006Ptr);

    // tests 2d shape overlap
    TestScene007* testScene007Ptr = new TestScene007();
    applicationPtr->AddScene(testScene007Ptr);

    // tests 3d shape overlap (TODO)
    TestScene008* testScene008Ptr = new TestScene008();
    applicationPtr->AddScene(testScene008Ptr);

    // tests shape generation
    TestScene001* testScene001Ptr = new TestScene001();
    applicationPtr->AddScene(testScene001Ptr);

    // tests 3d shape generation
    TestScene002* testScene002Ptr = new TestScene002();
    applicationPtr->AddScene(testScene002Ptr);

    applicationPtr->Run();

    delete applicationPtr;
    delete testScene001Ptr;
    delete testScene002Ptr;
    delete testScene003Ptr;
    delete testScene004Ptr;
    delete testScene005Ptr;
    delete testScene006Ptr;
    delete testScene007Ptr;
    delete testScene008Ptr;

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