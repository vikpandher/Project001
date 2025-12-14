// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#pragma once

#include "AutoIdMap.h"

#include <typeinfo>



namespace Project001
{
    class ComponentStores;
    class Renderer;
    class SoundPlayer;
    class Window;

    struct Event;
    struct FrameBufferSizeEvent;
    struct SwitchSceneEvent;
    struct WindowCloseEvent;

    struct ApplicationCreationInfo
    {
        const char* windowTitle;
        unsigned int windowWidth;
        unsigned int windowHeight;
        unsigned int frameBufferWidth;
        unsigned int frameBufferHeight;
        unsigned int instanceBufferCapacity;
        unsigned int batchedIndexBufferCapacity;
        unsigned int batchedVertexBufferCapacity;
        unsigned long long desiredFrameDuration_ns;
        bool sleepyRunLoop;
        unsigned int updatesInARowLimit;
        bool fixedSizeFramebuffer;
    };

    class Application
    {
    public:
        friend class Scene;

        Application(const ApplicationCreationInfo& applicationCreationInfo);
        virtual ~Application();

        Application(Application& other) = delete;
        void operator=(const Application&) = delete;

        template <typename SharedDataType>
        SharedDataType* GetSharedDataPtr();

        template <typename SharedDataType>
        void SetSharedDataPtr(SharedDataType* sharedDataPtr);

        void Run();

    protected:
        void HandleEvent(Event& event);

        void ProcessFrameBufferSizeEvent(FrameBufferSizeEvent& frameBufferSizeEvent);
        void ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent);
        void ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent);

        unsigned long long desiredFrameDuration_ns_;
        bool sleepyRunLoop_;
        unsigned long updatesInARowLimit_;
        bool fixedSizeFramebuffer_;

        size_t sharedDataTypeId_;
        void* sharedDataPtr_;

        bool running_;

        Window* windowPtr_;
        Renderer* rendererPtr_;
        SoundPlayer* soundPlayerPtr_;
        ComponentStores* componentStoresPtr_;

        AutoIdMap<Scene*> scenePtrMap_;
        Scene* activeScenePtr_;
    };

    // public ------------------------------------------------------------------

    template <typename SharedDataType>
    inline SharedDataType* Application::GetSharedDataPtr()
    {
        size_t sharedDataTypeId = typeid(SharedDataType).hash_code();
        if (sharedDataTypeId_ == sharedDataTypeId)
        {
            return reinterpret_cast<SharedDataType*>(sharedDataPtr_);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename SharedDataType>
    void Application::SetSharedDataPtr(SharedDataType* sharedDataPtr)
    {
        sharedDataTypeId_ = typeid(SharedDataType).hash_code();
        sharedDataPtr_ = sharedDataPtr;
    }
}