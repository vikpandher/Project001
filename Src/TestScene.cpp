#include "TestScene.h"

#include "DeathFlag.h"
#include "TestComponents.h"
#include "TestSceneData.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/LightSource.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/LRUArray.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene::TestScene()
    : componentStoresPtr_(nullptr)
    , meshStoresPtr_(nullptr)
    , rendererPtr_(nullptr)
    , textureStoresPtr_(nullptr)
    , windowPtr_(nullptr)
    , cubeMeshIndex_((unsigned int)-1)
    , diceTexture01Index_((unsigned int)-1)
    , diceTexture02Index_((unsigned int)-1)
    , thonkTextureIndex_((unsigned int)-1)
    , _100x100TextureIndex_((unsigned int)-1)
    , patternSpecularIndex_((unsigned int)-1)
    , thonkSpecularIndex_((unsigned int)-1)
    , shape01Index_((unsigned int)-1)
    , shape02Index_((unsigned int)-1)
    , shape03Index_((unsigned int)-1)
    , shape04Index_((unsigned int)-1)
    , shape05Index_((unsigned int)-1)
    , shape06Index_((unsigned int)-1)
    , shape07Index_((unsigned int)-1)
    , shape08Index_((unsigned int)-1)
    , shape09Index_((unsigned int)-1)
    , shape10Index_((unsigned int)-1)
    , shape11Index_((unsigned int)-1)
    , shape12Index_((unsigned int)-1)
    , shape13Index_((unsigned int)-1)
    , shape14Index_((unsigned int)-1)
    , shape15Index_((unsigned int)-1)
    , sceneDataEntityId_((unsigned int)-1)
    , mainCameraEntityId_((unsigned int)-1)
    , lightSourceEntityId_((unsigned int)-1)
    , cubeEntity01Id_((unsigned int)-1)
    , cubeEntity02Id_((unsigned int)-1)
    , cubeEntity03Id_((unsigned int)-1)
    , cubeEntity04Id_((unsigned int)-1)
    , cubeEntity05Id_((unsigned int)-1)
    , shape01EntityId_((unsigned int)-1)
    , shape02EntityId_((unsigned int)-1)
    , shape03EntityId_((unsigned int)-1)
    , shape04EntityId_((unsigned int)-1)
    , shape05EntityId_((unsigned int)-1)
    , shape06EntityId_((unsigned int)-1)
    , shape07EntityId_((unsigned int)-1)
    , shape08EntityId_((unsigned int)-1)
    , shape09EntityId_((unsigned int)-1)
    , shape10EntityId_((unsigned int)-1)
    , shape11EntityId_((unsigned int)-1)
    , shape12EntityId_((unsigned int)-1)
    , shape13EntityId_((unsigned int)-1)
    , shape14EntityId_((unsigned int)-1)
    , shape15EntityId_((unsigned int)-1)
{
    ComponentContainerTest();
    ComponentStoresTest();
}

TestScene::~TestScene()
{}

const char* TestScene::Name()
{
    return "Test";
}

