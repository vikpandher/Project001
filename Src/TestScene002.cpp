#include "TestScene002.h"

#include "DeathFlag.h"
#include "TestComponents.h"
#include "TestSceneData.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/LRUArray.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene002::TestScene002()
    : componentStoresPtr_(nullptr)
    , meshStoresPtr_(nullptr)
    , rendererPtr_(nullptr)
    , textureStoresPtr_(nullptr)
    , windowPtr_(nullptr)
{
    ClearIndiciesAndEntityIds();
}

TestScene002::~TestScene002()
{}

const char* TestScene002::Name()
{
    return "TestScene002";
}

void TestScene002::Initialize()
{
    windowPtr_ = GetApplicationWindowPtr();

    componentStoresPtr_ = GetApplicationComponentStoresPtr();
    meshStoresPtr_ = GetApplicationMeshStoresPtr();
    textureStoresPtr_ = GetApplicationTextureStoresPtr();

    rendererPtr_ = GetApplicationRendererPtr();

    int windowWidth, windowHeight;
    windowPtr_->GetWindowSize(windowWidth, windowHeight);
    windowPtr_->SetAspectRatio(windowWidth, windowHeight);

    // meshes
    // -------------------------------------------------------------------------
    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape01MeshIndex_, 0.64f, 0.64f, 0.64f));

    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape02MeshIndex_, 0.32f, 3, 2));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape03MeshIndex_, 0.32f, 4, 4));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape04MeshIndex_, 0.32f, 8, 8));

    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape05MeshIndex_, 0.32f, 3, 2, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape06MeshIndex_, 0.32f, 4, 4, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape07MeshIndex_, 0.32f, 8, 8, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape08MeshIndex_, 0.32f, 0));

    // textures
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(earthTextureIndex_, "../Textures/earth.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(earthTextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(earthTextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

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
        cameraPtr->TurnOn();
    }

    // light source entity
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(lightSourceEntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::LightSource>(lightSourceEntityId_));

        Project001::LightSource* lightSourcePtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));
        lightSourcePtr->SetAmbientColor(0.1f, 0.1f, 0.1f);
        lightSourcePtr->SetSpecularColor(1.0f, 1.0f, 1.0f);
        lightSourcePtr->TurnOn();
    }

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -3; j <= 3; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape01EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape01MeshIndex_);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape02EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape02EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape02EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape02MeshIndex_);
        renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape03EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape03EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape03EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape03MeshIndex_);
        renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape04EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape04EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape04EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape04MeshIndex_);
        renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape05EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape05EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape05EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape05MeshIndex_);
        renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape06EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape06EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape06EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape06MeshIndex_);
        renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape07EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape07EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape07EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape07MeshIndex_);
        renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    // {
    //     _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape08EntityId_));
    //     _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_));
    // 
    //     Project001::RenderedModel* renderedModelPtr;
    //     _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr));
    //     renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
    //     renderedModelPtr->SetMeshIndex(shape08MeshIndex_);
    //     renderedModelPtr->SetTextureIndex(earthTextureIndex_);
    // }
}

void TestScene002::Deinitialize()
{
    componentStoresPtr_->DeleteAllEntities();
    meshStoresPtr_->ClearMeshes();
    textureStoresPtr_->ClearTextures();
    rendererPtr_->ClearTextures();

    ClearIndiciesAndEntityIds();
}

