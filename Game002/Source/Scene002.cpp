// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#include "Scene002.h"

#include "Scene003.h"

#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "Components/RenderedModel.h"
#include "ComponentStores.h"
#include "Utilities/FindUtility.h"
#include "Utilities/FontUtility.h"
#include "Logger.h"
#include "Window.h"



// public ----------------------------------------------------------------------

Scene002::Scene002(Project001::Application* applicationPtr)
    : BaseScene001(applicationPtr)
{
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
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&Scene002::ProcessUpdateEvent, this, std::placeholders::_1));

    BaseScene001::HandleEvent(event);
}

// protected -------------------------------------------------------------------

void Scene002::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   Scene002:            " << GetId());

    // -------------------------------------------------------------------------

    for (size_t i = 0; i < SharedApplicationData::s_player_count; ++i)
    {
        PlayerCreationInfo& playerCreationInfo = sharedDataPtr_->playerCreationInfos[i];
        if (!playerCreationInfo.turnedOn)
        {
            playerCreationInfo.controlSchemeIndex = 0;
        }
    }

    CreateUiCameraEntity(uiCamera_entityId_, s_uiCamera_cameraMask_, 1000);
    CreateUiCameraEntity(backUiCamera_entityId_, s_backUiCamera_cameraMask_, -1000);
    CreateMainCameraEntity(mainCamera_entityId_, s_mainCamera_cameraMask_, 0);

    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_entityId_));
        if (cameraPtr != nullptr)
        {
            cameraPtr->ResetOrientation();
            cameraPtr->AddPitch(0.4f * glm::pi<float>());
            cameraPtr->AddYaw(glm::pi<float>());
            cameraPtr->FollowFocalPoint(s_lookAtPoint_, s_distanceFromFocalPoint_);
        }
    };

    CreateBackgroundEntity();
    CreateLightEntity();
    CreateMenuEntity();

    for (size_t i = 0; i < SharedApplicationData::s_player_count; ++i)
    {
        PlayerCreationInfo& playerCreationInfo = sharedDataPtr_->playerCreationInfos[i];
        CreatePenguinEntity(player_entityIds_[i], playerCreationInfo.playerNumber);
    }

    CreateSharkEntity(shark_entityId_);
}

void Scene002::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    LOG_INFO("DEINITIALIZING: Scene002:            " << GetId());

    // -------------------------------------------------------------------------

    GetComponentStoresPtr()->DeleteAllEntities();

    // Entity Ids --------------------------------------------------------------

    backUiCamera_entityId_ = static_cast<unsigned int>(-1);
    mainCamera_entityId_ = static_cast<unsigned int>(-1);
    uiCamera_entityId_ = static_cast<unsigned int>(-1);

    background_entityId_ = static_cast<unsigned int>(-1);
    light_entityId_ = static_cast<unsigned int>(-1);
    menuText_entityId_ = static_cast<unsigned int>(-1);

    for (size_t i = 0; i < SharedApplicationData::s_player_count; ++i)
    {
        player_entityIds_[i] = static_cast<unsigned int>(-1);
    }

    shark_entityId_ = static_cast<unsigned int>(-1);
}

void Scene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    sharedDataPtr_->UpdateKeyboardButtonPresses(keyEvent);
}

void Scene002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    sharedDataPtr_->UpdateMouseButtonPresses(mouseButtonEvent);
}

void Scene002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    sharedDataPtr_->UpdateButtonPressCounts(GetWindowPtr());

    unsigned long long timestep_ns = updateEvent.timestep_ns;
    float timestep_s = static_cast<float>(timestep_ns) / 1e9f;

    bool quit = false;
    UpdateMenuTextEntity(timestep_s, quit);
    if (quit)
    {
        return;
    }

    UpdateMainCameraEntity(timestep_s);
}

void Scene002::CreateBackgroundEntity()
{
    GetComponentStoresPtr()->CreateEntity(background_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(background_entityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, background_entityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_backUiCamera_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(1);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[0];
            mesh.SetCameraMask(s_backUiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuBackground_meshDataPtr);
            mesh.SetPositionY(64.0f);
            mesh.SetColor(0.9f, 0.9f, 0.9f, 1.0f);
            mesh.SetUseLighting(false);
        }
    }
}

