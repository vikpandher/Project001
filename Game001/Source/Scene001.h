// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-30

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

    SharedApplicationData* sharedDataPtr_;

    bool configFileFound_;

    // Entity Ids --------------------------------------------------------------

    static const uint32_t s_uiCamera_Mask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_EntityId_;

    unsigned int introText_EntityId_;
    unsigned int autorText_EntityId_;
};