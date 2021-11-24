#pragma once

#include <map>
#include <string>



namespace Project001
{
    class ComponentStores;
    class MeshStores;
    class Renderer;
    class TextureStores;
    class Window;

    struct Event;
    struct DeinitializeSceneEvent;
    struct InitializeSceneEvent;
    struct SwitchSceneEvent;
    struct WindowCloseEvent;

    class Application
    {
        friend class Scene;

    public:
        Application(const char* windowTitile, unsigned int windowWidth, unsigned int windowHeight);
        virtual ~Application();

        Application(Application& other) = delete;
        void operator=(const Application&) = delete;

        void AddScene(Scene* scenePtr);

        void Run();

    protected:
        void ProcessDeinitializeSceneEvent(DeinitializeSceneEvent& deinitializeSceneEvent);
        void ProcessInitializeSceneEvent(InitializeSceneEvent& initializeSceneEvent);
        void ProcessSwitchSceneEvent(SwitchSceneEvent& switchSceneEvent);
        void ProcessWindowCloseEvent(WindowCloseEvent& windowCloseEvent);

        void OnEvent(Event& event);

        std::string windowTitle_;
        unsigned int windowWidth_;
        unsigned int windowHeight_;

        double desiredFrameDuration_ms_;
        bool sleepyRunLoop_;

        bool running_;
        bool paused_;

        Window* windowPtr_;

        ComponentStores* componentStoresPtr_;
        MeshStores* meshStoresPtr_;
        TextureStores* textureStoresPtr_;

        Renderer* rendererPtr_;

        Scene* activeScenePtr_;
        std::map<std::string, Scene*> sceneMap_;

    private:
    };
}