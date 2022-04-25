#include "TestSceneBase001.h"

#include "DeathFlag.h"
#include "TestSceneData.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/SoundStores.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestSceneBase001::TestSceneBase001()
    : componentStoresPtr_(nullptr)
    , meshStoresPtr_(nullptr)
    , rendererPtr_(nullptr)
    , textureStoresPtr_(nullptr)
    , windowPtr_(nullptr)
{
    ClearIndiciesAndEntityIds();
}

TestSceneBase001::~TestSceneBase001()
{}

void TestSceneBase001::Initialize()
{
    windowPtr_ = GetApplicationWindowPtr();

    componentStoresPtr_ = GetApplicationComponentStoresPtr();
    meshStoresPtr_ = GetApplicationMeshStoresPtr();
    soundStoresPtr_ = GetApplicationSoundStoresPtr();
    textureStoresPtr_ = GetApplicationTextureStoresPtr();

    rendererPtr_ = GetApplicationRendererPtr();
    rendererPtr_->SetDepthTesting(true);

    soundPlayerPtr_ = GetApplicationSoundPlayerPtr();

    int windowWidth, windowHeight;
    windowPtr_->GetWindowSize(windowWidth, windowHeight);
    windowPtr_->SetAspectRatio(windowWidth, windowHeight);

    // scene data entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(sceneDataEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<TestSceneData>(sceneDataEntityId_));
    }

    // main camera entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(mainCameraEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::Camera>(mainCameraEntityId_));

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
        int framebufferWidth;
        int framebufferHeight;
        windowPtr_->GetFramebufferSize(framebufferWidth, framebufferHeight);
        float aspectRatio = (float)framebufferWidth / (float)framebufferHeight;
        cameraPtr->SetAspectRatio(aspectRatio);
        cameraPtr->SetPosition(0.0f, 0.0f, 5.0f);
        cameraPtr->AddYaw(glm::pi<float>());
        // cameraPtr->SetProjectionToOrthographic();
        // cameraPtr->SetLeftCutoff(aspectRatio * -5.0f);
        // cameraPtr->SetRightCutoff(aspectRatio * 5.0f);
        cameraPtr->TurnOn();
    }

    // light source entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(lightSourceEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::LightSource>(lightSourceEntityId_));

        Project001::LightSource* lightSourcePtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));
        lightSourcePtr->SetPosition(0.0f, 0.0f, 5.0f);
        lightSourcePtr->SetDirection(0.0f, 0.0f, -1.0f);
        lightSourcePtr->SetAmbientColor(0.1f, 0.1f, 0.1f);
        lightSourcePtr->SetSpecularColor(1.0f, 1.0f, 1.0f);
        lightSourcePtr->TurnOn();
    }
}

void TestSceneBase001::Deinitialize()
{
    componentStoresPtr_->DeleteAllEntities();
    meshStoresPtr_->ClearMeshes();
    soundStoresPtr_->ClearSounds();
    textureStoresPtr_->ClearTextures();
    rendererPtr_->ClearTextures();
    soundPlayerPtr_->RemoveAllSoundSources();
    soundPlayerPtr_->RemoveAllSoundBuffers();

    ClearIndiciesAndEntityIds();
}

void TestSceneBase001::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneBase001::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestSceneBase001::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneBase001::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneBase001::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneBase001::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneBase001::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestSceneBase001::ClearIndiciesAndEntityIds()
{
    sceneDataEntityId_ = (unsigned int)-1;
    mainCameraEntityId_ = (unsigned int)-1;
    lightSourceEntityId_ = (unsigned int)-1;
}

void TestSceneBase001::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
{
    bool mouseButton1Pressed = windowPtr_->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_1);

    if (mouseButton1Pressed)
    {
        float& currentXPosition = cursorButtonEvent.xPosition;
        float& currentYPosition = cursorButtonEvent.yPosition;

        TestSceneData* TestSceneDataPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<TestSceneData>(sceneDataEntityId_, TestSceneDataPtr));

        float& prevousXPosition = TestSceneDataPtr->previousCursorDownPosition.x;
        float& prevousYPosition = TestSceneDataPtr->previousCursorDownPosition.y;

        Project001::Camera* cameraPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

        float speedConstant = 0.005f;

        float xOffset = currentXPosition - prevousXPosition;
        float yOffset = currentYPosition - prevousYPosition;

        // moving cursor right = positive xOffset
        // moving cursor up = negative yOffset

        float cameraYaw = -1.0f * xOffset * speedConstant;
        float cameraPitch = yOffset * speedConstant;

        cameraPtr->AddYaw(cameraYaw);
        // cameraPtr->AddWorldRotationY(cameraYaw); // for fps camera
        cameraPtr->AddPitch(cameraPitch);

        prevousXPosition = currentXPosition;
        prevousYPosition = currentYPosition;
    }

    cursorButtonEvent.handled = true;
}