void TestScene::Initialize(
    Project001::ComponentStores* componentStoresPtr,
    Project001::MeshStores* meshStoresPtr,
    Project001::TextureStores* textureStoresPtr,
    Project001::Renderer* rendererPtr,
    Project001::Window* windowPtr)
{
    componentStoresPtr_ = componentStoresPtr;
    rendererPtr_ = rendererPtr;
    meshStoresPtr_ = meshStoresPtr;
    textureStoresPtr_ = textureStoresPtr;
    windowPtr_ = windowPtr;

    meshStoresPtr_->LoadMeshOBJ(cubeMeshIndex_, "../Models/Cube.obj", false);

    std::vector<glm::vec2> fanPositions;
    fanPositions.emplace_back(0.32f, 0.0f);
    fanPositions.emplace_back(0.24f, 0.24f);
    fanPositions.emplace_back(0.0f, 0.32f);
    fanPositions.emplace_back(-0.24f, 0.24f);
    fanPositions.emplace_back(-0.32f, 0.0f);
    fanPositions.emplace_back(-0.24f, -0.24f);
    fanPositions.emplace_back(0.0f, -0.32f);
    fanPositions.emplace_back(0.24f, -0.24f);
    for (size_t i = 0; i < fanPositions.size(); ++i)
    {
        fanPositions[i] += glm::vec2(0.5f, 0.5f);
    }
    meshStoresPtr_->Generate2DTriangleFan(shape01Index_, fanPositions, fanPositions);

    std::vector<glm::vec2> stripPositions;
    stripPositions.emplace_back(-0.32f, -0.32f);
    stripPositions.emplace_back(-0.24f, -0.24f);
    stripPositions.emplace_back(-0.32f, 0.32f);
    stripPositions.emplace_back(-0.24f, 0.24f);
    stripPositions.emplace_back(0.32f, 0.32f);
    stripPositions.emplace_back(0.24f, 0.24f);
    stripPositions.emplace_back(0.32f, -0.32f);
    stripPositions.emplace_back(0.24f, -0.24f);
    stripPositions.emplace_back(-0.16f, -0.32f);
    stripPositions.emplace_back(-0.08f, -0.24f);
    stripPositions.emplace_back(-0.16f, 0.16f);
    stripPositions.emplace_back(-0.08f, 0.08f);
    stripPositions.emplace_back(0.16f, 0.16f);
    stripPositions.emplace_back(0.08f, 0.08f);
    stripPositions.emplace_back(0.16f, -0.16f);
    stripPositions.emplace_back(0.08f, -0.08f);
    for (size_t i = 0; i < stripPositions.size(); ++i)
    {
        stripPositions[i] += glm::vec2(0.5f, 0.5f);
    }
    meshStoresPtr_->Generate2DTriangleStrip(shape02Index_, stripPositions, stripPositions);

    std::vector<glm::vec2> triPositions;
    triPositions.emplace_back(0.0f, 0.08f);
    triPositions.emplace_back(0.24f, 0.32f);
    triPositions.emplace_back(-0.24f, 0.32f);
    triPositions.emplace_back(0.08f, 0.0f);
    triPositions.emplace_back(0.32f, -0.24f);
    triPositions.emplace_back(0.32f, 0.24f);
    triPositions.emplace_back(0.0f, -0.08f);
    triPositions.emplace_back(-0.24f, -0.32f);
    triPositions.emplace_back(0.24f, -0.32f);
    triPositions.emplace_back(-0.08f, 0.0f);
    triPositions.emplace_back(-0.32f, 0.24f);
    triPositions.emplace_back(-0.32f, -0.24f);
    for (size_t i = 0; i < triPositions.size(); ++i)
    {
        triPositions[i] += glm::vec2(0.5f, 0.5f);
    }
    meshStoresPtr_->Generate2DTriangles(shape03Index_, triPositions, triPositions);

    std::vector<glm::vec2> linePositions;
    linePositions.emplace_back(-0.24f, -0.16f);
    linePositions.emplace_back(-0.24f, 0.0f);
    linePositions.emplace_back(-0.24f, 0.16f);
    linePositions.emplace_back(-0.16f, 0.24f);
    linePositions.emplace_back(0.0f, 0.24f);
    linePositions.emplace_back(0.16f, 0.24f);
    linePositions.emplace_back(0.24f, 0.16f);
    linePositions.emplace_back(0.24f, 0.0f);
    linePositions.emplace_back(0.24f, -0.16f);
    linePositions.emplace_back(0.16f, -0.24f);
    linePositions.emplace_back(0.0f, -0.24f);
    linePositions.emplace_back(-0.06f, -0.24f);
    linePositions.emplace_back(-0.12f, -0.16f);
    linePositions.emplace_back(-0.12f, 0.0f);
    linePositions.emplace_back(-0.12f, 0.08f);
    linePositions.emplace_back(0.0f, 0.08f);
    linePositions.emplace_back(0.12f, 0.08f);
    linePositions.emplace_back(0.12f, 0.0f);
    linePositions.emplace_back(0.12f, -0.08f);
    linePositions.emplace_back(0.0f, -0.08f);
    linePositions.emplace_back(0.0f, 0.0f);
    for (size_t i = 0; i < linePositions.size(); ++i)
    {
        linePositions[i] += glm::vec2(0.5f, 0.5f);
    }
    meshStoresPtr_->Generate2DLine(shape04Index_, linePositions, 0.08f);

    std::vector<glm::vec2> linePositions2;
    linePositions2.emplace_back(0.08f, 0.0f);
    linePositions2.emplace_back(0.24f, 0.16f);
    linePositions2.emplace_back(0.08f, 0.08f);
    linePositions2.emplace_back(0.08f, 0.24f);
    linePositions2.emplace_back(0.0f, 0.08f);
    linePositions2.emplace_back(-0.08f, 0.24f);
    linePositions2.emplace_back(-0.08f, 0.08f);
    linePositions2.emplace_back(-0.24f, 0.16f);
    linePositions2.emplace_back(-0.08f, 0.0f);
    linePositions2.emplace_back(-0.24f, -0.16f);
    linePositions2.emplace_back(-0.08f, -0.16f);
    linePositions2.emplace_back(-0.08f, -0.24f);
    linePositions2.emplace_back(0.0f, -0.16f);
    linePositions2.emplace_back(0.08f, -0.24f);
    linePositions2.emplace_back(0.08f, -0.16f);
    linePositions2.emplace_back(0.24f, -0.16f);
    linePositions2.emplace_back(0.16f, -0.08f);
    for (size_t i = 0; i < linePositions2.size(); ++i)
    {
        linePositions2[i] += glm::vec2(0.5f, 0.5f);
    }
    meshStoresPtr_->Generate2DLine(shape05Index_, linePositions2, 0.04f);

    meshStoresPtr_->Generate2DRegularPolygon(shape06Index_, 0.32f, 3);
    meshStoresPtr_->Generate2DRegularPolygon(shape07Index_, 0.32f, 4);
    meshStoresPtr_->Generate2DRegularPolygon(shape08Index_, 0.32f, 5);
    meshStoresPtr_->Generate2DRegularPolygon(shape09Index_, 0.32f, 6);
    meshStoresPtr_->Generate2DRegularPolygon(shape10Index_, 0.32f, 24);
    meshStoresPtr_->ScaleMesh(shape10Index_, glm::vec3(2.0f, 1.5f, 1.0f));
    meshStoresPtr_->RotateMesh(shape10Index_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    meshStoresPtr_->RotateMesh(shape10Index_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    meshStoresPtr_->RotateMesh(shape10Index_, glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / -4.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    meshStoresPtr_->TranslateMesh(shape10Index_, glm::vec3(0.16f, 0.0f, 0.0f));

    meshStoresPtr_->Generate2DArc(shape11Index_, 0.08f, 0.32f, 1, 0.0f, 0.5f * glm::pi<float>());

    meshStoresPtr_->Generate2DArc(shape12Index_, 0.08f, 0.32f, 2, 0.0f, 1.5f * glm::pi<float>());

    meshStoresPtr_->Generate2DArc(shape13Index_, 0.08f, 0.32f, 3, 0.0f, 2.0f * glm::pi<float>());

    meshStoresPtr_->Generate2DArc(shape14Index_, 0.08f, 0.32f, 4, 1.5f * glm::pi<float>(), 1.0f * glm::pi<float>());

    meshStoresPtr_->Generate2DArc(shape15Index_, 0.08f, 0.32f, 24, 1.25f * glm::pi<float>(), 0.75f * glm::pi<float>());

    textureStoresPtr_->LoadTexture(diceTexture01Index_, "../Textures/CounterclockwiseDie.png");
    Project001::TextureData diceTexture01Data;
    textureStoresPtr_->GetTexture(diceTexture01Index_, diceTexture01Data);
    rendererPtr_->AddTexture(diceTexture01Index_, diceTexture01Data.data,
        diceTexture01Data.width, diceTexture01Data.height, diceTexture01Data.numberOfComponents);

    textureStoresPtr_->LoadTexture(diceTexture02Index_, "../Textures/HallowDie.png");
    Project001::TextureData diceTexture02Data;
    textureStoresPtr_->GetTexture(diceTexture02Index_, diceTexture02Data);
    rendererPtr_->AddTexture(diceTexture02Index_, diceTexture02Data.data,
        diceTexture02Data.width, diceTexture02Data.height, diceTexture02Data.numberOfComponents);

    textureStoresPtr_->LoadTexture(thonkTextureIndex_, "../Textures/Thonk.png");
    Project001::TextureData thonkTextureData;
    textureStoresPtr_->GetTexture(thonkTextureIndex_, thonkTextureData);
    rendererPtr_->AddTexture(thonkTextureIndex_, thonkTextureData.data,
        thonkTextureData.width, thonkTextureData.height, thonkTextureData.numberOfComponents);

    textureStoresPtr_->LoadTexture(_100x100TextureIndex_, "../Textures/100x100.png");
    Project001::TextureData _100x100TextureData;
    textureStoresPtr->GetTexture(_100x100TextureIndex_, _100x100TextureData);
    rendererPtr->AddTexture(_100x100TextureIndex_, _100x100TextureData.data,
        _100x100TextureData.width, _100x100TextureData.height, _100x100TextureData.numberOfComponents);

    textureStoresPtr_->LoadTexture(patternSpecularIndex_, "../Textures/Specular2.png");
    Project001::TextureData patternSpecularData;
    textureStoresPtr_->GetTexture(patternSpecularIndex_, patternSpecularData);
    rendererPtr_->AddTexture(patternSpecularIndex_, patternSpecularData.data,
        patternSpecularData.width, patternSpecularData.height, patternSpecularData.numberOfComponents);

    textureStoresPtr_->LoadTexture(thonkSpecularIndex_, "../Textures/ThonkSpecular.png");
    Project001::TextureData thonkSpecularData;
    textureStoresPtr_->GetTexture(thonkSpecularIndex_, thonkSpecularData);
    rendererPtr_->AddTexture(thonkSpecularIndex_, thonkSpecularData.data,
        thonkSpecularData.width, thonkSpecularData.height, thonkSpecularData.numberOfComponents);

    // scene data entity
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(sceneDataEntityId_);
        componentStoresPtr_->CreateComponent<TestSceneData>(sceneDataEntityId_);
    }

    // main camera entity
    // -------------------------------------------------------------------------
    {
        componentStoresPtr->CreateEntity(mainCameraEntityId_);
        componentStoresPtr->CreateComponent<Project001::Camera>(mainCameraEntityId_);

        Project001::Camera* cameraPtr;
        componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr);
        int framebufferWidth;
        int framebufferHeight;
        windowPtr_->GetFramebufferSize(framebufferWidth, framebufferHeight);
        float aspectRatio = (float)framebufferWidth / (float)framebufferHeight;
        cameraPtr->SetAspectRatio(aspectRatio);
        cameraPtr->SetPosition(0.0f, 0.0f, 5.0f);
        // cameraPtr->SetProjectionToOrthographic();
        // cameraPtr->SetLeftCutoff(aspectRatio * -5.0f);
        // cameraPtr->SetRightCutoff(aspectRatio * 5.0f);
        cameraPtr->TurnOn();
    }

    // light source entity
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(lightSourceEntityId_);
        componentStoresPtr_->CreateComponent<Project001::LightSource>(lightSourceEntityId_);

        Project001::LightSource* lightSourcePtr;
        componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr);
        lightSourcePtr->SetAmbientColor(0.1f, 0.1f, 0.1f);
        lightSourcePtr->SetSpecularColor(1.0f, 1.0f, 1.0f);
        lightSourcePtr->TurnOn();
    }

    // Rendered Models
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    modelEntityPositions.emplace_back(-2.0f,  1.0f, 0.0f);
    modelEntityPositions.emplace_back(-1.0f,  1.0f, 0.0f);
    modelEntityPositions.emplace_back( 0.0f,  1.0f, 0.0f);
    modelEntityPositions.emplace_back( 1.0f,  1.0f, 0.0f);
    modelEntityPositions.emplace_back( 2.0f,  1.0f, 0.0f);
    modelEntityPositions.emplace_back(-2.0f,  0.0f, 0.0f);
    modelEntityPositions.emplace_back(-1.0f,  0.0f, 0.0f);
    modelEntityPositions.emplace_back( 0.0f,  0.0f, 0.0f);
    modelEntityPositions.emplace_back( 1.0f,  0.0f, 0.0f);
    modelEntityPositions.emplace_back( 2.0f,  0.0f, 0.0f);
    modelEntityPositions.emplace_back(-2.0f, -1.0f, 0.0f);
    modelEntityPositions.emplace_back(-1.0f, -1.0f, 0.0f);
    modelEntityPositions.emplace_back( 0.0f, -1.0f, 0.0f);
    modelEntityPositions.emplace_back( 1.0f, -1.0f, 0.0f);
    modelEntityPositions.emplace_back( 2.0f, -1.0f, 0.0f);
    modelEntityPositions.emplace_back(-2.0f, -2.0f, 0.0f);
    modelEntityPositions.emplace_back(-1.0f, -2.0f, 0.0f);
    modelEntityPositions.emplace_back( 0.0f, -2.0f, 0.0f);
    modelEntityPositions.emplace_back( 1.0f, -2.0f, 0.0f);
    modelEntityPositions.emplace_back( 2.0f, -2.0f, 0.0f);

    // cube entity 01
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity01Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity01Id_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity01Id_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[0]);
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetTextureIndex(thonkTextureIndex_);
        renderedModelPtr->SetSpecularIndex(thonkSpecularIndex_);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 02
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity02Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity02Id_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity02Id_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[1]);
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetTextureIndex(diceTexture01Index_);
        renderedModelPtr->SetShininess(32.0f);
    }

    // cube entity 03
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity03Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity03Id_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity03Id_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[2]);
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetTextureIndex(diceTexture02Index_);
        renderedModelPtr->SetShininess(32.0f);
        renderedModelPtr->SetColorRGB(0.2f, 0.8f, 0.6f);
    }

    // cube entity 04
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity05Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity05Id_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity05Id_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[3]);
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetColor(1.0f, 1.0f, 1.0f, 0.30f);
        renderedModelPtr->SetTranslucent(true);
    }

    // cube entity 05
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(cubeEntity04Id_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(cubeEntity04Id_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(cubeEntity04Id_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[4]);
        renderedModelPtr->SetMeshIndex(cubeMeshIndex_);
        renderedModelPtr->SetSpecularIndex(patternSpecularIndex_);
        renderedModelPtr->SetShininess(32.0f);
        renderedModelPtr->SetColorRGB(0.8f, 0.2f, 0.6f);
        renderedModelPtr->SetScale(0.5f, 0.75f, 1.0f);
        glm::quat rotationQuaternion = glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        renderedModelPtr->AddRoll(glm::pi<float>() / 4.0f);
    }

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape01EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape01EntityId_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape01EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[5]);
        renderedModelPtr->SetMeshIndex(shape01Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.8f, 0.6f, 0.2f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape02EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape02EntityId_);

        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape02EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[6]);
        renderedModelPtr->SetMeshIndex(shape02Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.2f, 0.6f, 0.8f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape03EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape03EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape03EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[7]);
        renderedModelPtr->SetMeshIndex(shape03Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.6f, 0.2f, 0.8f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape04EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape04EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape04EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[8]);
        renderedModelPtr->SetMeshIndex(shape04Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.6f, 0.8f, 0.2f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape05EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape05EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape05EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[9]);
        renderedModelPtr->SetMeshIndex(shape05Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(1.0f, 0.5f, 0.5f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape06EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape06EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape06EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[10]);
        renderedModelPtr->SetMeshIndex(shape06Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape07EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape07EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape07EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[11]);
        renderedModelPtr->SetMeshIndex(shape07Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape08EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[12]);
        renderedModelPtr->SetMeshIndex(shape08Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape09EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape09EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape09EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[13]);
        renderedModelPtr->SetMeshIndex(shape09Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(1.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape10EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape10EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape10EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[14]);
        renderedModelPtr->SetMeshIndex(shape10Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.5f, 0.5f, 0.5f);
        renderedModelPtr->SetTranslucent(true);
        renderedModelPtr->SetShininess(32.0f);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape11EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape11EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape11EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[15]);
        renderedModelPtr->SetMeshIndex(shape11Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape12EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape12EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape12EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[16]);
        renderedModelPtr->SetMeshIndex(shape12Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 13
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape13EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape13EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape13EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[17]);
        renderedModelPtr->SetMeshIndex(shape13Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(1.0f, 0.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 14
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape14EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape14EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape14EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[18]);
        renderedModelPtr->SetMeshIndex(shape14Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.0f, 1.0f, 0.0f);
        renderedModelPtr->SetTranslucent(true);
    }

    // generated shape entity 15
    // -------------------------------------------------------------------------
    {
        componentStoresPtr_->CreateEntity(shape15EntityId_);
        componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape15EntityId_);
        Project001::RenderedModel* renderedModelPtr;
        componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape15EntityId_, renderedModelPtr);
        renderedModelPtr->SetPosition(modelEntityPositions[19]);
        renderedModelPtr->SetMeshIndex(shape15Index_);
        renderedModelPtr->SetTextureIndex(_100x100TextureIndex_);
        renderedModelPtr->SetColorRGB(0.0f, 0.0f, 1.0f);
        renderedModelPtr->SetTranslucent(true);
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

        TestSceneData* testSceneDataPtr;
        componentStoresPtr_->GetComponent<TestSceneData>(sceneDataEntityId_, testSceneDataPtr);

        float& prevousXPosition = testSceneDataPtr->previousCursorDownPosition.x;
        float& prevousYPosition = testSceneDataPtr->previousCursorDownPosition.y;

        Project001::Camera* cameraPtr;
        componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr);

        float speedConstant = 0.005f;

        float xOffset = currentXPosition - prevousXPosition;
        float yOffset = currentYPosition - prevousYPosition;

        float cameraYaw = -1.0f * xOffset * speedConstant;
        float cameraPitch = -1.0f * yOffset * speedConstant;

        cameraPtr->AddYaw(cameraYaw);
        // cameraPtr->AddWorldRotationY(cameraYaw); // for fps camera
        cameraPtr->AddPitch(cameraPitch);

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
        TestSceneData* testSceneDataPtr;
        componentStoresPtr_->GetComponent<TestSceneData>(sceneDataEntityId_, testSceneDataPtr);

        float& prevousXPosition = testSceneDataPtr->previousCursorDownPosition.x;
        float& prevousYPosition = testSceneDataPtr->previousCursorDownPosition.y;

        windowPtr_->GetCursorPosition(prevousXPosition, prevousYPosition);
    }
}