void Scene002::CreateLightEntity()
{
    GetComponentStoresPtr()->CreateEntity(light_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::LightSource>(light_entityId_));
    Project001::LightSource* lightSourcePtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::LightSource>(lightSourcePtr, light_entityId_));
    if (lightSourcePtr != nullptr)
    {
        lightSourcePtr->SetCameraMask(s_mainCamera_cameraMask_);
        lightSourcePtr->SetPosition(512.0f, -512.0f, 2048.0f);
        lightSourcePtr->SetAmbientColor(0.6f, 0.6f, 0.6f);
        lightSourcePtr->SetDiffuseColor(0.4f, 0.4f, 0.4f);
        lightSourcePtr->TurnOn();
    }
}

void Scene002::CreateMenuEntity()
{
    GetComponentStoresPtr()->CreateEntity(menuText_entityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(menuText_entityId_));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, menuText_entityId_));
    if (renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_uiCamera_cameraMask_);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(12);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[0];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuTitleText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionY(224.0f);
            mesh.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[1];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuTitleText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionX(4.0f);
            mesh.SetPositionY(220.0f);
            mesh.SetColor(0.3f, 0.3f, 0.6f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(0);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[2];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuPlayerText1_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionY(-96.0f);
            mesh.SetColor(0.7f, 0.7f, 1.0f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[3];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuPlayerText2_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionY(-128.0f);
            mesh.SetColor(1.0f, 0.7f, 0.7f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[4];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuPlayerText3_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionY(-160.0f);
            mesh.SetColor(0.7f, 1.0f, 0.7f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[5];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuPlayerText4_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionY(-192.0f);
            mesh.SetColor(1.0f, 0.7f, 1.0f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[6];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuStartText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetPositionY(-232.0f);
            mesh.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[7];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuLeftText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);

            mesh.SetPositionX(-96.0f - 8.0f);
            mesh.SetPositionY(128.0f + 18.0f);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[8];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuRightText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);

            mesh.SetPositionX(96.0f + 8.0f);
            mesh.SetPositionY(128.0f + 18.0f);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[9];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuAuthorText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);

            mesh.SetPositionX(480.0f - 8.0f);
            mesh.SetPositionY(-320.0f + 8.0f);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[10];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuConfigFileFoundText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            if (!sharedDataPtr_->configFileFound_)
            {
                mesh.SetColor(0.6f, 0.3f, 0.3f, 1.0f);
            }
            else
            {
                mesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            }
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);

            mesh.SetPositionX(-480.0f + 8.0f);
            mesh.SetPositionY(320.0f - 8.0f - 16.0f);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[11];
            mesh.SetCameraMask(s_uiCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->uiMenuVersionText_meshDataPtr);
            mesh.SetTextureId(sharedDataPtr_->pixelFont_textureId);
            mesh.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
            mesh.SetTranslucent(true);
            mesh.SetUseLighting(false);
            mesh.SetRenderPriorityOverride(1);

            mesh.SetPositionX(-480.0f + 8.0f);
            mesh.SetPositionY(-320.0f + 8.0f);
        }
    }
}

