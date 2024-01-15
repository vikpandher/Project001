#include "Application.h"

#include "ComponentStores.h"
#include "Event.h"
#include "Logger.h"
#include "Renderer.h"
#include "ResourceStores.h"
#include "SoundPlayer.h"
#include "Scene.h"
#include "Window.h"


#include <functional>
#include <thread>



namespace Project001
{
    // public ------------------------------------------------------------------

    Application::Application(const ApplicationInfo& applicationInfo)
        : desiredFrameDuration_ns_(applicationInfo.desiredFrameDuration_ns)
        , sleepyRunLoop_(applicationInfo.sleepyRunLoop)
        , fixedSizeFramebuffer_(applicationInfo.fixedSizeFramebuffer_)
        , running_(false)
        , windowPtr_(nullptr)
        , rendererPtr_(nullptr)
        , soundPlayerPtr_(nullptr)
        , componentStoresPtr_(nullptr)
        , activeScenePtr_(nullptr)
        , creationSuccessful_(false)
    {
        windowPtr_ = Window::Create(applicationInfo.windowTitle, applicationInfo.windowWidth, applicationInfo.windowHeight);
        if (windowPtr_)
        {
            windowPtr_->SetEventCallback(std::bind(&Application::HandleEvent, this, std::placeholders::_1));
            windowPtr_->SetAspectRatio(applicationInfo.windowWidth, applicationInfo.windowHeight);

            int screenWidth;
            int screenHeight;
            windowPtr_->GetScreenSize(screenWidth, screenHeight);
            if (screenWidth > (int)applicationInfo.windowWidth && screenHeight > (int)applicationInfo.windowHeight)
            {
                windowPtr_->SetWindowPosition((screenWidth - applicationInfo.windowWidth) / 2, (screenHeight - applicationInfo.windowHeight) / 2);
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

            if (rendererPtr_)
            {
                soundPlayerPtr_ = SoundPlayer::Create();

                if (soundPlayerPtr_)
                {
                    componentStoresPtr_ = new ComponentStores();

                    if (componentStoresPtr_)
                    {
                        resourceStoresPtr_ = new ResourceStores();

                        if (resourceStoresPtr_)
                        {
                            creationSuccessful_ = true;
                        }
                        else
                        {
                            _LOG_ERROR("Application failed to create the ResourceStores");
                        }
                    }
                    else
                    {
                        _LOG_ERROR("Application failed to create the ComponentStores");
                    }
                }
                else
                {
                    _LOG_ERROR("Application failed to create the SoundPlayer");
                }
            }
            else
            {
                _LOG_ERROR("Application failed to create the Renderer");
            }
        }
        else
        {
            _LOG_ERROR("Application failed to create the Window");
        }
    }

    Application::~Application()
    {
        delete resourceStoresPtr_;
        delete componentStoresPtr_;
        delete soundPlayerPtr_;
        delete rendererPtr_;
        delete windowPtr_;

        activeScenePtr_ = nullptr;

        for (AutoIdMap<Scene*>::iterator iter = scenePtrMap_.IteratorAtBeginning();
            iter != scenePtrMap_.IteratorPastTheEnd(); ++iter)
        {
            // So Scenes don't attempt to use a nonexisting Application
            iter->second->applicationPtr_ = nullptr;
        }

        _DESTROY_LOGGER();
    }

    void Application::Run()
    {
        if (!creationSuccessful_)
        {
            _LOG_ERROR("Failed to create Application");
            return;
        }

        if (!activeScenePtr_)
        {
            _LOG_ERROR("No Active Scene");
            return;
        }

        HandleEvent(InitializeEvent());
        running_ = true;

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
                HandleEvent(UpdateEvent(0, desiredFrameDuration_ns_));
                simulationTimeDebt_ns -= desiredFrameDuration_ns_;
            }

            // Render
            // -----------------------------------------------------------------

            HandleEvent(RenderEvent(0, lastFrameDuration_ns));
            windowPtr_->PollEvents();
        }

        HandleEvent(DeinitializeEvent());
    }

    // protected ------------------------------------------------------------------

    void Application::HandleEvent(Event& event)
    {
        if (activeScenePtr_)
        {
            activeScenePtr_->HandleEvent(event);
        }

        DispatchEvent<FrameBufferSizeEvent>(event, std::bind(&Application::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
        DispatchEvent<SwitchSceneEvent>(event, std::bind(&Application::ProcessSwitchSceneEvent, this, std::placeholders::_1));
        DispatchEvent<WindowCloseEvent>(event, std::bind(&Application::ProcessWindowCloseEvent, this, std::placeholders::_1));
    }

    void Application::ProcessFrameBufferSizeEvent(FrameBufferSizeEvent& frameBufferSizeEvent)
    {
        const int& height = frameBufferSizeEvent.height;
        const int& width = frameBufferSizeEvent.width;

        int aspectRatioNumerator, aspectRatioDenominator;
        windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

        if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
        {
            float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;

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

            if (!fixedSizeFramebuffer_)
            {
                rendererPtr_->SetFramebufferSize(adjustedWidth, adjustedHeight);
            }
            rendererPtr_->SetViewport(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
        }
        else
        {
            if (!fixedSizeFramebuffer_)
            {
                rendererPtr_->SetFramebufferSize(width, height);
            }
            rendererPtr_->SetViewport(0, 0, width, height);
        }
        frameBufferSizeEvent.handled = true;
    }

    void Application::ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent)
    {
        unsigned int& sceneId = switchSceneEvent.sceneId;
        AutoIdMap<Scene*>::iterator iter = scenePtrMap_.Find(sceneId);
        if (iter != scenePtrMap_.IteratorPastTheEnd())
        {
            activeScenePtr_ = iter->second;
        }
        else
        {
            _LOG_MESSAGE("Application: Failed to set active scene: %u", sceneId);
        }
        switchSceneEvent.handled = true;
    }

    void Application::ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent)
    {
        running_ = false;
        windowCloseEvent.handled = true;
    }
}