void TestScene002::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene002::ProcessCursorPositionEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::FrameBufferSizeEvent>(event, std::bind(&TestScene002::ProcessFrameBufferSizeEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene002::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene002::ProcessMouseButtonEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene002::ProcessScrollEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene002::ProcessUpdateEvent, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestScene002::ClearIndiciesAndEntityIds()
{
    shape01MeshIndex_ = (unsigned int)-1;
    shape02MeshIndex_ = (unsigned int)-1;
    shape03MeshIndex_ = (unsigned int)-1;
    shape04MeshIndex_ = (unsigned int)-1;
    shape05MeshIndex_ = (unsigned int)-1;
    shape06MeshIndex_ = (unsigned int)-1;
    shape07MeshIndex_ = (unsigned int)-1;
    shape08MeshIndex_ = (unsigned int)-1;

    earthTextureIndex_ = (unsigned int)-1;

    sceneDataEntityId_ = (unsigned int)-1;
    mainCameraEntityId_ = (unsigned int)-1;
    lightSourceEntityId_ = (unsigned int)-1;
    shape01EntityId_ = (unsigned int)-1;
    shape02EntityId_ = (unsigned int)-1;
    shape03EntityId_ = (unsigned int)-1;
    shape04EntityId_ = (unsigned int)-1;
    shape05EntityId_ = (unsigned int)-1;
    shape06EntityId_ = (unsigned int)-1;
    shape07EntityId_ = (unsigned int)-1;
    shape08EntityId_ = (unsigned int)-1;
}

void TestScene002::ProcessCursorPositionEvent(Project001::CursorPositionEvent& cursorButtonEvent)
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
        float cameraPitch = -1.0f * yOffset * speedConstant;

        cameraPtr->AddYaw(cameraYaw);
        cameraPtr->AddPitch(cameraPitch);

        prevousXPosition = currentXPosition;
        prevousYPosition = currentYPosition;
    }
}

void TestScene002::ProcessFrameBufferSizeEvent(Project001::FrameBufferSizeEvent& frameBufferSizeEvent)
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

void TestScene002::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene001"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene001"));
            }
        }
    }
}

void TestScene002::ProcessMouseButtonEvent(Project001::MouseButtonEvent& mouseButtonEvent)
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

void TestScene002::ProcessScrollEvent(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.25f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    cameraPtr->MoveForward(cameraTranslation);
}

void TestScene002::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    double timestep = updateEvent.timestep_s;

    // Update Entities
    UpdateMainCameraEntityPositionAndRoll(timestep);

    UpdateLightEntityPosition();

    // Render all renderable components
    RenderRenderableEntities();

    updateEvent.handled = true;
}

void TestScene002::UpdateMainCameraEntityPositionAndRoll(double timestep)
{
    float speedConstant = 2.5f;
    float cameraTranslationSpeed = speedConstant * (float)timestep;
    float cameraRotationSpeed = speedConstant * (float)timestep;

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

    // cameraPtr->LookAt(-1.0f * cameraPtr->GetPosition()); // add to orbit
}

void TestScene002::UpdateLightEntityPosition()
{
    Project001::LightSource* lightSourcePtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr));

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    lightSourcePtr->SetPosition(cameraPtr->GetPosition());
}

void TestScene002::RenderRenderableEntities()
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

    _FAIL_CHECK(componentStoresPtr_->GetAllComponents<Project001::RenderedModel>(renderedModelArray, renderedModelCount));

    for (unsigned int i = 0; i < renderedModelCount; ++i)
    {
        Project001::RenderedModel& currentRenderedModel = renderedModelArray[i];

        if (currentRenderedModel.IsVisible())
        {
            _FAIL_CHECK(rendererPtr_->AddMesh(
                meshStoresPtr_,
                currentRenderedModel.GetMeshIndex(),
                currentRenderedModel.GetTextureIndex(),
                currentRenderedModel.GetSpecularIndex(),
                currentRenderedModel.GetShininess(),
                currentRenderedModel.GetColor(),
                currentRenderedModel.GetTranslucent(),
                currentRenderedModel.GetScale(),
                currentRenderedModel.GetPosition(),
                currentRenderedModel.GetOrientation(),
                currentRenderedModel.GetLit()
            ));
        }
    }

    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));

    if (cameraPtr->IsTurnedOn())
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->GetPosition());
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());
        rendererPtr_->Render();
    }

    rendererPtr_->SwapBuffers();
}