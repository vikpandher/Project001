// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-05-04

#pragma once

#include "Application.h"
#include "Event.h"

#include <functional>
#include <typeinfo>



namespace Project001
{
    class Application;
    class ComponentStores;
    class Renderer;
    class ResourceStores;
    class SoundPlayer;
    class Window;

    class Scene
    {
    public:
        virtual ~Scene();

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        const unsigned int& GetId();

        virtual void HandleEvent(Event& event) = 0;

    protected:
        Scene(Application* applicationPtr);

        Application* GetApplicationPtr();

        template <typename SharedDataType>
        SharedDataType* GetSharedDataPtr();

        ComponentStores* GetComponentStoresPtr();

        Renderer* GetRendererPtr();

        ResourceStores* GetResourceStoresPtr();

        SoundPlayer* GetSoundPlayerPtr();

        Window* GetWindowPtr();

        Scene* GetScene(unsigned int sceneId);

        Scene* GetActiveScene();

        void SendEventToApplication(Event& event);

        bool SendEventToScene(unsigned int sceneId, Event& event);

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

    // public ------------------------------------------------------------------

    inline const unsigned int& Scene::GetId()
    {
        return id_;
    }

    // protected ---------------------------------------------------------------

    inline Application* Scene::GetApplicationPtr()
    {
        return applicationPtr_;
    }

    template <typename SharedDataType>
    inline SharedDataType* Scene::GetSharedDataPtr()
    {
        size_t sharedDataTypeId = typeid(SharedDataType).hash_code();
        if (applicationPtr_->sharedDataTypeId_ == sharedDataTypeId)
        {
            return (SharedDataType*)applicationPtr_->sharedDataPtr_;
        }
        else
        {
            return nullptr;
        }
    }

    inline ComponentStores* Scene::GetComponentStoresPtr()
    {
        return applicationPtr_->componentStoresPtr_;
    }

    inline Renderer* Scene::GetRendererPtr()
    {
        return applicationPtr_->rendererPtr_;
    }

    inline ResourceStores* Scene::GetResourceStoresPtr()
    {
        return applicationPtr_->resourceStoresPtr_;
    }

    inline SoundPlayer* Scene::GetSoundPlayerPtr()
    {
        return applicationPtr_->soundPlayerPtr_;
    }

    inline Window* Scene::GetWindowPtr()
    {
        return applicationPtr_->windowPtr_;
    }

    inline Scene* Scene::GetScene(unsigned int sceneId)
    {
        AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(sceneId);
        if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
        {
            return iter->second;
        }
        else
        {
            return nullptr;
        }
    }

    inline Scene* Scene::GetActiveScene()
    {
        return applicationPtr_->activeScenePtr_;
    }

    inline void Scene::SendEventToApplication(Event& event)
    {
        applicationPtr_->HandleEvent(event);
    }

    inline bool Scene::SendEventToScene(unsigned int sceneId, Event& event)
    {
        AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(sceneId);
        if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
        {
            iter->second->HandleEvent(event);
            return true;
        }
        else
        {
            return false;
        }
    }

    inline void Scene::SetDesiredFrameDuration_ns(unsigned long long frameDuration_ns)
    {
        applicationPtr_->desiredFrameDuration_ns_ = frameDuration_ns;
    }

    inline unsigned long long Scene::GetDesiredFrameDuration_ns() const
    {
        return applicationPtr_->desiredFrameDuration_ns_;
    }

    inline void Scene::SetSleepyRunLoop(bool sleepyRunLoop)
    {
        applicationPtr_->sleepyRunLoop_ = sleepyRunLoop;
    }

    inline bool Scene::GetSleepyRunLoop() const
    {
        return applicationPtr_->sleepyRunLoop_;
    }

    inline void Scene::SetFixedSizeFramebuffer(bool fixedSizeFramebuffer)
    {
        applicationPtr_->fixedSizeFramebuffer_ = fixedSizeFramebuffer;
    }

    inline bool Scene::GetFixedSizeFramebuffer() const
    {
        return applicationPtr_->fixedSizeFramebuffer_;
    }
}