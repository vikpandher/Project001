#include "Scene.h"

#include "Engine/Logger.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    Scene::~Scene()
    {
        if (applicationPtr_)
        {
            // Remove this from its Application's sceneMap.
            if (applicationPtr_->sceneMap_.find(name_) != applicationPtr_->sceneMap_.end())
            {
                applicationPtr_->sceneMap_.erase(name_);
            }
            if (applicationPtr_->activeScenePtr_ == this)
            {
                applicationPtr_->activeScenePtr_ = nullptr;
            }
        }
    }

    // protected ---------------------------------------------------------------

    Scene::Scene(Application* applicationPtr, const std::string& name)
        : applicationPtr_(applicationPtr)
        , name_(name)
    {
        if (applicationPtr_)
        {
            if (applicationPtr_->sceneMap_.find(name_) == applicationPtr_->sceneMap_.end())
            {
                applicationPtr_->sceneMap_[name_] = this;

                if (applicationPtr_->sceneMap_.size() == 1)
                {
                    applicationPtr_->activeScenePtr_ = this;
                }
            }
            else
            {
                _LOG_ERROR("Application already has a Scene with the name: %s", name_.c_str());
                applicationPtr_ = nullptr;
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

    Scene* Scene::GetScene(const std::string& name)
    {
        if (applicationPtr_)
        {
            if (applicationPtr_->sceneMap_.find(name) != applicationPtr_->sceneMap_.end())
            {
                return applicationPtr_->sceneMap_[name];
            }
            else
            {
                _LOG_ERROR("Application doesn't have a Scene with the name: %s", name.c_str());
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

    void Scene::SendEventToScene(const std::string& name, Event& event)
    {
        if (applicationPtr_)
        {
            if (applicationPtr_->sceneMap_.find(name) != applicationPtr_->sceneMap_.end())
            {
                applicationPtr_->sceneMap_[name]->HandleEvent(event);
            }
            else
            {
                _LOG_ERROR("Application doesn't have a Scene with the name: %s", name.c_str());
            }
        }
        else
        {
            _LOG_ERROR("Application pointer is nullptr");
        }
    }
}