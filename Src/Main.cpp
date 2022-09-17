///
////////////////////////////////////////////////////////////////////////////////

#if (('1234' >> 24) == '1')
    #define LITTLE_ENDIAN
// #elif (('4321' >> 24) == '1')
//     #define BIG_ENDIAN
#else
    #error "Couldn't determine the endianness!"
#endif

#include "Engine/Application.h"

#include "TestScene002.h"
#include "TestScene003.h"
#include "TestScene004.h"
#include "TestScene006.h"
#include "TestScene007.h"
#include "TestScene010.h"
#include "TestScene011.h"
#include "TestScene012.h"
#include "TestScene013.h"
#include "TestScene020.h"



int main(int argc, char** argv)
{
    Project001::Application* applicationPtr = new Project001::Application("Project001", 240 * 6, 160 * 6);

    // tests 3d shape overlap (TODO) (001)
    TestScene013* testScene013Ptr = new TestScene013();
    applicationPtr->AddScene(testScene013Ptr);

    // tests sound (001)
    TestScene020* testScene020Ptr = new TestScene020();
    applicationPtr->AddScene(testScene020Ptr);

    // tests shape generation (001)
    TestScene002* testScene002Ptr = new TestScene002();
    applicationPtr->AddScene(testScene002Ptr);

    // tests 3d shape generation (001)
    TestScene003* testScene003Ptr = new TestScene003();
    applicationPtr->AddScene(testScene003Ptr);

    // tests additional shape generation and rendering 35 textures (001)
    TestScene004* testScene004Ptr = new TestScene004();
    applicationPtr->AddScene(testScene004Ptr);

    // testing text generation (001)
    TestScene006* testScene006Ptr = new TestScene006();
    applicationPtr->AddScene(testScene006Ptr);

    // tests 2d MeshStores::Generate2DLine_v2 (TODO) (001)
    TestScene007* testScene007Ptr = new TestScene007();
    applicationPtr->AddScene(testScene007Ptr);

    // tests 2d shape overlap (002)
    TestScene010* testScene010Ptr = new TestScene010();
    applicationPtr->AddScene(testScene010Ptr);

    // tests 2d shape overlap (002)
    TestScene011* testScene011Ptr = new TestScene011();
    applicationPtr->AddScene(testScene011Ptr);

    // tests 2d shape overlap (002)
    TestScene012* testScene012Ptr = new TestScene012();
    applicationPtr->AddScene(testScene012Ptr);

    applicationPtr->Run();

    delete applicationPtr;
    delete testScene002Ptr;
    delete testScene003Ptr;
    delete testScene004Ptr;
    delete testScene006Ptr;
    delete testScene007Ptr;
    delete testScene010Ptr;
    delete testScene011Ptr;
    delete testScene012Ptr;
    delete testScene013Ptr;
    delete testScene020Ptr;

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