void TestScene::ProcessScroll(Project001::ScrollEvent& scrollEvent)
{
    float& yOffset = scrollEvent.yOffset;

    float speedConstant = 0.25f;
    float cameraTranslation = speedConstant * yOffset;

    Project001::Camera* cameraPtr;
    componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr);
    cameraPtr->MoveForward(cameraTranslation);
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

    Project001::Camera* cameraPtr;
    componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr);

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

void TestScene::UpdateLightEntityPosition()
{
    Project001::LightSource* lightSourcePtr;
    componentStoresPtr_->GetComponent<Project001::LightSource>(lightSourceEntityId_, lightSourcePtr);

    Project001::Camera* cameraPtr;
    componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr);

    lightSourcePtr->SetPosition(cameraPtr->GetPosition());
}

void TestScene::DeleteDeadEntities()
{
    DeathFlag* deathFlagArray = nullptr;
    size_t deathFlagCount = 0;

    componentStoresPtr_->GetAllComponents<DeathFlag>(deathFlagArray, deathFlagCount);

    for (unsigned int i = 0; i < deathFlagCount; ++i)
    {
        DeathFlag& currentDeathFlag = deathFlagArray[i];
        if (currentDeathFlag.dead)
        {
            unsigned int entityId = (unsigned int)-1;
            componentStoresPtr_->GetComponentEntityId<DeathFlag>(&currentDeathFlag, entityId);
            componentStoresPtr_->DeleteEntity(entityId);
        }
    }
}

