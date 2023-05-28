#pragma once

#include <map>
#include <string>



namespace Project001
{
    class ComponentStores;
    class Renderer;
    class SoundPlayer;
    class Window;

    struct Event;
    struct DeinitializeSceneEvent;
    struct InitializeSceneEvent;
    struct SwitchSceneEvent;
    struct WindowCloseEvent;

    struct ApplicationInfo
    {
        const char* windowTitle;
        unsigned int windowWidth;
        unsigned int windowHeight;
        unsigned int frameBufferWidth;
        unsigned int frameBufferHeight;
        unsigned int indexBufferCapacity;
        unsigned int vertexBufferCapacity;
    };

    class Application
    {
        friend class Scene;

    public:
        Application(const ApplicationInfo& applicationInfo);
        virtual ~Application();

        Application(Application& other) = delete;
        void operator=(const Application&) = delete;

        bool AddScene(Scene* scenePtr);

        void Run();

    protected:
        void OnHandleEvent(Event& event);

        void ProcessDeinitializeSceneEvent(DeinitializeSceneEvent& deinitializeSceneEvent);
        void ProcessInitializeSceneEvent(InitializeSceneEvent& initializeSceneEvent);
        void ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent);
        void ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent);

        std::string windowTitle_;
        unsigned int windowWidth_;
        unsigned int windowHeight_;

        unsigned long desiredFrameDuration_ns_;
        bool sleepyRunLoop_;

        bool running_;

        Window* windowPtr_;
        Renderer* rendererPtr_;
        SoundPlayer* soundPlayerPtr_;
        ComponentStores* componentStoresPtr_;

        Scene* activeScenePtr_;
        std::map<std::string, Scene*> sceneMap_;

    private:
    };
}