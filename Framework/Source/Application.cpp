// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-11-23

#include "Application.h"

#include "ComponentStores.h"
#include "Logger.h"
#include "Renderer.h"
#include "Scene.h"
#include "SoundPlayer.h"
#include "Window.h"

#include <chrono>
#include <functional>
#include <thread>



namespace Project001
{
    // public ------------------------------------------------------------------

    Application::Application(const ApplicationCreationInfo& applicationCreationInfo)
        : desiredFrameDuration_ns_(applicationCreationInfo.desiredFrameDuration_ns)
        , sleepyRunLoop_(applicationCreationInfo.sleepyRunLoop)
        , updatesInARowLimit_(applicationCreationInfo.updatesInARowLimit)
        , fixedSizeFramebuffer_(applicationCreationInfo.fixedSizeFramebuffer)
        , sharedDataTypeId_(0)
        , sharedDataPtr_(nullptr)
        , running_(false)
        , windowPtr_(nullptr)
        , rendererPtr_(nullptr)
        , soundPlayerPtr_(nullptr)
        , componentStoresPtr_(nullptr)
        , activeScenePtr_(nullptr)
    {
        Logger::EnableConsole();

        windowPtr_ = Window::Create(applicationCreationInfo.windowTitle, applicationCreationInfo.windowWidth, applicationCreationInfo.windowHeight);
        windowPtr_->SetEventCallback(std::bind(&Application::HandleEvent, this, std::placeholders::_1));
        windowPtr_->SetAspectRatio(applicationCreationInfo.windowWidth, applicationCreationInfo.windowHeight);

        int screenWidth;
        int screenHeight;
        windowPtr_->GetScreenSize(screenWidth, screenHeight);
        if (screenWidth > (int)applicationCreationInfo.windowWidth && screenHeight > (int)applicationCreationInfo.windowHeight)
        {
            windowPtr_->SetWindowPosition((screenWidth - applicationCreationInfo.windowWidth) / 2, (screenHeight - applicationCreationInfo.windowHeight) / 2);
        }

        Project001::RendererInfo rendererInfo = {};
        rendererInfo.windowPtr = windowPtr_;
        rendererInfo.frameBufferWidth = applicationCreationInfo.frameBufferWidth;
        rendererInfo.frameBufferHeight = applicationCreationInfo.frameBufferHeight;
        rendererInfo.instanceBufferCapacity = applicationCreationInfo.instanceBufferCapacity;
        rendererInfo.batchedIndexBufferCapacity = applicationCreationInfo.batchedIndexBufferCapacity;
        rendererInfo.batchedVertexBufferCapacity = applicationCreationInfo.batchedVertexBufferCapacity;
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

    void Application::Run()
    {
        if (activeScenePtr_ == nullptr)
        {
            LOG_ERROR_F("No Active Scene");
            return;
        }

        HandleEvent(InitializeEvent());
        running_ = true;

        std::chrono::system_clock::time_point lastFrameTimeStamp = std::chrono::system_clock::now();

        unsigned long long simulationTimeDebt_ns = 0ull;

        while (running_)
        {
            std::chrono::system_clock::time_point currentFrameTimeStamp = std::chrono::system_clock::now();
            std::chrono::duration<unsigned long long, std::nano> frameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
            unsigned long long frameDuration_ns = frameDuration.count();

            // Sleep to achive desired frame duration
            // -----------------------------------------------------------------

            if (sleepyRunLoop_)
            {
                if (frameDuration_ns < desiredFrameDuration_ns_)
                {
                    std::chrono::duration<unsigned long long, std::nano> delta_ns(desiredFrameDuration_ns_ - frameDuration_ns);
                    std::this_thread::sleep_for(delta_ns);
                    currentFrameTimeStamp = std::chrono::system_clock::now();
                    frameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
                    frameDuration_ns = frameDuration.count();
                }
            }
            else
            {
                while (frameDuration_ns < desiredFrameDuration_ns_)
                {
                    currentFrameTimeStamp = std::chrono::system_clock::now();
                    frameDuration = currentFrameTimeStamp - lastFrameTimeStamp;
                    frameDuration_ns = frameDuration.count();
                }
            }

            // Slept for too long, so catch up on UpdateEvents
            // -----------------------------------------------------------------

            simulationTimeDebt_ns += frameDuration_ns;
            unsigned int updatesInARow = 0;
            while (simulationTimeDebt_ns >= desiredFrameDuration_ns_)
            {
                HandleEvent(UpdateEvent(0, desiredFrameDuration_ns_));
                updatesInARow++;
                simulationTimeDebt_ns -= desiredFrameDuration_ns_;

                if (updatesInARow >= updatesInARowLimit_)
                {
                    simulationTimeDebt_ns = 0;
                    // LOG_WARNING_F("Too many updates without Rendering. Resetting simulation time debt.");
                    break;
                }
            }

            // Render
            // -----------------------------------------------------------------

            windowPtr_->PollEvents();
            HandleEvent(RenderEvent(0, frameDuration_ns));
            lastFrameTimeStamp = currentFrameTimeStamp;
        }

        HandleEvent(DeinitializeEvent());
    }

    // protected ------------------------------------------------------------------

    void Application::HandleEvent(Event& event)
    {
        activeScenePtr_->HandleEvent(event);

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
            LOG_ERROR_F("Failed to set active scene: " << sceneId);
        }
        switchSceneEvent.handled = true;
    }

    void Application::ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent)
    {
        running_ = false;
        windowCloseEvent.handled = true;
    }
}