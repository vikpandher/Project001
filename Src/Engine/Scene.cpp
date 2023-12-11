#include "Scene.h"

#include "Engine/Application.h"
#include "Engine/Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    Scene::~Scene()
    {
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
        {
            AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(sceneId);
            if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
            {
                return iter->second;
            }
            else
            {
                _LOG_ERROR("Application doesn't have a Scene with the name: %u", sceneId);
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
        if (applicationPtr_)
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
        if (applicationPtr_)
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
        if (applicationPtr_)
        {
            AutoIdMap<Scene*>::iterator iter = applicationPtr_->scenePtrMap_.Find(sceneId);
            if (iter != applicationPtr_->scenePtrMap_.IteratorPastTheEnd())
            {
                iter->second->HandleEvent(event);
            }
            else
            {
                _LOG_ERROR("Application doesn't have a Scene with the name: %u", sceneId);
            }
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }
}