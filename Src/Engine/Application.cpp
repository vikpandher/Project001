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
        , desiredFrameDuration_ms_(1000.0 / 60)
        , sleepyRunLoop_(true)
        , running_(false)
        , paused_(false)
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
        if (activeScenePtr_ != nullptr)
        {
            activeScenePtr_->Initialize();
            running_ = true;
        }

        std::chrono::system_clock::time_point lastFrameTimeStamp;
        std::chrono::system_clock::time_point currentFrameTimeStamp = std::chrono::system_clock::now();

        double simulationTimeDebt_ms = 0.0;

        while (running_)
        {
            lastFrameTimeStamp = currentFrameTimeStamp;
            currentFrameTimeStamp = std::chrono::system_clock::now();
            std::chrono::duration<double, std::milli> lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
            double lastFrameDuration_ms = lastFrameDuration.count();

            if (sleepyRunLoop_)
            {
                if (lastFrameDuration_ms < desiredFrameDuration_ms_)
                {
                    std::chrono::duration<double, std::milli> delta_ms(desiredFrameDuration_ms_ - lastFrameDuration_ms);
                    std::this_thread::sleep_for(delta_ms);
                }
                currentFrameTimeStamp = std::chrono::system_clock::now();
                lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
                lastFrameDuration_ms = lastFrameDuration.count();
            }
            else
            {
                while (lastFrameDuration_ms < desiredFrameDuration_ms_)
                {
                    currentFrameTimeStamp = std::chrono::system_clock::now();
                    lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
                    lastFrameDuration_ms = lastFrameDuration.count();
                }
            }

            simulationTimeDebt_ms += lastFrameDuration_ms;
            while (simulationTimeDebt_ms > desiredFrameDuration_ms_)
            {
                OnEvent(UpdateEvent(0, desiredFrameDuration_ms_ / 1000.0));
                simulationTimeDebt_ms -= desiredFrameDuration_ms_;
            }

            OnEvent(RenderEvent(0, lastFrameDuration_ms / 1000.0));
            windowPtr_->PollEvents();
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