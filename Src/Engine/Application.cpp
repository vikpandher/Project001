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
        componentStoresPtr_ = new ComponentStores();

        windowPtr_ = Window::Create(windowTitle, windowWidth, windowHeight);
        windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        meshStoresPtr_ = new MeshStores();
        textureStoresPtr_ = new TextureStores();

        rendererPtr_ = Renderer::Create(meshStoresPtr_, textureStoresPtr_);
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
            scenePtr->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
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
        double timeStampDifference = 0.0;
        double sleepTime = 0.0;

        if (activeScenePtr_ != nullptr)
        {
            activeScenePtr_->Initialize(componentStoresPtr_, meshStoresPtr_, textureStoresPtr_, rendererPtr_, windowPtr_);
            running_ = true;
        }

        while (running_)
        {
            timeStampA = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> workTime_ms = timeStampA - timeStampB;

            if (workTime_ms.count() < secondsPerFrame_)
            {
                std::chrono::duration<double, std::milli> delta_ms(secondsPerFrame_ - workTime_ms.count());
                std::chrono::duration<long long, std::milli> deltaDuration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
                std::this_thread::sleep_for(std::chrono::milliseconds(deltaDuration_ms.count()));
            }

            timeStampB = std::chrono::system_clock::now();
            //std::chrono::duration<double> currentTime_s = timeStampB.time_since_epoch();
            std::chrono::duration<double, std::milli> sleepTime_ms = timeStampB - timeStampA;
            std::chrono::duration<double, std::milli> totalTime_ms = workTime_ms + sleepTime_ms;

            windowPtr_->PollEvents();
            OnEvent(UpdateEvent(0, totalTime_ms.count() / 1000.0f));
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
            currentScenePtr_->Initialize(componentStoresPtr_,meshStoresPtr_, textureStoresPtr_, rendererPtr_, windowPtr_);
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

    void Application::ProcessWindowSizeEvent(WindowSizeEvent& windowSizeEvent)
    {
        int height = windowSizeEvent.height;
        int width = windowSizeEvent.width;

        int oldWidth;
        int oldHeight;
        windowPtr_->GetWindowSize(oldWidth, oldHeight);

        float aspectRatio = (float)windowWidth_ / (float)windowHeight_;

        int adjustedHeight = (int)(width / aspectRatio);
        int adjustedWidth = (int)(height * aspectRatio);

        if (adjustedWidth > width)
        {
            adjustedWidth = width;
        }

        if (adjustedHeight > height)
        {
            adjustedHeight = height;
        }

        int lowerLeftX = (width - adjustedWidth) / 2;
        int lowerLeftY = (height - adjustedHeight) / 2;

        windowPtr_->SetViewportSize(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
    }

    void Application::OnEvent(Event& event)
    {
        DispatchEvent<SwitchSceneEvent>(event, std::bind(&Application::ProcessSwitchSceneEvent, this, std::placeholders::_1));
        DispatchEvent<InitializeSceneEvent>(event, std::bind(&Application::ProcessInitializeSceneEvent, this, std::placeholders::_1));
        DispatchEvent<DeinitializeSceneEvent>(event, std::bind(&Application::ProcessDeinitializeSceneEvent, this, std::placeholders::_1));
        DispatchEvent<WindowCloseEvent>(event, std::bind(&Application::ProcessWindowCloseEvent, this, std::placeholders::_1));
        DispatchEvent<WindowSizeEvent>(event, std::bind(&Application::ProcessWindowSizeEvent, this, std::placeholders::_1));

        if (!event.handled && activeScenePtr_ != nullptr)
        {
            activeScenePtr_->OnEvent(event);
        }
    }
}