void TestScene::RenderRenderableEntities()
{
    rendererPtr_->ClearDirectionalLight();
    rendererPtr_->ClearPointLights();
    rendererPtr_->ClearSpotLights();

    rendererPtr_->ClearBuffers();

    Project001::LightSource* lightSourceArray = nullptr;
    size_t lightSourceCount = 0;
    
    componentStoresPtr_->GetAllComponents<Project001::LightSource>(lightSourceArray, lightSourceCount);
    
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

        rendererPtr_->AddModel(
            currentRenderedModel.GetMeshIndex(),
            currentRenderedModel.GetTextureIndex(),
            currentRenderedModel.GetSpecularIndex(),
            currentRenderedModel.GetShininess(),
            currentRenderedModel.GetColor(),
            currentRenderedModel.GetTranslucent(),
            currentRenderedModel.GetScale(),
            currentRenderedModel.GetPosition(),
            currentRenderedModel.GetOrientation()
        );
    }

    Project001::Camera* cameraPtr;
    componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr);

    if (cameraPtr->IsTurnedOn())
    {
        rendererPtr_->SetViewMatrix(cameraPtr->GetViewMatrix());
        rendererPtr_->SetViewPosition(cameraPtr->GetPosition());
        rendererPtr_->SetProjectionMatrix(cameraPtr->GetProjectionMatrix());
        rendererPtr_->Render();
    }
}

