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

        bool IsInitialized();

        bool Initialize();

        bool Deinitialize();

        void HandleEvent(Event& event);

    protected:
        virtual bool OnInitialize() = 0;

        virtual bool OnDeinitialize() = 0;

        virtual void OnHandleEvent(Event& event) = 0;

        ComponentStores* GetApplicaitonComponentStoresPtr();

        Renderer* GetApplicationRendererPtr();

        SoundPlayer* GetApplicationSoundPlayerPtr();

        Window* GetApplicationWindowPtr();

        Scene* GetScene(const std::string& name);

        Scene* GetActiveScene();

        bool IsActiveScene();

        void SendEvent(Event& event);

    private:
        Application* applicationPtr_;

        std::function<void(Event&)> EventCallback;

        bool initialized_;
    };

    inline bool Scene::IsInitialized()
    {
        return initialized_;
    }

    inline bool Scene::Initialize()
    {
        if (!initialized_ && OnInitialize())
        {
            initialized_ = true;
            return true;
        }
        return false;
    }

    inline bool Scene::Deinitialize()
    {
        if (initialized_ && OnDeinitialize())
        {
            initialized_ = false;
            return true;
        }
        return false;
    }

    inline void Scene::HandleEvent(Event& event)
    {
        if (initialized_)
        {
            OnHandleEvent(event);
        }
    }
}