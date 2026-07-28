// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-27

#include "Scene002.h"

#include "Logger.h"
#include "RenderSystem.h"



// public ----------------------------------------------------------------------

Scene002::Scene002(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
{
    sharedDataPtr_ = GetSharedDataPtr<SharedApplicationData>();
    sharedDataPtr_->scene002Id = GetId();
}

Scene002::~Scene002()
{
}

void Scene002::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&Scene002::ProcessInitializeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&Scene002::ProcessDeinitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&Scene002::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&Scene002::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&Scene002::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&Scene002::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected -------------------------------------------------------------------

void Scene002::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   Scene002:            " << GetId());

    // -------------------------------------------------------------------------
}

void Scene002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene002:            " << GetId());

    // -------------------------------------------------------------------------
}

void Scene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    sharedDataPtr_->UpdateKeyboardButtonPresses(keyEvent);
}

void Scene002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    sharedDataPtr_->UpdateMouseButtonPresses(mouseButtonEvent);
}

void Scene002::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    GetRenderSystemPtr()->Render();
}

void Scene002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    sharedDataPtr_->UpdateButtonPressCounts(GetWindowPtr());

    bool startPressed = false;
    for (size_t i = 0; i < sharedDataPtr_->s_player_count; ++i)
    {
        startPressed |= sharedDataPtr_->playerCreationInfos[i].turnedOn && (sharedDataPtr_->playerCreationInfos[i].start_pressCount == 1);
    }
    if (startPressed)
    {
        SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene003Id));
        if (GetActiveScene()->GetId() == sharedDataPtr_->scene003Id)
        {
            SendEventToScene(GetId(), Project001::DeinitializeEvent());
            SendEventToApplication(Project001::InitializeEvent());
        }
        return;
    }
}
