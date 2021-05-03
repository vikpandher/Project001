#include "TestScene.h"

#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/LRUArray.h"
#include "Engine/ModelStores.h"
#include "Engine/SceneComponents.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: -----------------------------------------------------------------

TestScene::TestScene()
    : componentStoresPtr_(nullptr)
    , modelStoresPtr_(nullptr)
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
    , sceneDataEntityId_((unsigned int)-1)
    , mainCameraEntityId_((unsigned int)-1)
    , lightSourceEntityId_((unsigned int)-1)
    , cubeEntity01Id_((unsigned int)-1)
    , cubeEntity02Id_((unsigned int)-1)
    , cubeEntity03Id_((unsigned int)-1)
    , cubeEntity04Id_((unsigned int)-1)
    , shape01EntityId_((unsigned int)-1)
    , shape02EntityId_((unsigned int)-1)
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
    Project001::Window* windowPtr)
{
    componentStoresPtr_ = componentStoresPtr;
    modelStoresPtr_ = modelStoresPtr;
    textureStoresPtr_ = textureStoresPtr;
    windowPtr_ = windowPtr;

    modelStoresPtr_->LoadModel("../Models/Cube.obj", cubeModelIndex_);

    std::vector<glm::vec2> fanVerticies;
    fanVerticies.push_back(glm::vec2(0.32f, 0.0f));
    fanVerticies.push_back(glm::vec2(0.24f, 0.24f));
    fanVerticies.push_back(glm::vec2(0.0f, 0.32f));
    fanVerticies.push_back(glm::vec2(-0.24f, 0.24f));
    fanVerticies.push_back(glm::vec2(-0.32f, 0.0f));
    fanVerticies.push_back(glm::vec2(-0.24f, -0.24f));
    fanVerticies.push_back(glm::vec2(0.0f, -0.32f));
    fanVerticies.push_back(glm::vec2(0.24f, -0.24f));
    modelStoresPtr_->Generate2DTriangleFan(fanVerticies, shape01Index_);

    std::vector<glm::vec2> stripVerticies;
    stripVerticies.push_back(glm::vec2(-0.32f, -0.32f));
    stripVerticies.push_back(glm::vec2(-0.24f, -0.24f));
    stripVerticies.push_back(glm::vec2(-0.32f, 0.32f));
    stripVerticies.push_back(glm::vec2(-0.24f, 0.24f));
    stripVerticies.push_back(glm::vec2(0.32f, 0.32f));
    stripVerticies.push_back(glm::vec2(0.24f, 0.24f));
    stripVerticies.push_back(glm::vec2(0.32f, -0.32f));
    stripVerticies.push_back(glm::vec2(0.24f, -0.24f));
    stripVerticies.push_back(glm::vec2(-0.16f, -0.32f));
    stripVerticies.push_back(glm::vec2(-0.08f, -0.24f));
    stripVerticies.push_back(glm::vec2(-0.16f, 0.16f));
    stripVerticies.push_back(glm::vec2(-0.08f, 0.08f));
    stripVerticies.push_back(glm::vec2(0.16f, 0.16f));
    stripVerticies.push_back(glm::vec2(0.08f, 0.08f));
    stripVerticies.push_back(glm::vec2(0.16f, -0.16f));
    stripVerticies.push_back(glm::vec2(0.08f, -0.08f));
    modelStoresPtr->Generate2DTriangleStrip(stripVerticies, shape02Index_);

    textureStoresPtr->LoadTexture("../Textures/CounterclockwiseDie.png", diceTexture01Index_);
    Project001::TextureData* diceTexture01DataPtr;
    textureStoresPtr_->GetTexture(diceTexture01Index_, diceTexture01DataPtr);
    windowPtr_->AddTexture(diceTexture01Index_, diceTexture01DataPtr->data,
        diceTexture01DataPtr->width, diceTexture01DataPtr->height, diceTexture01DataPtr->numberOfComponents);

    textureStoresPtr->LoadTexture("../Textures/NumberDie.png", diceTexture02Index_);
    Project001::TextureData* diceTexture02DataPtr;
    textureStoresPtr_->GetTexture(diceTexture02Index_, diceTexture02DataPtr);
    windowPtr_->AddTexture(diceTexture02Index_, diceTexture02DataPtr->data,
        diceTexture02DataPtr->width, diceTexture02DataPtr->height, diceTexture02DataPtr->numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/Thonk.png", thonkTextureIndex_);
    Project001::TextureData* thonkTextureDataPtr;
    textureStoresPtr_->GetTexture(thonkTextureIndex_, thonkTextureDataPtr);
    windowPtr_->AddTexture(thonkTextureIndex_, thonkTextureDataPtr->data,
        thonkTextureDataPtr->width, thonkTextureDataPtr->height, thonkTextureDataPtr->numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/Specular2.png", patternSpecularIndex_);
    Project001::TextureData* patternSpecularDataPtr;
    textureStoresPtr_->GetTexture(patternSpecularIndex_, patternSpecularDataPtr);
    windowPtr_->AddTexture(patternSpecularIndex_, patternSpecularDataPtr->data,
        patternSpecularDataPtr->width, patternSpecularDataPtr->height, patternSpecularDataPtr->numberOfComponents);

    textureStoresPtr_->LoadTexture("../Textures/ThonkSpecular.png", thonkSpecularIndex_);
    Project001::TextureData* thonkSpecularDataPtr;
    textureStoresPtr_->GetTexture(thonkSpecularIndex_, thonkSpecularDataPtr);
    windowPtr_->AddTexture(thonkSpecularIndex_, thonkSpecularDataPtr->data,
        thonkSpecularDataPtr->width, thonkSpecularDataPtr->height, thonkSpecularDataPtr->numberOfComponents);

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
        renderedModelComponentPtr->position = glm::vec3(1.0f, 1.0f, 0.0f);
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
        renderedModelComponentPtr->color = glm::vec4(0.8f, 1.0f, 0.8f, 1.0f);
    }

    // cube entity 04
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity04Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(cubeEntity04Id_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(cubeEntity04Id_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(1.0f, 0.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = cubeModelIndex_;
        renderedModelComponentPtr->specularIndex = patternSpecularIndex_;
        renderedModelComponentPtr->color = glm::vec4(0.5f, 0.25f, 0.35f, 1.0f);
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
        renderedModelComponentPtr->position = glm::vec3(-1.0f, 1.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape01Index_;
        renderedModelComponentPtr->color = glm::vec4(1.0f, 0.6f, 0.2f, 1.0f);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape02EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModelComponent>(shape02EntityId_);

        Project001::RenderedModelComponent* renderedModelComponentPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModelComponent>(shape02EntityId_, renderedModelComponentPtr);
        renderedModelComponentPtr->position = glm::vec3(-1.0f, 0.0f, 0.0f);
        renderedModelComponentPtr->modelIndex = shape02Index_;
        renderedModelComponentPtr->color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
    }
}

void TestScene::Deinitialize()
{

}

void TestScene::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::CursorPositionEvent>(event, std::bind(&TestScene::ProcessCursorPosition, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::MouseButtonEvent>(event, std::bind(&TestScene::ProcessMouseButton, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::ScrollEvent>(event, std::bind(&TestScene::ProcessScroll, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene::Update, this, std::placeholders::_1));
}

// protected: --------------------------------------------------------------

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
    Project001::RenderData renderData;
    glm::mat4& viewMatrix = renderData.viewMatrix;
    glm::vec3& viewPosition = renderData.viewPosition;
    glm::mat4& projectionMatrix = renderData.projectionMatrix;
    Project001::DirectionalLight& directionalLight = renderData.directionalLight;
    std::vector<Project001::PointLight>& pointLights = renderData.pointLights;
    std::vector<Project001::SpotLight>& spotLights = renderData.spotLights;
    std::vector<Project001::VertexData>& vertexBuffer = renderData.vertexBuffer;
    std::vector<glm::uint>& indexBuffer = renderData.indexBuffer;

    // NOTE:
    // Index buffer will be useful for when I decide to draw meshes with shapes
    // other then triangles. Then I can reuse some verticies. Right now when an
    // object with faces other than triangles is used, it's mesh is broken up
    // into triangles by ModelStores when the file is read in.

    // Add light components to the renderData struct
    // -------------------------------------------------------------------------
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
                directionalLight.direction = currentLightComponent.direction;
                directionalLight.ambient = currentLightComponent.ambient;
                directionalLight.diffuse = currentLightComponent.diffuse;
                directionalLight.specular = currentLightComponent.specular;
            }
            else if (lightType == Project001::LightComponent::LightType::LIGHT_TYPE_POINT_LIGHT)
            {
                Project001::PointLight newPointLight;
                newPointLight.position = currentLightComponent.position;
                newPointLight.constant = currentLightComponent.constant;
                newPointLight.linear = currentLightComponent.linear;
                newPointLight.quadratic = currentLightComponent.quadratic;
                newPointLight.ambient = currentLightComponent.ambient;
                newPointLight.diffuse = currentLightComponent.diffuse;
                newPointLight.specular = currentLightComponent.specular;
                pointLights.push_back(newPointLight);
            }
            else if (lightType == Project001::LightComponent::LightType::LIGHT_TYPE_SPOT_LIGHT)
            {
                Project001::SpotLight newSpotLight;
                newSpotLight.position = currentLightComponent.position;
                newSpotLight.direction = currentLightComponent.direction;
                newSpotLight.cutoff = currentLightComponent.cutoff;
                newSpotLight.outerCutoff = currentLightComponent.outerCutoff;
                newSpotLight.constant = currentLightComponent.constant;
                newSpotLight.linear = currentLightComponent.linear;
                newSpotLight.quadratic = currentLightComponent.quadratic;
                newSpotLight.ambient = currentLightComponent.ambient;
                newSpotLight.diffuse = currentLightComponent.diffuse;
                newSpotLight.specular = currentLightComponent.specular;
                spotLights.push_back(newSpotLight);
            }
        }
    }

    // Add model verticies and indicies to the renderData struct
    // -------------------------------------------------------------------------
    Project001::RenderedModelComponent* renderedModelComponentPtrs = nullptr;
    size_t renderedModelComponentCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::RenderedModelComponent>(renderedModelComponentPtrs, renderedModelComponentCount);

    for (unsigned int i = 0; i < renderedModelComponentCount; ++i)
    {
        Project001::RenderedModelComponent& currentRenderedModelComponent = renderedModelComponentPtrs[i];

        Project001::ModelVertex* modelVerticies;
        glm::uint modelVertexCount;
        Project001::TextureData* textureData;
        Project001::TextureData* specularData;

        if (modelStoresPtr_->GetModel(currentRenderedModelComponent.modelIndex, modelVerticies, modelVertexCount))
        {
            float textureIndex = -1.0f;
            if (textureStoresPtr_->GetTexture(currentRenderedModelComponent.textureIndex, textureData))
            {
                textureIndex = (float)currentRenderedModelComponent.textureIndex;
            }

            float specularIndex = 99.0f;
            if (textureStoresPtr_->GetTexture(currentRenderedModelComponent.specularIndex, specularData))
            {
                specularIndex = (float)currentRenderedModelComponent.specularIndex;
            }

            for (unsigned int j = 0; j < modelVertexCount; ++j)
            {
                Project001::ModelVertex& currentModelVertex = modelVerticies[j];

                Project001::VertexData newVertex;
                newVertex.position = currentModelVertex.position;
                newVertex.textureCoordinte = currentModelVertex.textureCoordinte;
                newVertex.normal = currentModelVertex.normal;
                newVertex.color = currentRenderedModelComponent.color;
                newVertex.textureIndex = textureIndex;
                newVertex.specularIndex = specularIndex;
                newVertex.shininess = currentRenderedModelComponent.shininess;
                newVertex.scale = currentRenderedModelComponent.scale;
                newVertex.translation = currentRenderedModelComponent.position;
                newVertex.orientation.x = currentRenderedModelComponent.orientation.x;
                newVertex.orientation.y = currentRenderedModelComponent.orientation.y;
                newVertex.orientation.z = currentRenderedModelComponent.orientation.z;
                newVertex.orientation.w = currentRenderedModelComponent.orientation.w;

                vertexBuffer.push_back(newVertex);
                indexBuffer.push_back((glm::uint)indexBuffer.size());
            }
        }
    }

    // Render once for every camera component turned on
    // -------------------------------------------------------------------------
    Project001::CameraComponent* cameraComponentPtrs = nullptr;
    size_t cameraComponentCount = 0;

    componentStoresPtr_->GetAllComponents<Project001::CameraComponent>(cameraComponentPtrs, cameraComponentCount);

    for (unsigned int i = 0; i < cameraComponentCount; ++i)
    {
        Project001::CameraComponent& currentCameraComponent = cameraComponentPtrs[i];
        if (currentCameraComponent.turnedOn)
        {
            viewMatrix = currentCameraComponent.GetViewMatrix();
            viewPosition = currentCameraComponent.position;
            projectionMatrix = currentCameraComponent.GetProjectionMatrix();

            windowPtr_->Render(&renderData);
        }
    }
}

// private: ----------------------------------------------------------------

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
    Project001::RenderData testRenderData;
    glm::mat4& viewMatrix = testRenderData.viewMatrix;
    glm::vec3& viewPosition = testRenderData.viewPosition;
    glm::mat4& projectionMatrix = testRenderData.projectionMatrix;
    Project001::DirectionalLight& directionalLight = testRenderData.directionalLight;
    std::vector<Project001::PointLight>& pointLights = testRenderData.pointLights;
    std::vector<Project001::SpotLight>& spotLights = testRenderData.spotLights;
    std::vector<Project001::VertexData>& vertexBuffer = testRenderData.vertexBuffer;
    std::vector<glm::uint>& indexBuffer = testRenderData.indexBuffer;

    Project001::CameraComponent cameraComponent;
    cameraComponent.position.z = 5.0f;

    int framebufferWidth;
    int framebufferHeight;
    windowPtr_->GetFramebufferSize(framebufferWidth, framebufferHeight);

    cameraComponent.aspectRatio = (float)framebufferWidth / (float)framebufferHeight;

    viewMatrix = cameraComponent.GetViewMatrix();
    viewPosition = cameraComponent.position;
    projectionMatrix = cameraComponent.GetProjectionMatrix();

    glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
    //rotation = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / 2.0f, cameraComponent.s_worldForward);
    glm::vec4 rotationAsVec(rotation.x, rotation.y, rotation.z, rotation.w);

    Project001::VertexData v01;
    v01.orientation = rotationAsVec;
    Project001::VertexData v02;
    v02.position.x = 1.0f;
    v02.orientation = rotationAsVec;

    Project001::VertexData v03;
    v03.position.y = 0.5f;
    v03.orientation = rotationAsVec;

    vertexBuffer.push_back(v01);
    vertexBuffer.push_back(v02);
    vertexBuffer.push_back(v03);

    indexBuffer.push_back(0);
    indexBuffer.push_back(1);
    indexBuffer.push_back(2);

    windowPtr_->Render(&testRenderData);
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