void Scene002::CreatePenguinEntity(unsigned int& entityId, size_t playerNumber)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<PenguinInfo>(entityId));
    PenguinInfo* penguinInfoPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<PenguinInfo>(penguinInfoPtr, entityId));

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
    if (penguinInfoPtr != nullptr && renderedModelPtr != nullptr)
    {
        penguinInfoPtr->playerNumber = playerNumber;

        unsigned int textureId = static_cast<unsigned int>(-1);
        if (penguinInfoPtr->playerNumber == 0)
        {
            textureId = sharedDataPtr_->penguin_textureId;
        }
        else if (penguinInfoPtr->playerNumber == 1)
        {
            textureId = sharedDataPtr_->penguin_v2_textureId;
        }
        else if (penguinInfoPtr->playerNumber == 2)
        {
            textureId = sharedDataPtr_->penguin_v3_textureId;
        }
        else if (penguinInfoPtr->playerNumber == 3)
        {
            textureId = sharedDataPtr_->penguin_v4_textureId;
        }

        if (penguinInfoPtr->playerNumber == sharedDataPtr_->coolGlassesPlayerIndex)
        {
            penguinInfoPtr->glassesType = 0;
        }
        else
        {
            penguinInfoPtr->glassesType = 1;
        }

        renderedModelPtr->SetCameraMask(s_mainCamera_cameraMask_);
        renderedModelPtr->AddRelativeRotationZ(glm::three_over_two_pi<float>());
        renderedModelPtr->SetVisible(false);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(PenguinInfo::s_renderedMeshCount);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_body_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_body_meshId, sharedDataPtr_->penguin_body_meshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(textureId);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_right_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_flipper_right_meshId, sharedDataPtr_->penguin_flipper_right_meshDataPtr->maxBoundingRadius);
            mesh.SetPosition(9.0f, 0.0f, 23.0f);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_flipper_left_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_flipper_left_meshId, sharedDataPtr_->penguin_flipper_left_meshDataPtr->maxBoundingRadius);
            mesh.SetPosition(-9.0f, 0.0f, 23.0f);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_right_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_foot_right_meshId, sharedDataPtr_->penguin_foot_right_meshDataPtr->maxBoundingRadius);
            mesh.SetPosition(7.0f, -2.0f, 3.0f);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_foot_left_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_foot_left_meshId, sharedDataPtr_->penguin_foot_left_meshDataPtr->maxBoundingRadius);
            mesh.SetPosition(-7.0f, -2.0f, 3.0f);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_head_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_head_meshId, sharedDataPtr_->penguin_head_meshDataPtr->maxBoundingRadius);
            mesh.SetPosition(0.0f, 0.0f, 26.0f);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_body_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_eyes_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_eyes_meshId, sharedDataPtr_->penguin_eyes_meshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_head_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_beak_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_beak_meshId, sharedDataPtr_->penguin_beak_meshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_head_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_glasses_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            if (penguinInfoPtr->glassesType == 0)
            {
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_glasses_meshId, sharedDataPtr_->penguin_glasses_meshDataPtr->maxBoundingRadius);
            }
            else
            {
                mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->penguin_glasses_v2_meshId, sharedDataPtr_->penguin_glasses_v2_meshDataPtr->maxBoundingRadius);
            }
            mesh.SetTextureId(textureId);
            mesh.SetParentMeshIndex(PenguinInfo::s_head_renderedMeshIndex);
            mesh.SetColorAlpha(sharedDataPtr_->penguinGlassesAlpha);
            mesh.SetTranslucent(true);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[PenguinInfo::s_shadow_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshDataPtr(sharedDataPtr_->circle_meshDataPtr);
            mesh.SetScale(glm::vec3(SharedApplicationData::s_penguin_collisionRadius));
            mesh.SetPositionZ(0.1f);
            mesh.SetColor(0.0f, 0.0f, 0.0f, 0.8f);
            mesh.SetTranslucent(true);
            mesh.SetRenderPriorityOverride(2);
        }
    }
}

void Scene002::CreateSharkEntity(unsigned int& entityId)
{
    GetComponentStoresPtr()->CreateEntity(entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<SharkInfo>(entityId));
    SharkInfo* sharkInfoPtr = nullptr;
    GetComponentStoresPtr()->GetComponent<SharkInfo>(sharkInfoPtr, entityId);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedModel>(entityId));
    Project001::RenderedModel* renderedModelPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, entityId));
    if (sharkInfoPtr != nullptr && renderedModelPtr != nullptr)
    {
        renderedModelPtr->SetCameraMask(s_mainCamera_cameraMask_);
        renderedModelPtr->AddRelativeRotationZ(glm::three_over_two_pi<float>());
        renderedModelPtr->SetVisible(false);
        std::vector<Project001::RenderedMesh>& renderedMeshes = renderedModelPtr->GetRenderedMeshes();
        renderedMeshes.resize(SharkInfo::s_renderedMeshCount);

        renderedModelPtr->SetPositionZ(24.0f);

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_front_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->shark_front_meshId, sharedDataPtr_->shark_front_meshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(sharedDataPtr_->shark_textureId);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_back_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->shark_back_meshId, sharedDataPtr_->shark_back_meshDataPtr->maxBoundingRadius);
            mesh.SetTextureId(sharedDataPtr_->shark_textureId);
            mesh.SetParentMeshIndex(SharkInfo::s_front_renderedMeshIndex);
        }

        {
            Project001::RenderedMesh& mesh = renderedMeshes[SharkInfo::s_jaw_renderedMeshIndex];
            mesh.SetCameraMask(s_mainCamera_cameraMask_);
            mesh.SetMeshIdAndMaxBoundingRadius(sharedDataPtr_->shark_jaw_meshId, sharedDataPtr_->shark_jaw_meshDataPtr->maxBoundingRadius);
            mesh.SetPosition(0.0f, 48.0f, 5.0f);
            mesh.SetTextureId(sharedDataPtr_->shark_textureId);
            mesh.SetParentMeshIndex(SharkInfo::s_front_renderedMeshIndex);
        }
    }
}

