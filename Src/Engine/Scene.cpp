#include "Scene.h"

#include "Engine/Application.h"



namespace Project001
{
    Scene::Scene()
        : applicationPtr_(nullptr)
    {}

    ComponentStores* Scene::GetApplicaitonComponentStoresPtr()
    {
        return applicationPtr_->componentStoresPtr_;
    }

    Renderer* Scene::GetApplicationRendererPtr()
    {
        return applicationPtr_->rendererPtr_;
    }

    SoundPlayer* Scene::GetApplicationSoundPlayerPtr()
    {
        return applicationPtr_->soundPlayerPtr_;
    }

    Window* Scene::GetApplicationWindowPtr()
    {
        return applicationPtr_->windowPtr_;
    }

    Scene* Scene::GetScene(const std::string& name)
    {
        if (applicationPtr_->sceneMap_.find(name) != applicationPtr_->sceneMap_.end())
        {
            return applicationPtr_->sceneMap_[name];
        }

        return nullptr;
    }

    bool Scene::IsActiveScene()
    {
        return applicationPtr_->activeScenePtr_ == this;
    }

    void Scene::SendEvent(Event& event)
    {
        return EventCallback(event);
    }
}