void TestSceneBase001::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
{
    const int& height = frameBufferSizeEvent.height;
    const int& width = frameBufferSizeEvent.width;

    int aspectRatioNumerator, aspectRatioDenominator;
    windowPtr_->GetAspectRatio(aspectRatioNumerator, aspectRatioDenominator);

    if (aspectRatioNumerator > 0 && aspectRatioDenominator > 0)
    {
        float aspectRatio = (float)aspectRatioNumerator / (float)aspectRatioDenominator;

        int adjustedHeight = (int)(width / aspectRatio);
        int adjustedWidth = (int)(height * aspectRatio);

        if (adjustedWidth > width)
        {
            adjustedWidth = width;
        }

        if (adjustedHeight > height)
        {
            adjustedHeight = height;
        }

        int lowerLeftX = (width - adjustedWidth) / 2;
        int lowerLeftY = (height - adjustedHeight) / 2;

        rendererPtr_->SetFramebufferSize(adjustedWidth, adjustedHeight);
        rendererPtr_->SetViewportSize(lowerLeftX, lowerLeftY, adjustedWidth, adjustedHeight);
    }
    else
    {
        rendererPtr_->SetFramebufferSize(width, height);
        rendererPtr_->SetViewportSize(0, 0, width, height);
    }

    frameBufferSizeEvent.handled = true;
}

void TestSceneBase001::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        TestSceneData* TestSceneDataPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<TestSceneData>(sceneDataEntityId_, TestSceneDataPtr));

        float& prevousXPosition = TestSceneDataPtr->previousCursorDownPosition.x;
        float& prevousYPosition = TestSceneDataPtr->previousCursorDownPosition.y;

        windowPtr_->GetCursorPosition(prevousXPosition, prevousYPosition);
    }

    mouseButtonEvent.handled = true;
}

void TestSceneBase001::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->Clear();

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr->IsTurnedOn())
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->GetPosition());
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());
        rendererPtr_->PrepareCapabilities();

        Project001::LightSource* lightSourceArray = nullptr;
        size_t lightSourceCount = 0;

        _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::LightSource>(lightSourceArray, lightSourceCount));

        for (unsigned int i = 0; i < lightSourceCount; ++i)
        {
            Project001::LightSource& currentLightSource = lightSourceArray[i];
            if (currentLightSource.IsTurnedOn())
            {
                if (currentLightSource.IsLightTypeDirectional())
                {
                    rendererPtr_->SetDirectionalLight(
                        currentLightSource.GetDirection(),
                        currentLightSource.GetAmbientColor(),
                        currentLightSource.GetDiffuseColor(),
                        currentLightSource.GetSpecularColor()
                    );
                }
                else if (currentLightSource.IsLightTypePoint())
                {
                    rendererPtr_->AddPointLight(
                        currentLightSource.GetPosition(),
                        currentLightSource.GetAttenuationConstant(),
                        currentLightSource.GetLinearAttenuation(),
                        currentLightSource.GetQuadraticAttenuation(),
                        currentLightSource.GetAmbientColor(),
                        currentLightSource.GetDiffuseColor(),
                        currentLightSource.GetSpecularColor()
                    );
                }
                else if (currentLightSource.IsLightTypeSpot())
                {
                    rendererPtr_->AddSpotLight(
                        currentLightSource.GetPosition(),
                        currentLightSource.GetDirection(),
                        currentLightSource.GetCutoff(),
                        currentLightSource.GetOuterCutoff(),
                        currentLightSource.GetAttenuationConstant(),
                        currentLightSource.GetLinearAttenuation(),
                        currentLightSource.GetQuadraticAttenuation(),
                        currentLightSource.GetAmbientColor(),
                        currentLightSource.GetDiffuseColor(),
                        currentLightSource.GetSpecularColor()
                    );
                }
            }
        }

        Project001::RenderedModel* renderedModelArray = nullptr;
        size_t renderedModelCount = 0;

        componentStoresPtr_->GetAllComponents<Project001::RenderedModel>(renderedModelArray, renderedModelCount);

        for (unsigned int i = 0; i < renderedModelCount; ++i)
        {
            Project001::RenderedModel& currentRenderedModel = renderedModelArray[i];

            if (currentRenderedModel.IsVisible())
            {
                const Project001::MeshVertex* meshVerticies = nullptr;
                unsigned int meshVertexCount = 0;
                const unsigned int* meshIndicies = nullptr;
                unsigned int meshIndexCount = 0;

                _FAIL_CHECK(meshStoresPtr_->GetMesh(
                    currentRenderedModel.GetMeshIndex(),
                    meshVerticies,
                    meshVertexCount,
                    meshIndicies,
                    meshIndexCount));

                if (!rendererPtr_->AddMesh(
                    meshVerticies,
                    meshVertexCount,
                    meshIndicies,
                    meshIndexCount,
                    currentRenderedModel.GetTextureIndex(),
                    currentRenderedModel.GetSpecularIndex(),
                    currentRenderedModel.GetPosition(),
                    currentRenderedModel.GetOrientation(),
                    currentRenderedModel.GetScale(),
                    currentRenderedModel.GetColor(),
                    currentRenderedModel.GetShininess(),
                    currentRenderedModel.GetTranslucent(),
                    currentRenderedModel.GetLit()))
                {
                    rendererPtr_->Render();
                    rendererPtr_->AddMesh(
                        meshVerticies,
                        meshVertexCount,
                        meshIndicies,
                        meshIndexCount,
                        currentRenderedModel.GetTextureIndex(),
                        currentRenderedModel.GetSpecularIndex(),
                        currentRenderedModel.GetPosition(),
                        currentRenderedModel.GetOrientation(),
                        currentRenderedModel.GetScale(),
                        currentRenderedModel.GetColor(),
                        currentRenderedModel.GetShininess(),
                        currentRenderedModel.GetTranslucent(),
                        currentRenderedModel.GetLit());
                }
            }
        }

        rendererPtr_->Render();
    }

    windowPtr_->SwapBuffers();

    renderEvent.handled = true;
}

