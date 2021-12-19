#pragma once

#include <functional>



namespace Project001
{
    class ComponentStores;
    class MeshStores;
    class Renderer;
    class SoundPlayer;
    class SoundStores;
    class TextureStores;
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
        Window* GetApplicationWindowPtr();

        ComponentStores* GetApplicationComponentStoresPtr();

        MeshStores* GetApplicationMeshStoresPtr();

        SoundStores* GetApplicationSoundStoresPtr();

        TextureStores* GetApplicationTextureStoresPtr();

        Renderer* GetApplicationRendererPtr();

        SoundPlayer* GetApplicationSoundPlayerPtr();

        bool IsActiveScene();

        void SendEvent(Event& event);

    private:
        Application* applicationPtr_;

        std::function<void(Event&)> EventCallback;

    };
}