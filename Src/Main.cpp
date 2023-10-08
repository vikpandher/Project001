///
////////////////////////////////////////////////////////////////////////////////

#if (('1234' >> 24) == '1')
    #define LITTLE_ENDIAN
#elif (('4321' >> 24) == '1')
    #define BIG_ENDIAN
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
#include "TestScene030.h"
#include "TestScene031.h"
#include "TestScene032.h"
#include "TestScene033.h"
#include "TestScene034.h"
#include "TestScene050.h"



int main(int argc, char** argv)
{
    const size_t sizeScalar = 4;
    Project001::ApplicationInfo applicationInfo = {};
    applicationInfo.windowTitle = "Project001";
    applicationInfo.windowWidth = 240 * sizeScalar;
    applicationInfo.windowHeight = 160 * sizeScalar;
    applicationInfo.frameBufferWidth = 240 * sizeScalar;
    applicationInfo.frameBufferHeight = 160 * sizeScalar;
    applicationInfo.instanceBufferCapacity = 1024 * 8;
    applicationInfo.batchedIndexBufferCapacity = 1024 * 8;
    applicationInfo.batchedVertexBufferCapacity = 1024 * 8;
    Project001::Application* applicationPtr = new Project001::Application(applicationInfo);

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

    // tests 3d shape overlap tests (TODO) (001)
    TestScene013* testScene013Ptr = new TestScene013();
    applicationPtr->AddScene(testScene013Ptr);

    // tests framerate with many verticies batched rendering (001)
    TestScene030* testScene030Ptr = new TestScene030();
    applicationPtr->AddScene(testScene030Ptr);

    // tests framerate with many verticies instanced rendering (001)
    TestScene031* testScene031Ptr = new TestScene031();
    applicationPtr->AddScene(testScene031Ptr);

    // tests rendering batched and instanced together (001)
    TestScene032* testScene032Ptr = new TestScene032();
    applicationPtr->AddScene(testScene032Ptr);

    // tests multiple cameras (001)
    TestScene033* testScene033Ptr = new TestScene033();
    applicationPtr->AddScene(testScene033Ptr);

    // tests models (001)
    TestScene034* testScene034Ptr = new TestScene034();
    applicationPtr->AddScene(testScene034Ptr);

    // tests sound (001)
    TestScene050* testScene050Ptr = new TestScene050();
    applicationPtr->AddScene(testScene050Ptr);

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
    delete testScene030Ptr;
    delete testScene031Ptr;
    delete testScene032Ptr;
    delete testScene033Ptr;
    delete testScene034Ptr;
    delete testScene050Ptr;

    return 0;
}

// When building to run without a console...
// -----------------------------------------------------------------------------
#ifdef WINDOWS_NO_CONSOLE
#include <windows.h>

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    return main(0, nullptr);
}
#endif