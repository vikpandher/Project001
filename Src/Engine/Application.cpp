#include "Application.h"

#include <functional>
#include <thread>

#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/SoundStores.h"
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
        , desiredFrameDuration_ns_(1000000000ul / 60ul)
        , sleepyRunLoop_(true)
        , running_(false)
        , activeScenePtr_(nullptr)
    {
        windowPtr_ = Window::Create(windowTitle, windowWidth, windowHeight);
        windowPtr_->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        windowPtr_->SetAspectRatio(windowWidth, windowHeight);
        windowPtr_->MakeContextCurrent();

        int screenWidth;
        int screenHeight;
        windowPtr_->GetScreenSize(screenWidth, screenHeight);
        if (screenWidth > (int)windowWidth && screenHeight > (int)windowHeight)
        {
            windowPtr_->SetWindowPosition((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
        }

        componentStoresPtr_ = new ComponentStores();
        meshStoresPtr_ = new MeshStores();
        soundStoresPtr_ = new SoundStores();
        textureStoresPtr_ = new TextureStores();

        rendererPtr_ = Renderer::Create(windowWidth, windowHeight);
        soundPlayerPtr_ = SoundPlayer::Create();
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

        unsigned long simulationTimeDebt_ns = 0ul;

        while (running_)
        {
            lastFrameTimeStamp = currentFrameTimeStamp;
            currentFrameTimeStamp = std::chrono::system_clock::now();
            std::chrono::duration<unsigned long, std::nano> lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
            unsigned long lastFrameDuration_ns = lastFrameDuration.count();

            if (sleepyRunLoop_)
            {
                if (lastFrameDuration_ns < desiredFrameDuration_ns_)
                {
                    std::chrono::duration<unsigned long, std::nano> delta_ns(desiredFrameDuration_ns_ - lastFrameDuration_ns);
                    std::this_thread::sleep_for(delta_ns);
                }
                currentFrameTimeStamp = std::chrono::system_clock::now();
                lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
                lastFrameDuration_ns = lastFrameDuration.count();
            }
            else
            {
                while (lastFrameDuration_ns < desiredFrameDuration_ns_)
                {
                    currentFrameTimeStamp = std::chrono::system_clock::now();
                    lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
                    lastFrameDuration_ns = lastFrameDuration.count();
                }
            }

            simulationTimeDebt_ns += lastFrameDuration_ns;
            while (simulationTimeDebt_ns > desiredFrameDuration_ns_)
            {
                OnEvent(UpdateEvent(0, desiredFrameDuration_ns_));
                simulationTimeDebt_ns -= desiredFrameDuration_ns_;
            }

            OnEvent(RenderEvent(0, lastFrameDuration_ns));
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