void Scene002::UpdateMenuTextEntity(float timestep_s, bool& quit)
{
    std::vector<size_t> usedControlSchemeIndices;

    for (size_t i = 0; i < SharedApplicationData::s_player_count; ++i)
    {
        const PlayerCreationInfo& playerCreationInfo = sharedDataPtr_->playerCreationInfos[i];
        const size_t& currentControlSchemeIndex = playerCreationInfo.controlSchemeIndex;

        if (Project001::Find::Contains(usedControlSchemeIndices, currentControlSchemeIndex))
        {
            continue; // avoid the duplication of presses when two players have the same control scheme
        }
        else
        {
            usedControlSchemeIndices.push_back(currentControlSchemeIndex);
        }

        const ControlSchemeInfo& controlSchemeInfo = sharedDataPtr_->controlSchemeInfos[currentControlSchemeIndex];

        if (controlSchemeInfo.grab_pressCount == 1)
        {
            if (playerCreationInfo.turnedOn && menuCursorPosition_ == 4)
            {
                SendEventToApplication(Project001::SwitchSceneEvent(sharedDataPtr_->scene003Id));
                if (GetActiveScene()->GetId() == sharedDataPtr_->scene003Id)
                {
                    SendEventToScene(GetId(), Project001::DeinitializeEvent());
                    SendEventToApplication(Project001::InitializeEvent());
                }
                quit = true;
                return;
            }
        }
    }

    glm::vec2 moveDirection(0.0f, 0.0);

    for (size_t i = 0; i < SharedApplicationData::s_controlScheme_count; ++i)
    {
        const ControlSchemeInfo& controlSchemeInfo = sharedDataPtr_->controlSchemeInfos[i];

        if (controlSchemeInfo.left_pressCount == 1)
        {
            moveDirection.x -= 1.0f;
        }
        if (controlSchemeInfo.right_pressCount == 1)
        {
            moveDirection.x += 1.0f;
        }
        if (controlSchemeInfo.up_pressCount == 1)
        {
            moveDirection.y += 1.0f;
        }
        if (controlSchemeInfo.down_pressCount == 1)
        {
            moveDirection.y -= 1.0f;
        }

        if (menuAxisMoveTimes_s[i] > 0.0f)
        {
            if (glm::abs(controlSchemeInfo.leftRightAxisValue) < controlSchemeInfo.axisDeadzone &&
                glm::abs(controlSchemeInfo.upDownAxisValue) < controlSchemeInfo.axisDeadzone)
            {
                menuAxisMoveTimes_s[i] = 0.0f;
            }
            else
            {
                menuAxisMoveTimes_s[i] -= timestep_s;
            }
        }
        else
        {
            menuAxisMoveTimes_s[i] = 0.0f;

            if (glm::abs(controlSchemeInfo.leftRightAxisValue) > controlSchemeInfo.axisDeadzone)
            {
                moveDirection.x += controlSchemeInfo.leftRightAxisValue;
                menuAxisMoveTimes_s[i] += s_menuAxisMoveDelay_s_;
            }
            if (glm::abs(controlSchemeInfo.upDownAxisValue) > controlSchemeInfo.axisDeadzone)
            {
                moveDirection.y += controlSchemeInfo.upDownAxisValue;
                menuAxisMoveTimes_s[i] += s_menuAxisMoveDelay_s_;
            }
        }
    }

    if (menuCursorPosition_ < SharedApplicationData::s_player_count)
    {
        int currentControlSchemeIndex = static_cast<int>(sharedDataPtr_->playerCreationInfos[menuCursorPosition_].controlSchemeIndex);

        if (moveDirection.x > 0.0f)
        {
            currentControlSchemeIndex++;
        }
        else if (moveDirection.x < 0.0f)
        {
            currentControlSchemeIndex--;
        }

        if (currentControlSchemeIndex >= static_cast<int>(SharedApplicationData::s_controlScheme_count))
        {
            currentControlSchemeIndex = 0;
        }
        else if (currentControlSchemeIndex < 0)
        {
            currentControlSchemeIndex = static_cast<int>(SharedApplicationData::s_controlScheme_count) - 1;
        }

        sharedDataPtr_->playerCreationInfos[menuCursorPosition_].controlSchemeIndex = static_cast<size_t>(currentControlSchemeIndex);

        sharedDataPtr_->playerCreationInfos[menuCursorPosition_].turnedOn =
            sharedDataPtr_->playerCreationInfos[menuCursorPosition_].controlSchemeIndex != ControlSchemeInfo::s_controlSchemeIndex_unknown;
    }

    if (moveDirection.y > 0.0f && menuCursorPosition_ > 0)
    {
        menuCursorPosition_ -= 1;
    }
    else if (moveDirection.y < 0.0f && menuCursorPosition_ < 4)
    {
        menuCursorPosition_ += 1;
    }

    UpdateMenuTextMeshes();

    for (size_t i = 0; i < SharedApplicationData::s_player_count; ++i)
    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, player_entityIds_[i]));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetVisible(menuCursorPosition_ == i);
        }
    }

    {
        Project001::RenderedModel* renderedModelPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedModel>(renderedModelPtr, shark_entityId_));
        if (renderedModelPtr != nullptr)
        {
            renderedModelPtr->SetVisible(menuCursorPosition_ == 4);
        }
    }
}

