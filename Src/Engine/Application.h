#pragma once

#include <string>
#include <unordered_map>



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

    struct ApplicationInfo
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
        Application(const ApplicationInfo& applicationInfo);
        virtual ~Application();

        Application(Application& other) = delete;
        void operator=(const Application&) = delete;

        void Run();

    protected:
        friend class Scene;

        // This function exists so that if the Application gets deleted before
        // its Scenes, the Scenes won't be able to access junk memory.
        void NullifySceneApplicationPtr(Scene* scene);

        void HandleEvent(Event& event);

        void ProcessFrameBufferSizeEvent(FrameBufferSizeEvent& frameBufferSizeEvent);
        void ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent);
        void ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent);

        std::string windowTitle_;
        unsigned int windowWidth_;
        unsigned int windowHeight_;

        unsigned long long desiredFrameDuration_ns_;
        bool sleepyRunLoop_;
        bool fixedSizeFramebuffer_;

        bool running_;

        Window* windowPtr_;
        Renderer* rendererPtr_;
        SoundPlayer* soundPlayerPtr_;
        ComponentStores* componentStoresPtr_;

        std::unordered_map<std::string, Scene*> sceneMap_;
        Scene* activeScenePtr_;

    private:
        bool creationSuccessful_;
    };
}