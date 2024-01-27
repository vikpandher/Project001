#pragma once

#include <functional>



namespace Project001
{
    class ComponentStores;
    class Renderer;
    class ResourceStores;
    class SoundPlayer;
    class Window;

    struct Event;

    class Scene
    {
    public:
        friend class Application;

        virtual ~Scene();

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        const unsigned int& GetId();

        virtual void HandleEvent(Event& event) = 0;

    protected:
        Scene(Application* applicationPtr);

        ComponentStores* GetApplicaitonComponentStoresPtr();

        Renderer* GetApplicationRendererPtr();

        ResourceStores* GetApplicationResourceStoresPtr();

        SoundPlayer* GetApplicationSoundPlayerPtr();

        Window* GetApplicationWindowPtr();

        Scene* GetScene(unsigned int sceneId);

        Scene* GetActiveScene();

        void SendEventToApplication(Event& event);

        void SendEventToScene(unsigned int sceneId, Event& event);

        // ---------------------------------------------------------------------

        void SetDesiredFrameDuration_ns(unsigned long long frameDuration_ns);
        unsigned long long GetDesiredFrameDuration_ns() const;

        void SetSleepyRunLoop(bool sleepyRunLoop);
        bool GetSleepyRunLoop() const;

        void SetFixedSizeFramebuffer(bool fixedSizeFramebuffer);
        bool GetFixedSizeFramebuffer() const;

    private:
        Application* applicationPtr_;

        unsigned int id_;
    };

    inline const unsigned int& Scene::GetId()
    {
        return id_;
    }
}