#pragma once

#include "Application.h"

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
    public:
        virtual ~Scene();

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        const std::string& GetName();

        virtual void HandleEvent(Event& event) = 0;

    protected:
        Scene(Application* applicationPtr, const std::string& name);

        ComponentStores* GetApplicaitonComponentStoresPtr();

        Renderer* GetApplicationRendererPtr();

        SoundPlayer* GetApplicationSoundPlayerPtr();

        Window* GetApplicationWindowPtr();

        Scene* GetScene(const std::string& name);

        Scene* GetActiveScene();

        void SendEventToApplication(Event& event);

        void SendEventToScene(const std::string& name, Event& event);

    private:
        friend void Application::NullifySceneApplicationPtr(Scene* scene);

        Application* applicationPtr_;

        std::string name_;
    };

    inline const std::string& Scene::GetName()
    {
        return name_;
    }
}