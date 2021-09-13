///
////////////////////////////////////////////////////////////////////////////////

#include "Engine/Application.h"

#include "TestScene.h"



int main(int argc, char** argv)
{
    Project001::Application* applicationPtr = new Project001::Application("Project001", 240 * 4, 160 * 4);

    TestScene* testScenePtr = new TestScene();

    applicationPtr->AddScene(testScenePtr);
    applicationPtr->Run();

    delete testScenePtr;
    delete applicationPtr;

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