#include "Scene.h"

#include "Engine/Application.h"



namespace Project001
{
    Scene::Scene()
        : applicationPtr_(nullptr)
    {}

    Window* Scene::GetApplicationWindowPtr()
    {
        return applicationPtr_->windowPtr_;
    }

    ComponentStores* Scene::GetApplicationComponentStoresPtr()
    {
        return applicationPtr_->componentStoresPtr_;
    }

    MeshStores* Scene::GetApplicationMeshStoresPtr()
    {
        return applicationPtr_->meshStoresPtr_;
    }

    SoundStores* Scene::GetApplicationSoundStoresPtr()
    {
        return applicationPtr_->soundStoresPtr_;
    }

    TextureStores* Scene::GetApplicationTextureStoresPtr()
    {
        return applicationPtr_->textureStoresPtr_;
    }

    Renderer* Scene::GetApplicationRendererPtr()
    {
        return applicationPtr_->rendererPtr_;
    }

    SoundPlayer* Scene::GetApplicationSoundPlayerPtr()
    {
        return applicationPtr_->soundPlayerPtr_;
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