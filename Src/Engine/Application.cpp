#include "Application.h"

#include <functional>
#include <thread>

#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/Scene.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



namespace Project001
{
    // public: -----------------------------------------------------------------

    Application::Application(const char* windowTitle, unsigned int windowWidth, unsigned int windowHeight)
        : windowTitle_(windowTitle)
        , windowWidth_(windowWidth)
        , windowHeight_(windowHeight)
        , running_(false)
        , secondsPerFrame_(1.0 / 60.0)
        , activeScenePtr_(nullptr)
    {
        windowPtr_ = Window::Create(windowTitle, windowWidth, windowHeight);
        windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        windowPtr_->SetAspectRatio(windowWidth, windowHeight);

        int screenWidth;
        int screenHeight;
        windowPtr_->GetScreenSize(screenWidth, screenHeight);
        if (screenWidth > (int)windowWidth && screenHeight > (int)windowHeight)
        {
            windowPtr_->SetWindowPosition((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
        }

        componentStoresPtr_ = new ComponentStores();
        meshStoresPtr_ = new MeshStores();
        textureStoresPtr_ = new TextureStores();

        rendererPtr_ = Renderer::Create(windowWidth, windowHeight);
    }

    Application::~Application()
    {
        if (componentStoresPtr_ != nullptr)
        {
            delete componentStoresPtr_;
        }

        if (meshStoresPtr_ != nullptr)
        {
            delete meshStoresPtr_;
        }

        if (textureStoresPtr_ != nullptr)
        {
            delete textureStoresPtr_;
        }

        if (rendererPtr_ != nullptr)
        {
            delete rendererPtr_;
        }

        if (windowPtr_ != nullptr)
        {
            delete windowPtr_;
        }
    }

    void Application::AddScene(Scene* scenePtr)
    {
        std::string name(scenePtr->Name());
        if (sceneMap_.find(name) == sceneMap_.end())
        {
            scenePtr->applicationPtr_ = this;
            scenePtr->EventCallback = std::bind(&Application::OnEvent, this, std::placeholders::_1);

            sceneMap_[name] = scenePtr;

            if (sceneMap_.size() == 1)
            {
                OnEvent(SwitchSceneEvent(name));
            }
        }
    }

    void Application::Run()
    {
        std::chrono::system_clock::time_point timeStampA = std::chrono::system_clock::now();
        std::chrono::system_clock::time_point timeStampB = std::chrono::system_clock::now();

        if (activeScenePtr_ != nullptr)
        {
            activeScenePtr_->Initialize();
            running_ = true;
        }

        double millisecondsPerFrame = secondsPerFrame_ * 1000.0f;

        if (true)
        {
            while (running_)
            {
                timeStampA = std::chrono::system_clock::now();
                std::chrono::duration<double, std::milli> workTime_ms = timeStampA - timeStampB;

                if (workTime_ms.count() < millisecondsPerFrame)
                {
                    std::chrono::duration<double, std::milli> delta_ms(millisecondsPerFrame - workTime_ms.count());
                    std::this_thread::sleep_for(delta_ms);
                }

                timeStampB = std::chrono::system_clock::now();
                std::chrono::duration<double, std::milli> sleepTime_ms = timeStampB - timeStampA;
                std::chrono::duration<double, std::milli> totalTime_ms = workTime_ms + sleepTime_ms;

                OnEvent(UpdateEvent(0, totalTime_ms.count() / 1000.0f));
                windowPtr_->PollEvents();
            }
        }
        else
        {
            while (running_)
            {
                timeStampB = timeStampA;
                timeStampA = std::chrono::system_clock::now();
                std::chrono::duration<double, std::milli> totalTime_ms = timeStampA - timeStampB;

                while (totalTime_ms.count() < millisecondsPerFrame)
                {
                    timeStampA = std::chrono::system_clock::now();
                    totalTime_ms = timeStampA - timeStampB;
                }

                OnEvent(UpdateEvent(0, totalTime_ms.count() / 1000.0f));
                windowPtr_->PollEvents();
            }
        }
    }

    // protected: -----------------------------------------------------------------

    void Application::ProcessDeinitializeSceneEvent(DeinitializeSceneEvent& deinitializeSceneEvent)
    {
        std::string& name = deinitializeSceneEvent.sceneName;
        if (sceneMap_.find(name) != sceneMap_.end())
        {
            Scene* currentScenePtr_ = sceneMap_[name];
            currentScenePtr_->Deinitialize();
        }
        deinitializeSceneEvent.handled = true;
    }

    void Application::ProcessInitializeSceneEvent(InitializeSceneEvent& initializeSceneEvent)
    {
        std::string& name = initializeSceneEvent.sceneName;
        if (sceneMap_.find(name) != sceneMap_.end())
        {
            Scene* currentScenePtr_ = sceneMap_[name];
            currentScenePtr_->Initialize();
        }
        initializeSceneEvent.handled = true;
    }

    void Application::ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent)
    {
        std::string& name = switchSceneEvent.sceneName;
        if (sceneMap_.find(name) != sceneMap_.end())
        {
            activeScenePtr_ = sceneMap_[name];
        }
        switchSceneEvent.handled = true;
    }

    void Application::ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent)
    {
        running_ = false;
        windowCloseEvent.handled = true;
    }

    void Application::OnEvent(Event& event)
    {
        DispatchEvent<SwitchSceneEvent>(event, std::bind(&Application::ProcessSwitchSceneEvent, this, std::placeholders::_1));
        DispatchEvent<InitializeSceneEvent>(event, std::bind(&Application::ProcessInitializeSceneEvent, this, std::placeholders::_1));
        DispatchEvent<DeinitializeSceneEvent>(event, std::bind(&Application::ProcessDeinitializeSceneEvent, this, std::placeholders::_1));

        DispatchEvent<WindowCloseEvent>(event, std::bind(&Application::ProcessWindowCloseEvent, this, std::placeholders::_1));

        if (!event.handled && activeScenePtr_ != nullptr)
        {
            activeScenePtr_->OnEvent(event);
        }
    }
}