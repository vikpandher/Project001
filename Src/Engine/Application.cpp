#include "Application.h"

#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/Scene.h"
#include "Engine/Window.h"


#include <functional>
#include <thread>



namespace Project001
{
    // public ------------------------------------------------------------------

    Application::Application(const ApplicationInfo& applicationInfo)
        : windowTitle_(applicationInfo.windowTitle)
        , windowWidth_(applicationInfo.windowWidth)
        , windowHeight_(applicationInfo.windowHeight)
        , desiredFrameDuration_ns_(1000000000ull / 60ull)
        , sleepyRunLoop_(true)
        , running_(false)
        , activeScenePtr_(nullptr)
    {
        windowPtr_ = Window::Create(windowTitle_.c_str(), windowWidth_, windowHeight_);
        windowPtr_->SetEventCallback(std::bind(&Application::OnHandleEvent, this, std::placeholders::_1));
        windowPtr_->SetAspectRatio(windowWidth_, windowHeight_);

        int screenWidth;
        int screenHeight;
        windowPtr_->GetScreenSize(screenWidth, screenHeight);
        if (screenWidth > (int)windowWidth_ && screenHeight > (int)windowHeight_)
        {
            windowPtr_->SetWindowPosition((screenWidth - windowWidth_) / 2, (screenHeight - windowHeight_) / 2);
        }

        Project001::RendererInfo rendererInfo = {};
        rendererInfo.windowPtr = windowPtr_;
        rendererInfo.frameBufferWidth = applicationInfo.frameBufferWidth;
        rendererInfo.frameBufferHeight = applicationInfo.frameBufferHeight;
        rendererInfo.instanceBufferCapacity = applicationInfo.instanceBufferCapacity;
        rendererInfo.batchedIndexBufferCapacity = applicationInfo.batchedIndexBufferCapacity;
        rendererInfo.batchedVertexBufferCapacity = applicationInfo.batchedVertexBufferCapacity;
        rendererInfo.multisampleAntiAliasing = false;
        rendererInfo.depthTesting = true;
        rendererPtr_ = Renderer::Create(rendererInfo);
        soundPlayerPtr_ = SoundPlayer::Create();
        componentStoresPtr_ = new ComponentStores();
    }

    Application::~Application()
    {
        delete componentStoresPtr_;
        delete soundPlayerPtr_;
        delete rendererPtr_;
        delete windowPtr_;
    }

    bool Application::AddScene(Scene* scenePtr)
    {
        std::string name(scenePtr->Name());
        if (sceneMap_.find(name) == sceneMap_.end())
        {
            scenePtr->applicationPtr_ = this;
            scenePtr->EventCallback = std::bind(&Application::OnHandleEvent, this, std::placeholders::_1);

            sceneMap_[name] = scenePtr;

            if (sceneMap_.size() == 1)
            {
                OnHandleEvent(SwitchSceneEvent(name));
            }

            return true;
        }

        return false;
    }

    void Application::Run()
    {
        if (activeScenePtr_ != nullptr && activeScenePtr_->Initialize())
        {
            running_ = true;
        }

        std::chrono::system_clock::time_point lastFrameTimeStamp;
        std::chrono::system_clock::time_point currentFrameTimeStamp = std::chrono::system_clock::now();

        unsigned long long simulationTimeDebt_ns = 0ull;
        size_t unsigned_long_size = sizeof(unsigned long);

        while (running_)
        {
            lastFrameTimeStamp = currentFrameTimeStamp;
            currentFrameTimeStamp = std::chrono::system_clock::now();
            std::chrono::duration<unsigned long long, std::nano> lastFrameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
            unsigned long long lastFrameDuration_ns = lastFrameDuration.count();

            // Sleep to achive desired frame duration
            // -----------------------------------------------------------------

            if (sleepyRunLoop_)
            {
                if (lastFrameDuration_ns < desiredFrameDuration_ns_)
                {
                    std::chrono::duration<unsigned long long, std::nano> delta_ns(desiredFrameDuration_ns_ - lastFrameDuration_ns);
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

            // Slept for too long, so catch up on UpdateEvents
            // -----------------------------------------------------------------

            simulationTimeDebt_ns += lastFrameDuration_ns;
            while (simulationTimeDebt_ns > desiredFrameDuration_ns_)
            {
                OnHandleEvent(UpdateEvent(0, desiredFrameDuration_ns_));
                simulationTimeDebt_ns -= desiredFrameDuration_ns_;
            }

            // Render
            // -----------------------------------------------------------------

            OnHandleEvent(RenderEvent(0, lastFrameDuration_ns));
            windowPtr_->PollEvents();
        }
    }

    // protected ------------------------------------------------------------------

    void Application::OnHandleEvent(Event& event)
    {
        if (activeScenePtr_ != nullptr)
        {
            activeScenePtr_->OnHandleEvent(event);
        }

        if (!event.handled)
        {
            DispatchEvent<SwitchSceneEvent>(event, std::bind(&Application::ProcessSwitchSceneEvent, this, std::placeholders::_1));
            DispatchEvent<InitializeSceneEvent>(event, std::bind(&Application::ProcessInitializeSceneEvent, this, std::placeholders::_1));
            DispatchEvent<DeinitializeSceneEvent>(event, std::bind(&Application::ProcessDeinitializeSceneEvent, this, std::placeholders::_1));

            DispatchEvent<WindowCloseEvent>(event, std::bind(&Application::ProcessWindowCloseEvent, this, std::placeholders::_1));
        }
    }

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
}