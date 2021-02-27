///
////////////////////////////////////////////////////////////////////////////////

#include "Engine/Application.h"
#include "Engine/Logger.h"

#include "Engine/Component.h"
#include "Engine/ComponentStores.h"



int main(int argc, char** argv)
{
    Project001::ComponentContainer testComponentContainer;
    testComponentContainer.CreateComponent<Project001::TestComponent>(0, 10, 10, 10);
    testComponentContainer.CreateComponent<Project001::TestComponent>(1, 20, 20, 20);
    testComponentContainer.CreateComponent<Project001::TestComponent>(2, 30, 30, 30);

    Project001::TestComponent* testComponent0 = nullptr;
    Project001::TestComponent* testComponent1 = nullptr;
    Project001::TestComponent* testComponent2 = nullptr;
    testComponentContainer.GetComponent<Project001::TestComponent>(0, testComponent0);
    testComponentContainer.GetComponent<Project001::TestComponent>(1, testComponent1);
    testComponentContainer.GetComponent<Project001::TestComponent>(2, testComponent2);

    Project001::TestComponent* allTestComponents = nullptr;
    size_t count = 0;
    testComponentContainer.GetAllComponents<Project001::TestComponent>(allTestComponents, count);

    testComponentContainer.DeleteComponent<Project001::TestComponent>(0);
    
    Project001::Logger::Message("HELLO WORLD");
    Project001::Logger::Message("0123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789");

    Project001::Application* applicationPtr = new Project001::Application("Project001", 800, 600);

    applicationPtr->Run();
    
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