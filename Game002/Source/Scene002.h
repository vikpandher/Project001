// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-05

#pragma once

#include "BaseScene001.h"

#include "SharedApplicationData.h"

#include "glm/glm.hpp"



class Scene002 : public BaseScene001
{
public:
    Scene002(Project001::Application* applicationPtr);
    ~Scene002();

    Scene002(Scene002& other) = delete;
    void operator=(const Scene002&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void CreateBackgroundEntity();
    void CreateLightEntity();
    void CreateMenuEntity();
    void CreatePenguinEntity(unsigned int& entityId, size_t playerNumber);
    void CreateSharkEntity(unsigned int& entityId);

    void UpdateMenuTextEntity(float timestep_s, bool& quit); // modifies meshes
    void UpdateMenuTextMeshes(); // modifies meshes
    void UpdateMainCameraEntity(float timestep_s);

    // -------------------------------------------------------------------------

    SharedApplicationData* sharedDataPtr_ = nullptr;

    // Entity Ids --------------------------------------------------------------

    static constexpr uint32_t s_backUiCamera_cameraMask_ = 0b01000000000000000000000000000000;
    unsigned int backUiCamera_entityId_ = static_cast<unsigned int>(-1);

    static constexpr glm::vec3 s_lookAtPoint_ = glm::vec3(0.0f, 0.0f, 2.0f);
    static constexpr float s_distanceFromFocalPoint_ = 256.0f;
    static constexpr uint32_t s_mainCamera_cameraMask_ = 0b00000000000000000000000000000001;
    unsigned int mainCamera_entityId_ = static_cast<unsigned int>(-1);

    static constexpr uint32_t s_uiCamera_cameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_entityId_ = static_cast<unsigned int>(-1);

    unsigned int background_entityId_ = static_cast<unsigned int>(-1);

    unsigned int light_entityId_ = static_cast<unsigned int>(-1);

    unsigned int menuText_entityId_ = static_cast<unsigned int>(-1);

    unsigned int player_entityIds_[SharedApplicationData::s_player_count] = {
        static_cast<unsigned int>(-1),
        static_cast<unsigned int>(-1),
        static_cast<unsigned int>(-1),
        static_cast<unsigned int>(-1)
    };

    unsigned int shark_entityId_ = static_cast<unsigned int>(-1);

    // -------------------------------------------------------------------------

    size_t menuCursorPosition_ = 4;
    static constexpr float s_menuAxisMoveDelay_s_ = 0.25f;
    float menuAxisMoveTime_s = 0.0f;
};