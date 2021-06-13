#include "TestScene.h"

#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/LRUArray.h"
#include "Engine/ModelStores.h"
#include "Engine/Renderer.h"
#include "Engine/SceneComponents.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene::TestScene()
    : componentStoresPtr_(nullptr)
    , modelStoresPtr_(nullptr)
    , rendererPtr_(nullptr)
    , textureStoresPtr_(nullptr)
    , windowPtr_(nullptr)
    , cubeModelIndex_((unsigned int)-1)
    , diceTexture01Index_((unsigned int)-1)
    , diceTexture02Index_((unsigned int)-1)
    , thonkTextureIndex_((unsigned int)-1)
    , patternSpecularIndex_((unsigned int)-1)
    , thonkSpecularIndex_((unsigned int)-1)
    , shape01Index_((unsigned int)-1)
    , shape02Index_((unsigned int)-1)
    , shape03Index_((unsigned int)-1)
    , shape04Index_((unsigned int)-1)
    , shape05Index_((unsigned int)-1)
    , sceneDataEntityId_((unsigned int)-1)
    , mainCameraEntityId_((unsigned int)-1)
    , lightSourceEntityId_((unsigned int)-1)
    , cubeEntity01Id_((unsigned int)-1)
    , cubeEntity02Id_((unsigned int)-1)
    , cubeEntity03Id_((unsigned int)-1)
    , cubeEntity04Id_((unsigned int)-1)
    , shape01EntityId_((unsigned int)-1)
    , shape02EntityId_((unsigned int)-1)
    , shape03EntityId_((unsigned int)-1)
    , shape04EntityId_((unsigned int)-1)
    , shape05EntityId_((unsigned int)-1)
{}

TestScene::~TestScene()
{}

const char* TestScene::Name()
{
    return "Test";
}

