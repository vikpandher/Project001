// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#pragma once

#include "Scene.h"

#include "SharedApplicationData.h"



class Scene001 : public Project001::Scene
{
public:
    Scene001(Project001::Application* applicationPtr);
    ~Scene001();

    Scene001(Scene001& other) = delete;
    void operator=(const Scene001&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);
    void ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent);
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void LoadPixelFontResources();
    void LoadIntroTextResources();
    void LoadCursorResources();
    void LoadGroundGridResources();
    void LoadGroundResources();
    void LoadHouseLightResources();
    void LoadHouseResources();
    void LoadLampLightResources();
    void LoadLampResources();
    void LoadPersonResources();
    void LoadPlayerLightResources();
    void LoadUiTextResources();
    void LoadSoundResources();

    void FreeResources();

    void ReadConfigFile();

    void CreateUiCameraEntity();

    void CreateIntroTextEntity();

    // -------------------------------------------------------------------------

    SharedApplicationData* sharedDataPtr_ = nullptr;

    bool configFileFound_ = false;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_uiCamera_Mask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_EntityId_ = static_cast<unsigned int>(-1);

    unsigned int introText_EntityId_ = static_cast<unsigned int>(-1);

    unsigned int autorText_EntityId_ = static_cast<unsigned int>(-1);
};