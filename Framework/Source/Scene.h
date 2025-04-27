// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-26

#pragma once

#include "Event.h"

#include <functional>
#include <typeinfo>



namespace Project001
{
    class ComponentStores;
    class Renderer;
    class ResourceStores;
    class SoundPlayer;
    class Window;

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

        template <typename SharedDataType>
        SharedDataType* GetApplicationSharedDataPtr();

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
        void* GetApplicationSharedDataPtr_H(size_t sharedDataTypeId);

        Application* applicationPtr_;

        unsigned int id_;
    };

    // protected ---------------------------------------------------------------

    inline const unsigned int& Scene::GetId()
    {
        return id_;
    }

    template <typename SharedDataType>
    SharedDataType* Scene::GetApplicationSharedDataPtr()
    {
        size_t sharedDataTypeId = typeid(SharedDataType).hash_code();
        return (SharedDataType*)GetApplicationSharedDataPtr_H(sharedDataTypeId);
    }
}