void TestSceneBase001::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    cameraPtr->MoveForward(cameraTranslation);

    scrollEvent.handled = true;
}

void TestSceneBase001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long timestep_ns = updateEvent.timestep_ns;

    // Update Entities
    UpdateMainCameraEntityPositionAndRoll(timestep_ns);

    SyncComponentPositions();

    // Delete all entities with marked for deletion component
    // DeleteDeadEntities();

    // Play all audio components???

    updateEvent.handled = true;
}

void TestSceneBase001::UpdateMainCameraEntityPositionAndRoll(unsigned long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    float speedConstant = 1.0f;
    float cameraTranslationSpeed = speedConstant * timestep_s;
    float cameraRotationSpeed = speedConstant * 2.0f * timestep_s;

    bool movingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_A);
    bool movingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_D);
    bool movingUp = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_W);
    bool movingDown = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_S);
    bool rollingLeft = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_Q);
    bool rollingRight = windowPtr_->GetKeyPressed(Project001::KeyCode::KEY_CODE_E);

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (movingLeft)
    {
        cameraPtr->MoveLeft(cameraTranslationSpeed);
        // cameraPtr->RevolveAroundHorizontally(glm::vec3(0.0f, 0.0f, 0.0f), -1.0f * cameraRotationSpeed); // add to orbit
    }

    if (movingRight)
    {
        cameraPtr->MoveRight(cameraTranslationSpeed);
        // cameraPtr->RevolveAroundHorizontally(glm::vec3(0.0f, 0.0f, 0.0f), cameraRotationSpeed); // add to orbit
    }

    if (movingUp)
    {
        cameraPtr->MoveUp(cameraTranslationSpeed);
    }

    if (movingDown)
    {
        cameraPtr->MoveDown(cameraTranslationSpeed);
    }

    if (rollingLeft)
    {
        cameraPtr->AddRoll(-1.0f * cameraRotationSpeed);
    }

    if (rollingRight)
    {
        cameraPtr->AddRoll(cameraRotationSpeed);
    }

    // cameraPtr->LookAt(-1.0f * cameraPtr->GetPosition()); // add for hacky orbit
}

void TestSceneBase001::SyncComponentPositions()
{
    Project001::LightSource* lightSourcePtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    lightSourcePtr->SetPosition(cameraPtr->GetPosition());

    soundPlayerPtr_->UpdateListener(
        cameraPtr->GetPosition(),
        cameraPtr->GetForwardVector(),
        cameraPtr->GetUpVector(),
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f
    );
}

void TestSceneBase001::DeleteDeadEntities()
{
    DeathFlag* deathFlagArray = nullptr;
    size_t deathFlagCount = 0;

    _FAIL_CHECK(componentStoresPtr_->GetAllComponents<DeathFlag>(deathFlagArray, deathFlagCount));

    for (unsigned int i = 0; i < deathFlagCount; ++i)
    {
        DeathFlag& currentDeathFlag = deathFlagArray[i];
        if (currentDeathFlag.dead)
        {
            unsigned int entityId = (unsigned int)-1;
            _FAIL_CHECK(componentStoresPtr_->GetComponentEntityId<DeathFlag>(&currentDeathFlag, entityId));
            componentStoresPtr_->DeleteEntity(entityId);
        }
    }
}
