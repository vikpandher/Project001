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

    TextureStores* Scene::GetApplicationTextureStoresPtr()
    {
        return applicationPtr_->textureStoresPtr_;
    }

    Renderer* Scene::GetApplicationRendererPtr()
    {
        return applicationPtr_->rendererPtr_;
    }
}