// private: --------------------------------------------------------------------

void TestScene::ComponentContainerTest() const
{
    bool testBool = false;

    Project001::ComponentContainer testComponentContainer;

    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 10, 10, 10);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 10, 10, 10);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(1, 20, 20, 20);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(2, 30, 30, 30);

    TestComponent00* testComponent0 = nullptr;
    TestComponent00* testComponent1 = nullptr;
    TestComponent00* testComponent2 = nullptr;

    testBool = testComponentContainer.GetComponent<TestComponent00>(0, testComponent0);
    testBool = testComponentContainer.GetComponent<TestComponent00>(1, testComponent1);
    testBool = testComponentContainer.GetComponent<TestComponent00>(2, testComponent2);

    TestComponent00* allTestComponents = nullptr;
    size_t count = 0;

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentContainer.DeleteComponent(0);
    testBool = testComponentContainer.DeleteComponent(0);
    testBool = testComponentContainer.GetComponent<TestComponent00>(0, testComponent0);

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testComponentContainer.DeleteAllComponents();

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 11, 11, 11);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(0, 11, 11, 11);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(1, 22, 22, 22);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(2, 33, 33, 33);
    testBool = testComponentContainer.CreateComponent<TestComponent00>(3, 44, 44, 44);

    testBool = testComponentContainer.GetComponent<TestComponent00>(0, testComponent0);
    testBool = testComponentContainer.GetComponent<TestComponent00>(1, testComponent1);

    unsigned int returnedEntityId0 = (unsigned int)-1;
    unsigned int returnedEntityId1 = (unsigned int)-1;
    unsigned int returnedEntityId2 = (unsigned int)-1;
    testBool = testComponentContainer.GetComponentEntityId<TestComponent00>(testComponent0, returnedEntityId0);
    testBool = testComponentContainer.GetComponentEntityId<TestComponent00>(testComponent1, returnedEntityId1);
    testBool = testComponentContainer.GetComponentEntityId<TestComponent00>(testComponent2, returnedEntityId2);

    testComponentContainer.DeleteAllComponents();

    testBool = testComponentContainer.GetAllComponents<TestComponent00>(allTestComponents, count);
}

