#include "Scene.h"

#include "Application.h"
#include "Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    Scene::~Scene()
    {
        if (applicationPtr_ != nullptr)
        {
            // Remove this from its Application's sceneMap.
            AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(id_);
            if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
            {
                applicationPtr_->scenePtrMap_.Erase(iter);
            }
            if (applicationPtr_->activeScenePtr_ == this)
            {
                applicationPtr_->activeScenePtr_ = nullptr;
            }
        }
    }

    // protected ---------------------------------------------------------------

    Scene::Scene(Application* applicationPtr)
        : applicationPtr_(applicationPtr)
        , id_((unsigned int)-1)
    {
        if (applicationPtr_ != nullptr)
        {
            applicationPtr_->scenePtrMap_.Add(id_, this);

            if (applicationPtr_->scenePtrMap_.Size() == 1)
            {
                applicationPtr_->activeScenePtr_ = this;
            }
        }
        else
        {
            _LOG_ERROR("Application pointer is null");
        }
    }

    ComponentStores* Scene::GetApplicaitonComponentStoresPtr()
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->componentStoresPtr_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    Renderer* Scene::GetApplicationRendererPtr()
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->rendererPtr_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    ResourceStores* Scene::GetApplicationResourceStoresPtr()
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->resourceStoresPtr_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    SoundPlayer* Scene::GetApplicationSoundPlayerPtr()
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->soundPlayerPtr_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    Window* Scene::GetApplicationWindowPtr()
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->windowPtr_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    Scene* Scene::GetScene(unsigned int sceneId)
    {
        if (applicationPtr_ != nullptr)
        {
            AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(sceneId);
            if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
            {
                return iter->second;
            }
            else
            {
                _LOG_ERROR("Application doesn't have a Scene with the id: %u", sceneId);
            }
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    Scene* Scene::GetActiveScene()
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->activeScenePtr_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return nullptr;
    }

    void Scene::SendEventToApplication(Event& event)
    {
        if (applicationPtr_ != nullptr)
        {
            applicationPtr_->HandleEvent(event);
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }

    void Scene::SendEventToScene(unsigned int sceneId, Event& event)
    {
        if (applicationPtr_ != nullptr)
        {
            AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(sceneId);
            if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
            {
                iter->second->HandleEvent(event);
            }
            else
            {
                _LOG_ERROR("Application doesn't have a Scene with the id: %u", sceneId);
            }
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }

    void Scene::SetDesiredFrameDuration_ns(unsigned long long frameDuration_ns)
    {
        if (applicationPtr_ != nullptr)
        {
            applicationPtr_->desiredFrameDuration_ns_ = frameDuration_ns;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }

    unsigned long long Scene::GetDesiredFrameDuration_ns() const
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->desiredFrameDuration_ns_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return 0ull;
    }

    void Scene::SetSleepyRunLoop(bool sleepyRunLoop)
    {
        if (applicationPtr_ != nullptr)
        {
            applicationPtr_->sleepyRunLoop_ = sleepyRunLoop;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }

    bool Scene::GetSleepyRunLoop() const
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->sleepyRunLoop_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return false;
    }

    void Scene::SetFixedSizeFramebuffer(bool fixedSizeFramebuffer)
    {
        if (applicationPtr_ != nullptr)
        {
            applicationPtr_->fixedSizeFramebuffer_ = fixedSizeFramebuffer;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }

    bool Scene::GetFixedSizeFramebuffer() const
    {
        if (applicationPtr_ != nullptr)
        {
            return applicationPtr_->fixedSizeFramebuffer_;
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
        return false;
    }
}