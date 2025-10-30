// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

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

#include "Scene001.h"
#include "Scene002.h"
#include "SharedApplicationData.h"



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
    applicationCreationInfo.windowTitle = "Trick or Treat on Spooky Street";
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

    SharedApplicationData sharedApplicationData = {};
    applicationPtr->SetSharedDataPtr<SharedApplicationData>(&sharedApplicationData);

    // main menu
    Scene001* scene001Ptr = new Scene001(applicationPtr);

    // game
    Scene002* scene002Ptr = new Scene002(applicationPtr);

    applicationPtr->Run();

    delete scene001Ptr;
    delete scene002Ptr;
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