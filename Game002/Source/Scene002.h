// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-27

#pragma once

#include "Scene.h"

#include "SharedApplicationData.h"



class Scene002 : public Project001::Scene
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
    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);
    void ProcessUpdateEvent(Project001::UpdateEvent& updateEvent);

    // -------------------------------------------------------------------------

    SharedApplicationData* sharedDataPtr_ = nullptr;
};