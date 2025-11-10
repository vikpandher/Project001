// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-10

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
#include "TestScene005.h"
#include "TestScene006.h"
#include "TestScene007.h"
#include "TestScene009.h"
#include "TestScene020.h"
#include "TestScene021.h"
#include "TestScene022.h"
#include "TestScene023.h"
#include "TestScene025.h"
#include "TestScene026.h"
#include "TestScene027.h"
#include "TestScene028.h"
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
#include "TestScene104.h"



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
    applicationCreationInfo.sleepyRunLoop = true;
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

    // (TODO)
    // * inherits 001
    TestScene005* testScene005Ptr = new TestScene005(applicationPtr);

    // testing text generation
    // * inherits 001
    TestScene006* testScene006Ptr = new TestScene006(applicationPtr);

    // testing shape generation
    // * inherits 001
    TestScene007* testScene007Ptr = new TestScene007(applicationPtr);

    // testing shape generation
    // * inherits 001
    TestScene009* TestScene009Ptr = new TestScene009(applicationPtr);

    // tests 2d shape overlap
    // * inherits 002
    // * has "unit" tests
    TestScene020* TestScene020Ptr = new TestScene020(applicationPtr);

    // tests 2d shape overlap
    // * inherits 002
    TestScene021* TestScene021Ptr = new TestScene021(applicationPtr);

    // tests 2d shape overlap
    // * inherits 002
    TestScene022* TestScene022Ptr = new TestScene022(applicationPtr);

    // tests 2d shape overlap with many shapes
    // * inherits 002
    TestScene023* TestScene023Ptr = new TestScene023(applicationPtr);

    // tests 2d physics interactions
    // * inherits 002
    TestScene025* TestScene025Ptr = new TestScene025(applicationPtr);

    // tests 2d physics interactions
    // * inherits 002
    TestScene026* TestScene026Ptr = new TestScene026(applicationPtr);

    // tests 2d physics interactions
    // * inherits 002
    TestScene027* TestScene027Ptr = new TestScene027(applicationPtr);

    // (TODO)
    // tests 3d shape overlap tests
    // * inherits 001
    // * has "unit" tests
    TestScene028* TestScene028Ptr = new TestScene028(applicationPtr);

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

    // test mouse picking
    TestScene102* testScene102Ptr = new TestScene102(applicationPtr);

    // (TODO)
    TestScene103* testScene103Ptr = new TestScene103(applicationPtr);

    // (TODO)
    TestScene104* testScene104Ptr = new TestScene104(applicationPtr);

    applicationPtr->Run();

    delete testScene001Ptr;
    delete testScene002Ptr;
    delete testScene003Ptr;
    delete testScene004Ptr;
    delete testScene005Ptr;
    delete testScene006Ptr;
    delete testScene007Ptr;
    delete TestScene009Ptr;
    delete TestScene020Ptr;
    delete TestScene021Ptr;
    delete TestScene022Ptr;
    delete TestScene023Ptr;
    delete TestScene025Ptr;
    delete TestScene026Ptr;
    delete TestScene027Ptr;
    delete TestScene028Ptr;
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
    delete testScene104Ptr;
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