void TestScene::ComponentStoresTest() const
{
    bool testBool = false;

    Project001::ComponentStores testComponentStores;

    testBool = testComponentStores.CreateComponent<TestComponent00>(0, 0, 0, 0);

    unsigned int entity0;
    testBool = testComponentStores.CreateEntity(entity0);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity0, 10, 10, 10);

    unsigned int entity1;
    testBool = testComponentStores.CreateEntity(entity1);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity1, 11, 11, 11);

    unsigned int entity2;
    testBool = testComponentStores.CreateEntity(entity2);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity2, 12, 12, 12);

    testBool = testComponentStores.DeleteEntity(9);
    testBool = testComponentStores.DeleteEntity(entity0);
    testBool = testComponentStores.DeleteEntity(entity0);

    unsigned int entity3;
    testBool = testComponentStores.CreateEntity(entity3);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity3, 13, 13, 13);

    TestComponent00* testComponent0 = nullptr;
    testBool = testComponentStores.GetComponent<TestComponent00>(entity1, testComponent0);
    int sum = testComponent0->GetSum();

    testBool = testComponentStores.DeleteAllComponents<TestComponent00>();
    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    testBool = testComponentStores.CreateComponent<TestComponent00>(entity0, 20, 20, 20);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity1, 21, 21, 21);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity2, 22, 22, 22);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity3, 23, 23, 23);

    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    testComponentStores.DeleteAllEntities();

    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    testBool = testComponentStores.CreateEntity(entity0);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity0, 20, 20, 20);
    testBool = testComponentStores.CreateEntity(entity1);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity1, 21, 21, 21);
    testBool = testComponentStores.CreateEntity(entity2);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity2, 22, 22, 22);
    testBool = testComponentStores.CreateEntity(entity3);
    testBool = testComponentStores.CreateComponent<TestComponent00>(entity3, 23, 23, 23);

    testBool = testComponentStores.GetComponent<TestComponent00>(entity0, testComponent0);

    TestComponent00* testComponent1 = nullptr;
    testBool = testComponentStores.GetComponent<TestComponent00>(entity1, testComponent1);

    TestComponent00* testComponent2 = nullptr;

    unsigned int testEntityId0 = (unsigned int)-1;
    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent0, testEntityId0);
    unsigned int testEntityId1 = (unsigned int)-1;
    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent1, testEntityId1);
    unsigned int testEntityId2 = (unsigned int)-1;
    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(testComponent2, testEntityId2);

    TestComponent00* allTestComponents = nullptr;
    size_t count = 0;

    testBool = testComponentStores.GetAllComponents<TestComponent00>(allTestComponents, count);

    testBool = testComponentStores.GetComponentEntityId<TestComponent00>(&allTestComponents[2], testEntityId2);
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

void TestScene::MeshStoresTest() const
{
    bool testBool = false;

    Project001::MeshStores testMeshStores;

    unsigned int model0;
    testBool = testMeshStores.LoadMeshOBJ(model0, "");
    testBool = testMeshStores.LoadMeshOBJ(model0, "../Models/CubeQ.obj");
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
    testBool = testTextureStores.LoadTexture(texture0, "");
    testBool = testTextureStores.LoadTexture(texture0, "../Textures/rgb.png");

    unsigned int texture1;
    testBool = testTextureStores.LoadTexture(texture1, "../Textures/Cube.png");

    unsigned int texture2;
    testBool = testTextureStores.LoadTexture(texture2, "../Textures/CubeSpecular.png");
}