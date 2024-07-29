#pragma once

#include "AutoIdMap.h"



namespace Project001
{
    class ComponentStores;
    class Renderer;
    class ResourceStores;
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
        bool fixedSizeFramebuffer_;
    };

    class Application
    {
    public:
        friend class Scene;

        Application(const ApplicationCreationInfo& applicationInfo);
        virtual ~Application();

        Application(Application& other) = delete;
        void operator=(const Application&) = delete;

        void Run();

    protected:
        void HandleEvent(Event& event);

        void ProcessFrameBufferSizeEvent(FrameBufferSizeEvent& frameBufferSizeEvent);
        void ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent);
        void ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent);

        unsigned long long desiredFrameDuration_ns_;
        bool sleepyRunLoop_;
        bool fixedSizeFramebuffer_;

        bool running_;

        Window* windowPtr_;
        Renderer* rendererPtr_;
        SoundPlayer* soundPlayerPtr_;
        ComponentStores* componentStoresPtr_;
        ResourceStores* resourceStoresPtr_;

        AutoIdMap<Scene*> scenePtrMap_;
        Scene* activeScenePtr_;
    };
}