// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#pragma once

#include "BaseScene001.h"



class Scene001 : public BaseScene001
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
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    void LoadPixelFontResources();
    void LoadGeneralResources();
    void LoadMainMenuResources();
    void LoadCursorResources();
    void LoadImpactResources();
    void LoadStageResources();
    void LoadStageGridResources();
    void LoadActorResources();
    void LoadUiResources();

    void FreeResources();

    void ReadConfigFile();

    void CreateKillZoneMesh();
    void CreateLoadingTextEntity();

    // Entity Ids --------------------------------------------------------------

    static constexpr uint32_t s_uiCamera_cameraMask_ = 0b10000000000000000000000000000000;
    unsigned int uiCamera_entityId_ = static_cast<unsigned int>(-1);

    unsigned int loadingText_entityId_ = static_cast<unsigned int>(-1);
};