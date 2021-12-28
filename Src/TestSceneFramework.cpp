#include "TestSceneFramework.h"

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

TestSceneFramework::TestSceneFramework()
    : componentStoresPtr_(nullptr)
    , meshStoresPtr_(nullptr)
    , rendererPtr_(nullptr)
    , textureStoresPtr_(nullptr)
    , windowPtr_(nullptr)
{
    ClearIndiciesAndEntityIds();
}

TestSceneFramework::~TestSceneFramework()
{}

void TestSceneFramework::Initialize()
{
    windowPtr_ = GetApplicationWindowPtr();

    componentStoresPtr_ = GetApplicationComponentStoresPtr();
    meshStoresPtr_ = GetApplicationMeshStoresPtr();
    soundStoresPtr_ = GetApplicationSoundStoresPtr();
    textureStoresPtr_ = GetApplicationTextureStoresPtr();

    rendererPtr_ = GetApplicationRendererPtr();
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
        cameraPtr->aspectRatio = aspectRatio;
        cameraPtr->position.z =5.0f;
        cameraPtr->AddYaw(glm::pi<float>());
        // cameraPtr->cameraProjection = Project001::Camera::CameraProjection::CAMERA_PROJECTION_ORTHOGRAPHIC;
        // cameraPtr->leftCutoff  = aspectRatio * -5.0f;
        // cameraPtr->rightCutoff = aspectRatio * 5.0f;
        cameraPtr->turnedOn = true;
    }

    // light source entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(lightSourceEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::LightSource>(lightSourceEntityId_));

        Project001::LightSource* lightSourcePtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));
        lightSourcePtr->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        lightSourcePtr->specular = glm::vec3(1.0f, 1.0f, 1.0f);
        lightSourcePtr->turnedOn = true;
    }
}

void TestSceneFramework::Deinitialize()
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

void TestSceneFramework::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestSceneFramework::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestSceneFramework::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestSceneFramework::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestSceneFramework::ProcessRenderEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestSceneFramework::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestSceneFramework::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestSceneFramework::ClearIndiciesAndEntityIds()
{
    sceneDataEntityId_ = (unsigned int)-1;
    mainCameraEntityId_ = (unsigned int)-1;
    lightSourceEntityId_ = (unsigned int)-1;
}

void TestSceneFramework::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
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

        float cameraYaw = -1.0f * xOffset * speedConstant;
        float cameraPitch = yOffset * speedConstant;

        cameraPtr->AddYaw(cameraYaw);
        // cameraPtr->AddWorldRotationY(cameraYaw); // for fps camera
        cameraPtr->AddPitch(cameraPitch);

        prevousXPosition = currentXPosition;
        prevousYPosition = currentYPosition;
    }
}

void TestSceneFramework::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
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
}

void TestSceneFramework::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
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
}

void TestSceneFramework::ProcessRenderEvent(Project001::RenderEvent& renderEvent)
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->ClearBuffers();

    Project001::LightSource* lightSourceArray = nullptr;
    size_t lightSourceCount = 0;

    _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::LightSource>(lightSourceArray, lightSourceCount));

    for (unsigned int i = 0; i < lightSourceCount; ++i)
    {
        Project001::LightSource& currentLightSource = lightSourceArray[i];
        if (currentLightSource.turnedOn)
        {
            if (currentLightSource.lightType == Project001::LightSource::LightType::LIGHT_TYPE_DIRECTIONAL)
            {
                rendererPtr_->SetDirectionalLight(
                    currentLightSource.direction,
                    currentLightSource.ambient,
                    currentLightSource.diffuse,
                    currentLightSource.specular
                );
            }
            else if (currentLightSource.lightType == Project001::LightSource::LightType::LIGHT_TYPE_POINT)
            {
                rendererPtr_->AddPointLight(
                    currentLightSource.position,
                    currentLightSource.constant,
                    currentLightSource.linear,
                    currentLightSource.quadratic,
                    currentLightSource.ambient,
                    currentLightSource.diffuse,
                    currentLightSource.specular
                );
            }
            else if (currentLightSource.lightType == Project001::LightSource::LightType::LIGHT_TYPE_SPOT)
            {
                rendererPtr_->AddSpotLight(
                    currentLightSource.position,
                    currentLightSource.direction,
                    currentLightSource.cutoff,
                    currentLightSource.outerCutoff,
                    currentLightSource.constant,
                    currentLightSource.linear,
                    currentLightSource.quadratic,
                    currentLightSource.ambient,
                    currentLightSource.diffuse,
                    currentLightSource.specular
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

        if (currentRenderedModel.visible)
        {
            const Project001::MeshVertex* meshVerticies = nullptr;
            unsigned int meshVertexCount = 0;
            const unsigned int* meshIndicies = nullptr;
            unsigned int meshIndexCount = 0;

            _FAIL_CHECK(meshStoresPtr_->GetMesh(
                currentRenderedModel.meshIndex,
                meshVerticies,
                meshVertexCount,
                meshIndicies,
                meshIndexCount
            ));

            _FAIL_CHECK(rendererPtr_->AddMesh(
                meshVerticies,
                meshVertexCount,
                meshIndicies,
                meshIndexCount,
                currentRenderedModel.textureIndex,
                currentRenderedModel.specularIndex,
                currentRenderedModel.position,
                currentRenderedModel.orientation,
                currentRenderedModel.scale,
                currentRenderedModel.color,
                currentRenderedModel.shininess,
                currentRenderedModel.translucent,
                currentRenderedModel.lit
            ));
        }
    }

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr->turnedOn)
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->position);
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());
        rendererPtr_->Render();
    }

    rendererPtr_->SwapBuffers();

    renderEvent.handled = true;
}

void TestSceneFramework::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.1f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    cameraPtr->MoveForward(cameraTranslation);
}

void TestSceneFramework::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
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

void TestSceneFramework::UpdateMainCameraEntityPositionAndRoll(unsigned long timestep_ns)
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

void TestSceneFramework::SyncComponentPositions()
{
    Project001::LightSource* lightSourcePtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    lightSourcePtr->position = cameraPtr->position;

    soundPlayerPtr_->UpdateListener(
        cameraPtr->position,
        cameraPtr->GetForwardVector(),
        cameraPtr->GetUpVector(),
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f
    );
}

void TestSceneFramework::DeleteDeadEntities()
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