void TestScene::Initialize(
    Project001::ComponentStores* componentStoresPtr,
    Project001::ModelStores* modelStoresPtr,
    Project001::TextureStores* textureStoresPtr,
    Project001::Renderer* rendererPtr,
    Project001::Window* windowPtr)
{
    componentStoresPtr_ = componentStoresPtr;
    rendererPtr_ = rendererPtr;
    modelStoresPtr_ = modelStoresPtr;
    textureStoresPtr_ = textureStoresPtr;
    windowPtr_ = windowPtr;

    modelStoresPtr_->LoadModel("../Models/Cube.obj", cubeModelIndex_);

    std::vector<glm::vec2> fanVerticies;
    fanVerticies.emplace_back(0.32f, 0.0f);
    fanVerticies.emplace_back(0.24f, 0.24f);
    fanVerticies.emplace_back(0.0f, 0.32f);
    fanVerticies.emplace_back(-0.24f, 0.24f);
    fanVerticies.emplace_back(-0.32f, 0.0f);
    fanVerticies.emplace_back(-0.24f, -0.24f);
    fanVerticies.emplace_back(0.0f, -0.32f);
    fanVerticies.emplace_back(0.24f, -0.24f);
    modelStoresPtr_->Generate2DTriangleFan(fanVerticies, shape01Index_);

    std::vector<glm::vec2> stripVerticies;
    stripVerticies.emplace_back(-0.32f, -0.32f);
    stripVerticies.emplace_back(-0.24f, -0.24f);
    stripVerticies.emplace_back(-0.32f, 0.32f);
    stripVerticies.emplace_back(-0.24f, 0.24f);
    stripVerticies.emplace_back(0.32f, 0.32f);
    stripVerticies.emplace_back(0.24f, 0.24f);
    stripVerticies.emplace_back(0.32f, -0.32f);
    stripVerticies.emplace_back(0.24f, -0.24f);
    stripVerticies.emplace_back(-0.16f, -0.32f);
    stripVerticies.emplace_back(-0.08f, -0.24f);
    stripVerticies.emplace_back(-0.16f, 0.16f);
    stripVerticies.emplace_back(-0.08f, 0.08f);
    stripVerticies.emplace_back(0.16f, 0.16f);
    stripVerticies.emplace_back(0.08f, 0.08f);
    stripVerticies.emplace_back(0.16f, -0.16f);
    stripVerticies.emplace_back(0.08f, -0.08f);
    modelStoresPtr_->Generate2DTriangleStrip(stripVerticies, shape02Index_);

    std::vector<glm::vec2> triVerticies;
    triVerticies.emplace_back(0.0f, 0.08f);
    triVerticies.emplace_back(0.24f, 0.32f);
    triVerticies.emplace_back(-0.24f, 0.32f);
    triVerticies.emplace_back(0.08f, 0.0f);
    triVerticies.emplace_back(0.32f, -0.24f);
    triVerticies.emplace_back(0.32f, 0.24f);
    triVerticies.emplace_back(0.0f, -0.08f);
    triVerticies.emplace_back(-0.24f, -0.32f);
    triVerticies.emplace_back(0.24f, -0.32f);
    triVerticies.emplace_back(-0.08f, 0.0f);
    triVerticies.emplace_back(-0.32f, 0.24f);
    triVerticies.emplace_back(-0.32f, -0.24f);
    modelStoresPtr_->GenerateTriangles(triVerticies, shape03Index_);

    std::vector<glm::vec2> lineVerticies;
    lineVerticies.emplace_back(-0.24f, -0.16f);
    lineVerticies.emplace_back(-0.24f, 0.0f);
    lineVerticies.emplace_back(-0.24f, 0.16f);
    lineVerticies.emplace_back(-0.16f, 0.24f);
    lineVerticies.emplace_back(0.0f, 0.24f);
    lineVerticies.emplace_back(0.16f, 0.24f);
    lineVerticies.emplace_back(0.24f, 0.16f);
    lineVerticies.emplace_back(0.24f, 0.0f);
    lineVerticies.emplace_back(0.24f, -0.16f);
    lineVerticies.emplace_back(0.16f, -0.24f);
    lineVerticies.emplace_back(0.0f, -0.24f);
    lineVerticies.emplace_back(-0.06f, -0.24f);
    lineVerticies.emplace_back(-0.12f, -0.16f);
    lineVerticies.emplace_back(-0.12f, 0.0f);
    lineVerticies.emplace_back(-0.12f, 0.08f);
    lineVerticies.emplace_back(0.0f, 0.08f);
    lineVerticies.emplace_back(0.12f, 0.08f);
    lineVerticies.emplace_back(0.12f, 0.0f);
    lineVerticies.emplace_back(0.12f, -0.08f);
    lineVerticies.emplace_back(0.0f, -0.08f);
    lineVerticies.emplace_back(0.0f, 0.0f);
    modelStoresPtr_->Generate2DLine(lineVerticies, 0.08f, shape04Index_);

    std::vector<glm::vec2> lineVerticies2;
    lineVerticies2.emplace_back(0.08f, 0.0f);
    lineVerticies2.emplace_back(0.24f, 0.16f);
    lineVerticies2.emplace_back(0.08f, 0.08f);
    lineVerticies2.emplace_back(0.08f, 0.24f);
    lineVerticies2.emplace_back(0.0f, 0.08f);
    lineVerticies2.emplace_back(-0.08f, 0.24f);
    lineVerticies2.emplace_back(-0.08f, 0.08f);
    lineVerticies2.emplace_back(-0.24f, 0.16f);
    lineVerticies2.emplace_back(-0.08f, 0.0f);
    lineVerticies2.emplace_back(-0.24f, -0.16f);
    lineVerticies2.emplace_back(-0.08f, -0.16f);
    lineVerticies2.emplace_back(-0.08f, -0.24f);
    lineVerticies2.emplace_back(0.0f, -0.16f);
    lineVerticies2.emplace_back(0.08f, -0.24f);
    lineVerticies2.emplace_back(0.08f, -0.16f);
    lineVerticies2.emplace_back(0.24f, -0.16f);
    lineVerticies2.emplace_back(0.16f, -0.08f);
    modelStoresPtr_->Generate2DLine(lineVerticies2, 0.04f, shape05Index_);

    textureStoresPtr_->LoadTexture("../Textures/CounterclockwiseDie.png", diceTexture01Index_);
    Project001::TextureData diceTexture01Data;
    textureStoresPtr_->GetTexture(diceTexture01Index_, diceTexture01Data);
    rendererPtr_->AddTexture(diceTexture01Index_, diceTexture01Data.data,
        diceTexture01Data.width, diceTexture01Data.height, diceTexture01Data.numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/HallowDie.png", diceTexture02Index_);
    Project001::TextureData diceTexture02Data;
    textureStoresPtr_->GetTexture(diceTexture02Index_, diceTexture02Data);
    rendererPtr_->AddTexture(diceTexture02Index_, diceTexture02Data.data,
        diceTexture02Data.width, diceTexture02Data.height, diceTexture02Data.numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/Thonk.png", thonkTextureIndex_);
    Project001::TextureData thonkTextureData;
    textureStoresPtr_->GetTexture(thonkTextureIndex_, thonkTextureData);
    rendererPtr_->AddTexture(thonkTextureIndex_, thonkTextureData.data,
        thonkTextureData.width, thonkTextureData.height, thonkTextureData.numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/Specular2.png", patternSpecularIndex_);
    Project001::TextureData patternSpecularData;
    textureStoresPtr_->GetTexture(patternSpecularIndex_, patternSpecularData);
    rendererPtr_->AddTexture(patternSpecularIndex_, patternSpecularData.data,
        patternSpecularData.width, patternSpecularData.height, patternSpecularData.numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/ThonkSpecular.png", thonkSpecularIndex_);
    Project001::TextureData thonkSpecularData;
    textureStoresPtr_->GetTexture(thonkSpecularIndex_, thonkSpecularData);
    rendererPtr_->AddTexture(thonkSpecularIndex_, thonkSpecularData.data,
        thonkSpecularData.width, thonkSpecularData.height, thonkSpecularData.numberOfComponents);

    // scene data entity
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(sceneDataEntityId_);
        componentStoresPtr_->CreateComponent<Project001::SceneDataComponent>(sceneDataEntityId_);
    }

    // main camera entity
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(mainCameraEntityId_);
        componentStoresPtr_->CreateComponent<Project001::CameraComponent>(mainCameraEntityId_);

        Project001::CameraComponent* cameraComponentPtr;
        componentStoresPtr_->GetComponent<Project001::CameraComponent>(mainCameraEntityId_, cameraComponentPtr);
        cameraComponentPtr->position.z = 3.0f;
        //cameraComponentPtr->position.x = 3.0f;
        int framebufferWidth;
        int framebufferHeight;
        windowPtr_->GetFramebufferSize(framebufferWidth, framebufferHeight);
        cameraComponentPtr->aspectRatio = (float)framebufferWidth / (float)framebufferHeight;
        //cameraComponentPtr->orthographicProjection = true;
        //cameraComponentPtr->leftCutOff = -3.0f;
        //cameraComponentPtr->rightCutOff = 3.0f;
        //cameraComponentPtr->bottomCutOff = cameraComponentPtr->leftCutOff / cameraComponentPtr->aspectRatio;
        //cameraComponentPtr->topCutOff = cameraComponentPtr->rightCutOff / cameraComponentPtr->aspectRatio;
        //glm::quat rotationQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, cameraComponentPtr->s_worldUp);
        //cameraComponentPtr->orientation = rotationQuaternion * cameraComponentPtr->orientation;
    }

    // light source entity
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(lightSourceEntityId_);
        componentStoresPtr_->CreateComponent<Project001::LightComponent>(lightSourceEntityId_);

        Project001::LightComponent* lightComponentPtr;
        componentStoresPtr_->GetComponent<Project001::LightComponent>(lightSourceEntityId_, lightComponentPtr);
        lightComponentPtr->specular = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    // cube entity 01
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity01Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(cubeEntity01Id_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(cubeEntity01Id_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(-2.0f, 1.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = cubeModelIndex_;
        renderedModelComponentPtr->textureIndex = thonkTextureIndex_;
        renderedModelComponentPtr->specularIndex = thonkSpecularIndex_;
    }

    // cube entity 02
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity02Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(cubeEntity02Id_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(cubeEntity02Id_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(-1.0f, 1.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = cubeModelIndex_;
        renderedModelComponentPtr->textureIndex = diceTexture01Index_;
    }

    // cube entity 03
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity03Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(cubeEntity03Id_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(cubeEntity03Id_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(0.0f, 1.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = cubeModelIndex_;
        renderedModelComponentPtr->textureIndex = diceTexture02Index_;
        renderedModelComponentPtr->color = glm::vec4(0.2f, 0.8f, 0.6f, 1.0f);
    }

    // cube entity 04
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity04Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(cubeEntity04Id_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(cubeEntity04Id_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(1.0f, 1.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = cubeModelIndex_;
        renderedModelComponentPtr->specularIndex = patternSpecularIndex_;
        renderedModelComponentPtr->color = glm::vec4(0.8f, 0.2f, 0.6f, 1.0f);
        renderedModelComponentPtr->scale = glm::vec3(0.5f, 0.75f, 1.0f);
        glm::quat rotationQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        renderedModelComponentPtr->orientation = rotationQuaternion * renderedModelComponentPtr->orientation;
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape01EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(shape01EntityId_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(shape01EntityId_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(2.0f, 1.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape01Index_;
        renderedModelComponentPtr->color = glm::vec4(0.8f, 0.6f, 0.2f, 1.0f);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape02EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(shape02EntityId_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(shape02EntityId_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(-2.0f, 0.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape02Index_;
        renderedModelComponentPtr->color = glm::vec4(0.2f, 0.6f, 0.8f, 1.0f);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape03EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(shape03EntityId_);
        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(shape03EntityId_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(-1.0f, 0.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape03Index_;
        renderedModelComponentPtr->color = glm::vec4(0.6f, 0.2f, 0.8f, 1.0f);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape04EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(shape04EntityId_);
        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(shape04EntityId_, renderedModelComponentPtr);
        //renderedModelComponentPtr->position = glm::vec3(0.0f, 0.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape04Index_;
        renderedModelComponentPtr->color = glm::vec4(0.6f, 0.8f, 0.2f, 1.0f);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape05EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(shape05EntityId_);
        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(shape05EntityId_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(1.0f, 0.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape05Index_;
        renderedModelComponentPtr->color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
    }
}

void TestScene::Deinitialize()
{
    componentStoresPtr_->DeleteEntity(sceneDataEntityId_);
    componentStoresPtr_->DeleteEntity(mainCameraEntityId_);
    componentStoresPtr_->DeleteEntity(lightSourceEntityId_);

    componentStoresPtr_->DeleteEntity(cubeEntity01Id_);
    componentStoresPtr_->DeleteEntity(cubeEntity02Id_);
    componentStoresPtr_->DeleteEntity(cubeEntity03Id_);
    componentStoresPtr_->DeleteEntity(cubeEntity04Id_);

    componentStoresPtr_->DeleteEntity(shape01EntityId_);
    componentStoresPtr_->DeleteEntity(shape02EntityId_);
    componentStoresPtr_->DeleteEntity(shape03EntityId_);
}

void TestScene::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene::ProcessCursorPosition, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene::ProcessMouseButton, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene::ProcessScroll, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene::Update, this, std::placeholders::_1));
}

// protected: ------------------------------------------------------------------

void TestScene::ProcessCursorPosition(Project001::CursorPositionEvent& cursorButtonEvent)
{
    bool mouseButton1Pressed = windowPtr_->GetMouseButtonPressed(Project001::MouseButton::MOUSE_BUTTON_1);

    if (mouseButton1Pressed)
    {
        float& currentXPosition = cursorButtonEvent.xPosition;
        float& currentYPosition = cursorButtonEvent.yPosition;

        Project001::SceneDataComponent* sceneDataComponentPtr;
        componentStoresPtr_->GetComponent<Project001::SceneDataComponent>(sceneDataEntityId_, sceneDataComponentPtr);

        float& prevousXPosition = sceneDataComponentPtr->previousCursorDownPosition.x;
        float& prevousYPosition = sceneDataComponentPtr->previousCursorDownPosition.y;

        Project001::CameraComponent* cameraComponentPtr;
        componentStoresPtr_->GetComponent<Project001::CameraComponent>(mainCameraEntityId_, cameraComponentPtr);

        glm::quat& orientation = cameraComponentPtr->orientation;

        float speedConstant = 0.005f;

        float xOffset = currentXPosition - prevousXPosition;
        float yOffset = currentYPosition - prevousYPosition;

        float cameraYaw = -1.0f * xOffset * speedConstant;
        float cameraPitch = -1.0f * yOffset * speedConstant;

        glm::vec3 directionRight = cameraComponentPtr->GetRightVector();
        glm::vec3 directionUp = cameraComponentPtr->GetUpVector();

        glm::quat yawQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), cameraYaw, directionUp);
        glm::quat pitchQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), cameraPitch, directionRight);

        orientation = pitchQuaternion * yawQuaternion * orientation;

        prevousXPosition = currentXPosition;
        prevousYPosition = currentYPosition;
    }
}

void TestScene::ProcessMouseButton(Project001::MouseButtonEvent& mouseButtonEvent)
{
    Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;

    if (mouseButton == Project001::MouseButton::MOUSE_BUTTON_1 &&
        buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
    {
        Project001::SceneDataComponent* sceneDataComponentPtr;
        componentStoresPtr_->GetComponent<Project001::SceneDataComponent>(sceneDataEntityId_, sceneDataComponentPtr);

        float& prevousXPosition = sceneDataComponentPtr->previousCursorDownPosition.x;
        float& prevousYPosition = sceneDataComponentPtr->previousCursorDownPosition.y;

        windowPtr_->GetCursorPosition(prevousXPosition, prevousYPosition);
    }
}

void TestScene::ProcessScroll(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.25f;
    float cameraTranslationSpeed = speedConstant * yOffset;

    Project001::CameraComponent* cameraComponentPtr;
    componentStoresPtr_->GetComponent<Project001::CameraComponent>(mainCameraEntityId_, cameraComponentPtr);

    cameraComponentPtr->position += cameraTranslationSpeed * cameraComponentPtr->GetForwardVector();
}

void TestScene::Update(Project001::UpdateEvent& updateEvent)
{
    double timestep = updateEvent.timestep_s;

    // Update Entities
    UpdateMainCameraEntityPositionAndRoll(timestep);

    UpdateLightEntityPosition();

    // Delete all entities with marked for deletion component
    DeleteDeadEntities();

    // Render all renderable components
    RenderRenderableEntities();

    // Play all audio components???

    updateEvent.handled = true;
}

void TestScene::UpdateMainCameraEntityPositionAndRoll(double timestep)
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

    Project001::CameraComponent* cameraComponentPtr;
    componentStoresPtr_->GetComponent<Project001::CameraComponent>(mainCameraEntityId_, cameraComponentPtr);

    glm::vec3 directionRight = cameraComponentPtr->GetRightVector();
    glm::vec3 directionUp = cameraComponentPtr->GetUpVector();
    glm::vec3 directionForward = cameraComponentPtr->GetForwardVector();

    glm::vec3& cameraPosition = cameraComponentPtr->position;
    glm::quat& cameraOrientation = cameraComponentPtr->orientation;

    if (movingLeft)
    {
        cameraPosition -= cameraTranslationSpeed * directionRight;
    }

    if (movingRight)
    {
        cameraPosition += cameraTranslationSpeed * directionRight;
    }

    if (movingUp)
    {
        cameraPosition += cameraTranslationSpeed * directionUp;
    }

    if (movingDown)
    {
        cameraPosition -= cameraTranslationSpeed * directionUp;
    }

    if (rollingLeft)
    {
        glm::quat rollQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), -1.0f * cameraRotationSpeed, directionForward);
        cameraOrientation = rollQuaternion * cameraOrientation;
    }
    if (rollingRight)
    {
        glm::quat rollQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), cameraRotationSpeed, directionForward);
        cameraOrientation = rollQuaternion * cameraOrientation;
    }
}

void TestScene::UpdateLightEntityPosition()
{
    Project001::LightComponent* lightComponentPtr;
    componentStoresPtr_->GetComponent<Project001::LightComponent>(lightSourceEntityId_, lightComponentPtr);

    Project001::CameraComponent* cameraComponentPtr;
    componentStoresPtr_->GetComponent<Project001::CameraComponent>(mainCameraEntityId_, cameraComponentPtr);

    lightComponentPtr->position = cameraComponentPtr->position;
}

void TestScene::DeleteDeadEntities()
{
    Project001::DeathFlagComponent* deathFlagComponentPtrs = nullptr;
    size_t deathFlagComponentCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::DeathFlagComponent>(deathFlagComponentPtrs, deathFlagComponentCount);

    for (unsigned int i = 0; i < deathFlagComponentCount; ++i)
    {
        Project001::DeathFlagComponent& currentDeathFlagComponent = deathFlagComponentPtrs[i];
        if (currentDeathFlagComponent.dead)
        {
            componentStoresPtr_->DeleteEntity(currentDeathFlagComponent.entityId);
        }
    }
}

void TestScene::RenderRenderableEntities()
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->ClearBuffers();

    Project001::LightComponent* lightComponentPtrs = nullptr;
    size_t lightComponentCount = 0;
    
    componentStoresPtr_->GetAllComponents<Project001::LightComponent>(lightComponentPtrs, lightComponentCount);
    
    for (unsigned int i = 0; i < lightComponentCount; ++i)
    {
        Project001::LightComponent& currentLightComponent = lightComponentPtrs[i];
        if (currentLightComponent.turnedOn)
        {
            Project001::LightComponent::LightType lightType = currentLightComponent.lightType;
    
            if (lightType == Project001::LightComponent::LightType::LIGHT_TYPE_DIRECTIONAL_LIGHT)
            {
                rendererPtr_->SetDirectionalLight(
                    currentLightComponent.direction,
                    currentLightComponent.ambient,
                    currentLightComponent.diffuse,
                    currentLightComponent.specular
                );
            }
            else if (lightType == Project001::LightComponent::LightType::LIGHT_TYPE_POINT_LIGHT)
            {
                rendererPtr_->AddPointLight(
                    currentLightComponent.position,
                    currentLightComponent.constant,
                    currentLightComponent.linear,
                    currentLightComponent.quadratic,
                    currentLightComponent.ambient,
                    currentLightComponent.diffuse,
                    currentLightComponent.specular
                );
            }
            else if (lightType == Project001::LightComponent::LightType::LIGHT_TYPE_SPOT_LIGHT)
            {
                rendererPtr_->AddSpotLight(
                    currentLightComponent.position,
                    currentLightComponent.direction,
                    currentLightComponent.cutoff,
                    currentLightComponent.outerCutoff,
                    currentLightComponent.constant,
                    currentLightComponent.linear,
                    currentLightComponent.quadratic,
                    currentLightComponent.ambient,
                    currentLightComponent.diffuse,
                    currentLightComponent.specular
                );
            }
        }
    }

    Project001::RenderedModelComponent* renderedModelComponentPtrs = nullptr;
    size_t renderedModelComponentCount = 0;
    
    componentStoresPtr_->GetAllComponents<Project001::RenderedModelComponent>(renderedModelComponentPtrs, renderedModelComponentCount);
    
    for (unsigned int i = 0; i < renderedModelComponentCount; ++i)
    {
        Project001::RenderedModelComponent& currentRenderedModelComponent = renderedModelComponentPtrs[i];

        rendererPtr_->AddModel(
            currentRenderedModelComponent.modelIndex,
            currentRenderedModelComponent.textureIndex,
            currentRenderedModelComponent.specularIndex,
            currentRenderedModelComponent.shininess,
            currentRenderedModelComponent.color,
            currentRenderedModelComponent.scale,
            currentRenderedModelComponent.position,
            currentRenderedModelComponent.orientation
        );
    }

    Project001::CameraComponent* cameraComponentPtrs = nullptr;
    size_t cameraComponentCount = 0;
    
    componentStoresPtr_->GetAllComponents<Project001::CameraComponent>(cameraComponentPtrs, cameraComponentCount);
    
    for (unsigned int i = 0; i < cameraComponentCount; ++i)
    {
        Project001::CameraComponent& currentCameraComponent = cameraComponentPtrs[i];
        if (currentCameraComponent.turnedOn)
        {
            rendererPtr_->SetViewMatrix(currentCameraComponent.GetViewMatrix());
            rendererPtr_->SetViewPosition(currentCameraComponent.position);
            rendererPtr_->SetProjectionMatrix(currentCameraComponent.GetProjectionMatrix());

            rendererPtr_->Render();
    
            // Only rendering with one camera for now...
            break;
        }
    }
}

// private: --------------------------------------------------------------------

void TestScene::ComponentContainerTest() const
{
    bool testBool = false;

    Project001::ComponentContainer testComponentContainer;

    testBool = testComponentContainer.CreateComponent<Project001::TestComponent>(0, 10, 10, 10);
    testBool = testComponentContainer.CreateComponent<Project001::TestComponent>(0, 10, 10, 10);
    testBool = testComponentContainer.CreateComponent<Project001::TestComponent>(1, 20, 20, 20);
    testBool = testComponentContainer.CreateComponent<Project001::TestComponent>(2, 30, 30, 30);

    Project001::TestComponent* testComponent0 = nullptr;
    Project001::TestComponent* testComponent1 = nullptr;
    Project001::TestComponent* testComponent2 = nullptr;

    testBool = testComponentContainer.GetComponent<Project001::TestComponent>(0, testComponent0);
    testBool = testComponentContainer.GetComponent<Project001::TestComponent>(1, testComponent1);
    testBool = testComponentContainer.GetComponent<Project001::TestComponent>(2, testComponent2);

    Project001::TestComponent* allTestComponents = nullptr;
    size_t count = 0;

    testBool = testComponentContainer.GetAllComponents<Project001::TestComponent>(allTestComponents, count);

    testBool = testComponentContainer.DeleteComponent(0);
    testBool = testComponentContainer.DeleteComponent(0);
    testBool = testComponentContainer.GetComponent<Project001::TestComponent>(0, testComponent0);
}

void TestScene::ComponentStoresTest() const
{
    bool testBool = false;

    Project001::ComponentStores testComponentStores;

    testBool = testComponentStores.CreateComponent<Project001::TestComponent>(0, 0, 0, 0);

    unsigned int entity0;
    testBool = testComponentStores.CreateEntity(entity0);
    testBool = testComponentStores.CreateComponent<Project001::TestComponent>(entity0, 10, 10, 10);

    unsigned int entity1;
    testBool = testComponentStores.CreateEntity(entity1);
    testBool = testComponentStores.CreateComponent<Project001::TestComponent>(entity1, 11, 11, 11);

    unsigned int entity2;
    testBool = testComponentStores.CreateEntity(entity2);
    testBool = testComponentStores.CreateComponent<Project001::TestComponent>(entity2, 12, 12, 12);

    testBool = testComponentStores.DeleteEntity(9);
    testBool = testComponentStores.DeleteEntity(entity0);
    testBool = testComponentStores.DeleteEntity(entity0);

    unsigned int entity3;
    testBool = testComponentStores.CreateEntity(entity3);
    testBool = testComponentStores.CreateComponent<Project001::TestComponent>(entity3, 13, 13, 13);

    Project001::TestComponent* testComponent0 = nullptr;
    testBool = testComponentStores.GetComponent<Project001::TestComponent>(entity1, testComponent0);
    int sum = testComponent0->GetSum();
}

void TestScene::LRUArrayTest() const
{
    Project001::LRUArray<unsigned int> testLRUArray(4);
    testLRUArray.Add(0);
    testLRUArray.Add(1);
    testLRUArray.Add(2);
    testLRUArray.Add(3);
    unsigned int x0 = testLRUArray.Get(0);
    unsigned int x1 = testLRUArray.Get(1);
    unsigned int x2 = testLRUArray.Get(2);
    unsigned int x3 = testLRUArray.Get(3);
    size_t size = testLRUArray.Size();

    unsigned int temp = testLRUArray.Get(0);
    testLRUArray.Add(4);
    temp = testLRUArray.Get(2);
    testLRUArray.Add(5);
    testLRUArray.Add(6);

    x0 = testLRUArray[0];
    x1 = testLRUArray[1];
    x2 = testLRUArray[2];
    x3 = testLRUArray[3];

    testLRUArray[0] = 0;
    testLRUArray[1] = 1;
    testLRUArray[2] = 2;
    testLRUArray[3] = 3;
}

void TestScene::ModelStoresTest() const
{
    bool testBool = false;

    Project001::ModelStores testModelStores;

    unsigned int model0;
    testBool = testModelStores.LoadModel("", model0);
    testBool = testModelStores.LoadModel("../Models/CubeQ.obj", model0);
}

void TestScene::RendererTest() const
{
    // ...
}

void TestScene::TextureStoresTest() const
{
    bool testBool = false;

    Project001::TextureStores testTextureStores;

    unsigned int texture0;
    testBool = testTextureStores.LoadTexture("", texture0);
    testBool = testTextureStores.LoadTexture("../Textures/rgb.png", texture0);

    unsigned int texture1;
    testBool = testTextureStores.LoadTexture("../Textures/Cube.png", texture1);

    unsigned int texture2;
    testBool = testTextureStores.LoadTexture("../Textures/CubeSpecular.png", texture2);
}