// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-20

#ifdef _WIN32
#ifdef WINDOWS_MEMORY_LEAK_CHECK
#define _CRTDBG_MAP_ALLOC // Required for _CrtSetDbgFlag to work correctly
#include <crtdbg.h>
#endif
#endif

#if (('1234' >> 24) == '1')
    #define LITTLE_ENDIAN
#elif (('4321' >> 24) == '1')
    #define BIG_ENDIAN
#else
    #error "Couldn't determine the endianness!"
#endif

#include "Application.h"

#include "TestApplicationData.h"
#include "TestScene001.h"
#include "TestScene002.h"
#include "TestScene003.h"
#include "TestScene004.h"
#include "TestScene006.h"
#include "TestScene007.h"
#include "TestScene008.h"
#include "TestScene009.h"
#include "TestScene010.h"
#include "TestScene011.h"
#include "TestScene012.h"
#include "TestScene013.h"
#include "TestScene015.h"
#include "TestScene016.h"
#include "TestScene017.h"
#include "TestScene023.h"
#include "TestScene030.h"
#include "TestScene031.h"
#include "TestScene032.h"
#include "TestScene033.h"
#include "TestScene034.h"
#include "TestScene050.h"
#include "TestScene051.h"
#include "TestScene060.h"
#include "TestScene070.h"
#include "TestScene101.h"
#include "TestScene102.h"
#include "TestScene103.h"



int main(int argc, char** argv)
{
#ifdef _WIN32
#ifdef WINDOWS_MEMORY_LEAK_CHECK
    // Enable memory leak detection
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // _CrtSetBreakAlloc(643);
#endif
#endif

    const size_t sizeScalar = 4;
    Project001::ApplicationCreationInfo applicationCreationInfo = {};
    applicationCreationInfo.windowTitle = "Project001_TestApp";
    applicationCreationInfo.windowWidth = 240 * sizeScalar;
    applicationCreationInfo.windowHeight = 160 * sizeScalar;
    applicationCreationInfo.frameBufferWidth = 240 * sizeScalar;
    applicationCreationInfo.frameBufferHeight = 160 * sizeScalar;
    applicationCreationInfo.instanceBufferCapacity = 1024 * 8;
    applicationCreationInfo.batchedIndexBufferCapacity = 1024 * 8;
    applicationCreationInfo.batchedVertexBufferCapacity = 1024 * 8;
    applicationCreationInfo.desiredFrameDuration_ns = 1000000000ull / 60ull;
    applicationCreationInfo.sleepyRunLoop = false;
    applicationCreationInfo.updatesInARowLimit = 100;
    applicationCreationInfo.fixedSizeFramebuffer = false;
    Project001::Application* applicationPtr = new Project001::Application(applicationCreationInfo);

    TestApplicationData testApplicationData = {};
    applicationPtr->SetSharedDataPtr<TestApplicationData>(&testApplicationData);

    // main menu
    TestScene001* testScene001Ptr = new TestScene001(applicationPtr);

    // tests shape generation
    // * inherits 001
    // * has "unit" tests
    TestScene002* testScene002Ptr = new TestScene002(applicationPtr);

    // tests shape generation
    // * inherits 001
    TestScene003* testScene003Ptr = new TestScene003(applicationPtr);

    // tests shape generation and rendering 35 textures
    // * inherits 001
    TestScene004* testScene004Ptr = new TestScene004(applicationPtr);

    // testing text generation
    // * inherits 001
    TestScene006* testScene006Ptr = new TestScene006(applicationPtr);

    // testing shape generation
    // * inherits 001
    TestScene007* testScene007Ptr = new TestScene007(applicationPtr);

    // testing shape generation
    // * inherits 001
    TestScene008* testScene008Ptr = new TestScene008(applicationPtr);

    // (TODO)
    // * inherits 001
    TestScene009* testScene009Ptr = new TestScene009(applicationPtr);

    // tests 2d shape overlap
    // * inherits 002
    // * has "unit" tests
    TestScene010* testScene010Ptr = new TestScene010(applicationPtr);

    // tests 2d shape overlap
    // * inherits 002
    TestScene011* testScene011Ptr = new TestScene011(applicationPtr);

    // tests 2d shape overlap
    // * inherits 002
    TestScene012* testScene012Ptr = new TestScene012(applicationPtr);

    // tests 2d shape overlap with many shapes
    // * inherits 002
    TestScene013* testScene013Ptr = new TestScene013(applicationPtr);

    // tests 2d physics interactions
    // * inherits 002
    TestScene015* testScene015Ptr = new TestScene015(applicationPtr);

    // tests 2d physics interactions
    // * inherits 002
    TestScene016* testScene016Ptr = new TestScene016(applicationPtr);

    // tests 2d physics interactions
    // * inherits 002
    TestScene017* testScene017Ptr = new TestScene017(applicationPtr);

    // (TODO)
    // tests 3d shape overlap tests
    // * inherits 001
    // * has "unit" tests
    TestScene023* testScene023Ptr = new TestScene023(applicationPtr);

    // tests framerate with many vertices batched rendering
    // * inherits 001
    TestScene030* testScene030Ptr = new TestScene030(applicationPtr);

    // tests framerate with many vertices instanced rendering
    // * inherits 001
    TestScene031* testScene031Ptr = new TestScene031(applicationPtr);

    // tests rendering batched and instanced together
    // * inherits 001
    TestScene032* testScene032Ptr = new TestScene032(applicationPtr);

    // tests multiple cameras
    // * inherits 001
    TestScene033* testScene033Ptr = new TestScene033(applicationPtr);

    // tests manipulating models
    // * inherits 001
    TestScene034* testScene034Ptr = new TestScene034(applicationPtr);

    // tests sound
    // * inherits 001
    // * has "unit" tests
    TestScene050* testScene050Ptr = new TestScene050(applicationPtr);

    // tests multiple sound sources
    TestScene051* testScene051Ptr = new TestScene051(applicationPtr);

    // tests joystick input
    TestScene060* testScene060Ptr = new TestScene060(applicationPtr);

    // tests the ".ini" reader and writer
    TestScene070* testScene070Ptr = new TestScene070(applicationPtr);

    // (TODO)
    TestScene101* testScene101Ptr = new TestScene101(applicationPtr);

    // (TODO)
    TestScene102* testScene102Ptr = new TestScene102(applicationPtr);

    // (TODO)
    TestScene103* testScene103Ptr = new TestScene103(applicationPtr);

    applicationPtr->Run();

    delete testScene001Ptr;
    delete testScene002Ptr;
    delete testScene003Ptr;
    delete testScene004Ptr;
    delete testScene006Ptr;
    delete testScene007Ptr;
    delete testScene008Ptr;
    delete testScene009Ptr;
    delete testScene010Ptr;
    delete testScene011Ptr;
    delete testScene012Ptr;
    delete testScene013Ptr;
    delete testScene015Ptr;
    delete testScene016Ptr;
    delete testScene017Ptr;
    delete testScene023Ptr;
    delete testScene030Ptr;
    delete testScene031Ptr;
    delete testScene032Ptr;
    delete testScene033Ptr;
    delete testScene034Ptr;
    delete testScene050Ptr;
    delete testScene051Ptr;
    delete testScene060Ptr;
    delete testScene070Ptr;
    delete testScene101Ptr;
    delete testScene102Ptr;
    delete testScene103Ptr;
    delete applicationPtr;

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