// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

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

#include "TestScene001.h"
#include "TestScene002.h"
#include "TestScene003.h"
#include "TestScene004.h"
#include "TestScene006.h"
#include "TestScene007.h"
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
#include "TestSceneIds.h"

unsigned int g_testScene001Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene002Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene003Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene004Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene006Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene007Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene010Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene011Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene012Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene013Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene015Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene016Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene017Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene023Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene030Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene031Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene032Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene033Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene034Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene050Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene051Id = (unsigned int)-1; // declared extern in TestSceneIds.h
unsigned int g_testScene060Id = (unsigned int)-1; // declared extern in TestSceneIds.h



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

    // main menu
    TestScene001* testScene001Ptr = new TestScene001(applicationPtr);
    g_testScene001Id = testScene001Ptr->GetId();

    // tests shape generation
    // * inherits 001
    // * has "unit" tests
    TestScene002* testScene002Ptr = new TestScene002(applicationPtr);
    g_testScene002Id = testScene002Ptr->GetId();

    // tests 3d shape generation
    // * inherits 001
    TestScene003* testScene003Ptr = new TestScene003(applicationPtr);
    g_testScene003Id = testScene003Ptr->GetId();

    // tests additional shape generation and rendering 35 textures
    // * inherits 001
    TestScene004* testScene004Ptr = new TestScene004(applicationPtr);
    g_testScene004Id = testScene004Ptr->GetId();

    // testing text generation
    // * inherits 001
    TestScene006* testScene006Ptr = new TestScene006(applicationPtr);
    g_testScene006Id = testScene006Ptr->GetId();

    // testing ...
    // * inherits 001
    TestScene007* testScene007Ptr = new TestScene007(applicationPtr);
    g_testScene007Id = testScene007Ptr->GetId();

    // tests 2d shape overlap
    // * inherits 002
    // * has "unit" tests
    TestScene010* testScene010Ptr = new TestScene010(applicationPtr);
    g_testScene010Id = testScene010Ptr->GetId();

    // tests 2d shape overlap
    // * inherits 002
    TestScene011* testScene011Ptr = new TestScene011(applicationPtr);
    g_testScene011Id = testScene011Ptr->GetId();

    // tests 2d shape overlap
    // * inherits 002
    TestScene012* testScene012Ptr = new TestScene012(applicationPtr);
    g_testScene012Id = testScene012Ptr->GetId();

    // tests 2d shape overlap with many shapes
    // * inherits 002
    TestScene013* testScene013Ptr = new TestScene013(applicationPtr);
    g_testScene013Id = testScene013Ptr->GetId();

    // (TODO)
    // tests 2d physics interactions
    // * inherits 002
    TestScene015* testScene015Ptr = new TestScene015(applicationPtr);
    g_testScene015Id = testScene015Ptr->GetId();

    // (TODO)
    // tests 2d physics interactions
    // * inherits 002
    TestScene016* testScene016Ptr = new TestScene016(applicationPtr);
    g_testScene016Id = testScene016Ptr->GetId();

    // (TODO)
    // tests 2d physics interactions
    // * inherits 002
    TestScene017* testScene017Ptr = new TestScene017(applicationPtr);
    g_testScene017Id = testScene017Ptr->GetId();

    // (TODO)
    // tests 3d shape overlap tests
    // * inherits 001
    // * has "unit" tests
    TestScene023* testScene023Ptr = new TestScene023(applicationPtr);
    g_testScene023Id = testScene023Ptr->GetId();

    // tests framerate with many verticies batched rendering
    // * inherits 001
    TestScene030* testScene030Ptr = new TestScene030(applicationPtr);
    g_testScene030Id = testScene030Ptr->GetId();

    // tests framerate with many verticies instanced rendering
    // * inherits 001
    TestScene031* testScene031Ptr = new TestScene031(applicationPtr);
    g_testScene031Id = testScene031Ptr->GetId();

    // tests rendering batched and instanced together
    // * inherits 001
    TestScene032* testScene032Ptr = new TestScene032(applicationPtr);
    g_testScene032Id = testScene032Ptr->GetId();

    // tests multiple cameras
    // * inherits 001
    TestScene033* testScene033Ptr = new TestScene033(applicationPtr);
    g_testScene033Id = testScene033Ptr->GetId();

    // tests manipulating models
    // * inherits 001
    TestScene034* testScene034Ptr = new TestScene034(applicationPtr);
    g_testScene034Id = testScene034Ptr->GetId();

    // tests sound
    // * inherits 001
    // * has "unit" tests
    TestScene050* testScene050Ptr = new TestScene050(applicationPtr);
    g_testScene050Id = testScene050Ptr->GetId();

    // tests multiple sound sources
    TestScene051* testScene051Ptr = new TestScene051(applicationPtr);
    g_testScene051Id = testScene051Ptr->GetId();

    // tests first joystick input
    TestScene060* testScene060Ptr = new TestScene060(applicationPtr);
    g_testScene060Id = testScene060Ptr->GetId();

    applicationPtr->Run();

    delete testScene001Ptr;
    delete testScene002Ptr;
    delete testScene003Ptr;
    delete testScene004Ptr;
    delete testScene006Ptr;
    delete testScene007Ptr;
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