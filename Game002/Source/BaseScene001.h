// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-05

#pragma once

#include "Scene.h"



class BaseScene001 : public Project001::Scene
{
public:
    BaseScene001(Project001::Application* applicationPtr);
    ~BaseScene001();

    BaseScene001(BaseScene001& other) = delete;
    void operator=(const BaseScene001&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    void ProcessRenderEvent(Project001::RenderEvent& renderEvent);

    void CreateMainCameraEntity(unsigned int& entityId, const uint32_t cameraMask, int priorityValue);
    void CreateUiCameraEntity(unsigned int& entityId, const uint32_t cameraMask, int priorityValue);

    // -------------------------------------------------------------------------

    bool skipRenderingOnce_ = true;

};