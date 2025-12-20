// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-20

#pragma once

#include "Application.h"
#include "Event.h"

#include <functional>
#include <typeinfo>



namespace Project001
{
    class Scene
    {
    public:
        virtual ~Scene();

        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        const unsigned int& GetId() const;

        virtual void HandleEvent(Event& event) = 0;

    protected:
        Scene(Application* applicationPtr);

        Application* GetApplicationPtr();

        template <typename SharedDataType>
        SharedDataType* GetSharedDataPtr();

        template <typename SharedDataType>
        void SetSharedDataPtr(SharedDataType* sharedDataPtr);

        ComponentStores* GetComponentStoresPtr();

        Renderer* GetRendererPtr();

        SoundPlayer* GetSoundPlayerPtr();

        Window* GetWindowPtr();

        CollisionSystem2D* GetCollisionSystemPtr();

        RenderSystem* GetRenderSystemPtr();

        Scene* GetScene(unsigned int sceneId);

        Scene* GetActiveScene();

        void SendEventToApplication(Event& event);

        bool SendEventToScene(unsigned int sceneId, Event& event);

        void SetDesiredFrameDuration_ns(unsigned long long frameDuration_ns);
        unsigned long long GetDesiredFrameDuration_ns() const;

        bool GetSleepyRunLoop() const;
        void SetSleepyRunLoop(bool sleepyRunLoop);

        bool GetFixedSizeFramebuffer() const;
        void SetFixedSizeFramebuffer(bool fixedSizeFramebuffer);

    private:
        Application* applicationPtr_;

        unsigned int id_;
    };

    // public ------------------------------------------------------------------

    inline const unsigned int& Scene::GetId() const
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
            return reinterpret_cast<SharedDataType*>(applicationPtr_->sharedDataPtr_);
        }
        else
        {
            return nullptr;
        }
    }

    template <typename SharedDataType>
    void Scene::SetSharedDataPtr(SharedDataType* sharedDataPtr)
    {
        applicationPtr_->sharedDataTypeId_ = typeid(SharedDataType).hash_code();
        applicationPtr_->sharedDataPtr_ = sharedDataPtr;
    }

    inline ComponentStores* Scene::GetComponentStoresPtr()
    {
        return applicationPtr_->componentStoresPtr_;
    }

    inline Renderer* Scene::GetRendererPtr()
    {
        return applicationPtr_->rendererPtr_;
    }

    inline SoundPlayer* Scene::GetSoundPlayerPtr()
    {
        return applicationPtr_->soundPlayerPtr_;
    }

    inline Window* Scene::GetWindowPtr()
    {
        return applicationPtr_->windowPtr_;
    }

    inline CollisionSystem2D* Scene::GetCollisionSystemPtr()
    {
        return applicationPtr_->collisionSystemPtr_;
    }

    inline RenderSystem* Scene::GetRenderSystemPtr()
    {
        return applicationPtr_->renderSystemPtr_;
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

    inline bool Scene::GetSleepyRunLoop() const
    {
        return applicationPtr_->sleepyRunLoop_;
    }

    inline void Scene::SetSleepyRunLoop(bool sleepyRunLoop)
    {
        applicationPtr_->sleepyRunLoop_ = sleepyRunLoop;
    }

    inline bool Scene::GetFixedSizeFramebuffer() const
    {
        return applicationPtr_->fixedSizeFramebuffer_;
    }

    inline void Scene::SetFixedSizeFramebuffer(bool fixedSizeFramebuffer)
    {
        applicationPtr_->fixedSizeFramebuffer_ = fixedSizeFramebuffer;
    }
}