void Scene002::UpdateMenuTextMeshes()
{
    constexpr float titlePixelSize = 5.0f;

    sharedDataPtr_->uiMenuTitleText_meshDataPtr->Clear();

    std::string titleString = "COOL PENGUIN SNOWBALL";

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuTitleText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        titleString,
        titlePixelSize,
        1
    ));

    // -------------------------------------------------------------------------

    constexpr float playerPixelSize = 2.0f;

    sharedDataPtr_->uiMenuPlayerText1_meshDataPtr->Clear();
    sharedDataPtr_->uiMenuPlayerText2_meshDataPtr->Clear();
    sharedDataPtr_->uiMenuPlayerText3_meshDataPtr->Clear();
    sharedDataPtr_->uiMenuPlayerText4_meshDataPtr->Clear();

    std::string playerString1;
    if (menuCursorPosition_ == 0) playerString1 += "> ";
    playerString1 += "P1: ";
    if (sharedDataPtr_->playerCreationInfos[0].turnedOn)
    {
        const char* controlSchemeString1 = ControlSchemeInfo::ControlSchemeIndexToString(sharedDataPtr_->playerCreationInfos[0].controlSchemeIndex);
        playerString1 += controlSchemeString1;
    }
    else
    {
        playerString1 += "OFF";
    }
    if (menuCursorPosition_ == 0) playerString1 += " <";

    std::string playerString2;
    if (menuCursorPosition_ == 1) playerString2 += "> ";
    playerString2 += "P2: ";
    if (sharedDataPtr_->playerCreationInfos[1].turnedOn)
    {
        const char* controlSchemeString2 = ControlSchemeInfo::ControlSchemeIndexToString(sharedDataPtr_->playerCreationInfos[1].controlSchemeIndex);
        playerString2 += controlSchemeString2;
    }
    else
    {
        playerString2 += "OFF";
    }
    if (menuCursorPosition_ == 1) playerString2 += " <";

    std::string playerString3;
    if (menuCursorPosition_ == 2) playerString3 += "> ";
    playerString3 += "P3: ";
    if (sharedDataPtr_->playerCreationInfos[2].turnedOn)
    {
        const char* controlSchemeString3 = ControlSchemeInfo::ControlSchemeIndexToString(sharedDataPtr_->playerCreationInfos[2].controlSchemeIndex);
        playerString3 += controlSchemeString3;
    }
    else
    {
        playerString3 += "OFF";
    }
    if (menuCursorPosition_ == 2) playerString3 += " <";

    std::string playerString4;
    if (menuCursorPosition_ == 3) playerString4 += "> ";
    playerString4 += "P4: ";
    if (sharedDataPtr_->playerCreationInfos[3].turnedOn)
    {
        const char* controlSchemeString4 = ControlSchemeInfo::ControlSchemeIndexToString(sharedDataPtr_->playerCreationInfos[3].controlSchemeIndex);
        playerString4 += controlSchemeString4;
    }
    else
    {
        playerString4 += "OFF";
    }
    if (menuCursorPosition_ == 3) playerString4 += " <";

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuPlayerText1_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        playerString1,
        playerPixelSize,
        1
    ));

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuPlayerText2_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        playerString2,
        playerPixelSize,
        1
    ));

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuPlayerText3_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        playerString3,
        playerPixelSize,
        1
    ));

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuPlayerText4_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        playerString4,
        playerPixelSize,
        1
    ));

    // -------------------------------------------------------------------------

    constexpr float startPixelSize = 3.0f;

    sharedDataPtr_->uiMenuStartText_meshDataPtr->Clear();

    std::string startString;
    if (menuCursorPosition_ == 4) startString += "> ";
    startString += "START";
    if (menuCursorPosition_ == 4) startString += " <";

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuStartText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        startString,
        startPixelSize,
        1
    ));

    // -------------------------------------------------------------------------

    constexpr float menuLeftPixelSize = 2.0f;

    sharedDataPtr_->uiMenuLeftText_meshDataPtr->Clear();

    std::string menuLeftString;

    if (menuCursorPosition_ < 4)
    {
        std::string groundApothemString = std::to_string(sharedDataPtr_->groundApothem);
        groundApothemString = groundApothemString.substr(0, groundApothemString.find(".") + 2);
        std::string groundApothemShrinkRateString = std::to_string(sharedDataPtr_->groundApothemShrinkRate_s);
        groundApothemShrinkRateString = groundApothemShrinkRateString.substr(0, groundApothemShrinkRateString.find(".") + 2);
        std::string sharkPathOffsetString = std::to_string(sharedDataPtr_->sharkPathOffset);
        sharkPathOffsetString = sharkPathOffsetString.substr(0, sharkPathOffsetString.find(".") + 2);
        std::string killzoneString = std::to_string(sharedDataPtr_->killzoneApothem);
        killzoneString = killzoneString.substr(0, killzoneString.find(".") + 2);

        menuLeftString += "GAME_CONSTANTS\n";
        menuLeftString += "------------------------------\n";
        menuLeftString += groundApothemString +           "      GROUND_APOTHEM" + "\n";
        menuLeftString += groundApothemShrinkRateString + "  GROUND_SHRINK_RATE" + "\n";
        menuLeftString += sharkPathOffsetString +         "   SHARK_PATH_OFFSET" + "\n";
        menuLeftString += killzoneString +                "    KILLZONE_APOTHEM" + "\n";
        menuLeftString += "\n";
        menuLeftString += "\n";
        menuLeftString += "\n";
        menuLeftString += "\n";
        menuLeftString += "\n";
        menuLeftString += "";
    }

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuLeftText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        menuLeftString,
        menuLeftPixelSize,
        2
    ));

    // -------------------------------------------------------------------------

    constexpr float menuRightPixelSize = 2.0f;

    sharedDataPtr_->uiMenuRightText_meshDataPtr->Clear();

    std::string menuRightString;

    if (menuCursorPosition_ < 4)
    {
        switch (sharedDataPtr_->playerCreationInfos[menuCursorPosition_].controlSchemeIndex)
        {
        case ControlSchemeInfo::s_controlSchemeIndex_unknown:
        {
            menuRightString += "OFF\n";
            menuRightString += "------------------------------\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_keyboard1:
        {
            menuRightString += "KEYBOARD_1\n";
            menuRightString += "------------------------------\n";
            menuRightString += "PAUSE     " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_pause_keyCode, false) + "\n";
            menuRightString += "LEFT      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_left_keyCode, false) + "\n";
            menuRightString += "RIGHT     " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_right_keyCode, false) + "\n";
            menuRightString += "UP        " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_up_keyCode, false) + "\n";
            menuRightString += "DOWN      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_down_keyCode, false) + "\n";
            menuRightString += "GRAB      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_grab_keyCode, false) + "\n";
            menuRightString += "DROP      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_1_drop_keyCode, false) + "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_keyboard2:
        {
            menuRightString += "KEYBOARD_2\n";
            menuRightString += "------------------------------\n";
            menuRightString += "PAUSE     " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_pause_keyCode, false) + "\n";
            menuRightString += "LEFT      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_left_keyCode, false) + "\n";
            menuRightString += "RIGHT     " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_right_keyCode, false) + "\n";
            menuRightString += "UP        " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_up_keyCode, false) + "\n";
            menuRightString += "DOWN      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_down_keyCode, false) + "\n";
            menuRightString += "GRAB      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_grab_keyCode, false) + "\n";
            menuRightString += "DROP      " + Project001::KeyCodeToString(sharedDataPtr_->keyboard_2_drop_keyCode, false) + "\n";
            menuRightString += "\n";
            menuRightString += "\n";
            menuRightString += "";
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller1:
        {
            std::string deadzoneString = std::to_string(sharedDataPtr_->controller_1_axisDeadzone);
            deadzoneString = deadzoneString.substr(0, deadzoneString.find(".") + 2);

            menuRightString += "CONTROLLER_1\n";
            menuRightString += "------------------------------\n";
            menuRightString += "PAUSE     " + std::to_string(sharedDataPtr_->controller_1_pause_buttonIndex) + "\n";
            menuRightString += "LEFT      " + std::to_string(sharedDataPtr_->controller_1_left_buttonIndex) + "\n";
            menuRightString += "RIGHT     " + std::to_string(sharedDataPtr_->controller_1_right_buttonIndex) + "\n";
            menuRightString += "UP        " + std::to_string(sharedDataPtr_->controller_1_up_buttonIndex) + "\n";
            menuRightString += "DOWN      " + std::to_string(sharedDataPtr_->controller_1_down_buttonIndex) + "\n";
            menuRightString += "GRAB      " + std::to_string(sharedDataPtr_->controller_1_grab_buttonIndex) + "\n";
            menuRightString += "DROP      " + std::to_string(sharedDataPtr_->controller_1_drop_buttonIndex) + "\n";
            menuRightString += "H-AXIS    " + std::to_string(sharedDataPtr_->controller_1_moveRightLeft_axisIndex) + "\n";
            menuRightString += "V-AXIS    " + std::to_string(sharedDataPtr_->controller_1_moveDownUp_axisIndex) + "\n";
            menuRightString += "DEADZONE  " + deadzoneString;
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller2:
        {
            std::string deadzoneString = std::to_string(sharedDataPtr_->controller_2_axisDeadzone);
            deadzoneString = deadzoneString.substr(0, deadzoneString.find(".") + 2);

            menuRightString += "CONTROLLER_2\n";
            menuRightString += "------------------------------\n";
            menuRightString += "PAUSE     " + std::to_string(sharedDataPtr_->controller_2_pause_buttonIndex) + "\n";
            menuRightString += "LEFT      " + std::to_string(sharedDataPtr_->controller_2_left_buttonIndex) + "\n";
            menuRightString += "RIGHT     " + std::to_string(sharedDataPtr_->controller_2_right_buttonIndex) + "\n";
            menuRightString += "UP        " + std::to_string(sharedDataPtr_->controller_2_up_buttonIndex) + "\n";
            menuRightString += "DOWN      " + std::to_string(sharedDataPtr_->controller_2_down_buttonIndex) + "\n";
            menuRightString += "GRAB      " + std::to_string(sharedDataPtr_->controller_2_grab_buttonIndex) + "\n";
            menuRightString += "DROP      " + std::to_string(sharedDataPtr_->controller_2_drop_buttonIndex) + "\n";
            menuRightString += "H-AXIS    " + std::to_string(sharedDataPtr_->controller_2_moveRightLeft_axisIndex) + "\n";
            menuRightString += "V-AXIS    " + std::to_string(sharedDataPtr_->controller_2_moveDownUp_axisIndex) + "\n";
            menuRightString += "DEADZONE  " + deadzoneString;
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller3:
        {
            std::string deadzoneString = std::to_string(sharedDataPtr_->controller_3_axisDeadzone);
            deadzoneString = deadzoneString.substr(0, deadzoneString.find(".") + 2);

            menuRightString += "CONTROLLER_3\n";
            menuRightString += "------------------------------\n";
            menuRightString += "PAUSE     " + std::to_string(sharedDataPtr_->controller_3_pause_buttonIndex) + "\n";
            menuRightString += "LEFT      " + std::to_string(sharedDataPtr_->controller_3_left_buttonIndex) + "\n";
            menuRightString += "RIGHT     " + std::to_string(sharedDataPtr_->controller_3_right_buttonIndex) + "\n";
            menuRightString += "UP        " + std::to_string(sharedDataPtr_->controller_3_up_buttonIndex) + "\n";
            menuRightString += "DOWN      " + std::to_string(sharedDataPtr_->controller_3_down_buttonIndex) + "\n";
            menuRightString += "GRAB      " + std::to_string(sharedDataPtr_->controller_3_grab_buttonIndex) + "\n";
            menuRightString += "DROP      " + std::to_string(sharedDataPtr_->controller_3_drop_buttonIndex) + "\n";
            menuRightString += "H-AXIS    " + std::to_string(sharedDataPtr_->controller_3_moveRightLeft_axisIndex) + "\n";
            menuRightString += "V-AXIS    " + std::to_string(sharedDataPtr_->controller_3_moveDownUp_axisIndex) + "\n";
            menuRightString += "DEADZONE  " + deadzoneString;
            break;
        }
        case ControlSchemeInfo::s_controlSchemeIndex_controller4:
        {
            std::string deadzoneString = std::to_string(sharedDataPtr_->controller_4_axisDeadzone);
            deadzoneString = deadzoneString.substr(0, deadzoneString.find(".") + 2);

            menuRightString += "CONTROLLER_4\n";
            menuRightString += "------------------------------\n";
            menuRightString += "PAUSE     " + std::to_string(sharedDataPtr_->controller_4_pause_buttonIndex) + "\n";
            menuRightString += "LEFT      " + std::to_string(sharedDataPtr_->controller_4_left_buttonIndex) + "\n";
            menuRightString += "RIGHT     " + std::to_string(sharedDataPtr_->controller_4_right_buttonIndex) + "\n";
            menuRightString += "UP        " + std::to_string(sharedDataPtr_->controller_4_up_buttonIndex) + "\n";
            menuRightString += "DOWN      " + std::to_string(sharedDataPtr_->controller_4_down_buttonIndex) + "\n";
            menuRightString += "GRAB      " + std::to_string(sharedDataPtr_->controller_4_grab_buttonIndex) + "\n";
            menuRightString += "DROP      " + std::to_string(sharedDataPtr_->controller_4_drop_buttonIndex) + "\n";
            menuRightString += "H-AXIS    " + std::to_string(sharedDataPtr_->controller_4_moveRightLeft_axisIndex) + "\n";
            menuRightString += "V-AXIS    " + std::to_string(sharedDataPtr_->controller_4_moveDownUp_axisIndex) + "\n";
            menuRightString += "DEADZONE  " + deadzoneString;
            break;
        }
        }
    }

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuRightText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        menuRightString,
        menuRightPixelSize,
        0
    ));

    // -------------------------------------------------------------------------

    constexpr float authorPixelSize = 2.0f;

    sharedDataPtr_->uiMenuAuthorText_meshDataPtr->Clear();

    std::string authorString = "created by VIK PANDHER";

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuAuthorText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        authorString,
        authorPixelSize,
        2
    ));

    // -------------------------------------------------------------------------

    constexpr float configPixelSize = 2.0f;

    sharedDataPtr_->uiMenuConfigFileFoundText_meshDataPtr->Clear();

    std::string configString = "Config. File ";
    
    if (sharedDataPtr_->configFileFound_)
    {
        configString += "Loaded";
    }
    else
    {
        configString += "Not Found";
    }

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuConfigFileFoundText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        configString,
        configPixelSize,
        0
    ));

    // -------------------------------------------------------------------------

    constexpr float versionPixelSize = 2.0f;

    sharedDataPtr_->uiMenuVersionText_meshDataPtr->Clear();

    std::string versionString = "versoin 0.1.0";

    FAIL_CHECK(Project001::Font::GenerateMeshDataFromFontDataAndString(
        *sharedDataPtr_->uiMenuVersionText_meshDataPtr,
        *sharedDataPtr_->pixelFont_fontDataPtr,
        versionString,
        versionPixelSize,
        0
    ));
}

void Scene002::UpdateMainCameraEntity(float timestep_s)
{
    constexpr float yawSpeed = 0.25f * glm::pi<float>();

    Project001::Camera* cameraPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCamera_entityId_));
    if (cameraPtr != nullptr)
    {
        float yawStep = yawSpeed * timestep_s;
        cameraPtr->RevolveAround(s_lookAtPoint_, yawStep, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 lookAtDirection = s_lookAtPoint_ - cameraPtr->GetPosition();
        lookAtDirection = glm::normalize(lookAtDirection);
        cameraPtr->LookAt(lookAtDirection, glm::vec3(0.0f, 0.0f, 1.0f));
    }
}