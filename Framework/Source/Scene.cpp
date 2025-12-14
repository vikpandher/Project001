// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#include "Scene.h"

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
        , id_(static_cast<unsigned int>(-1))
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
            LOG_ERROR_F("Application pointer is null");
        }
    }
}