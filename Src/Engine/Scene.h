#pragma once

#include <functional>
#include <string>



namespace Project001
{
    class ComponentStores;
    class Renderer;
    class SoundPlayer;
    class Window;

    struct Event;

    class Scene
    {
        friend class Application;

    public:
        Scene();

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        virtual const char* Name() = 0;

        virtual void Initialize() = 0;

        virtual void Deinitialize() = 0;

        virtual void OnEvent(Event& event) = 0;

    protected:
        ComponentStores* GetApplicaitonComponentStoresPtr();

        Renderer* GetApplicationRendererPtr();

        SoundPlayer* GetApplicationSoundPlayerPtr();

        Window* GetApplicationWindowPtr();

        Scene* GetScene(const std::string& name);

        bool IsActiveScene();

        void SendEvent(Event& event);

    private:
        Application* applicationPtr_;

        std::function<void(Event&)> EventCallback;
    };
}