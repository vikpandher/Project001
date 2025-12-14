// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#include "TestScene009.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedModel.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene009::TestScene009(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
    , gradiant_TextureId_(static_cast<unsigned int>(-1))
    , numbers12x6_TextureId_(static_cast<unsigned int>(-1))
{
    GetSharedDataPtr<TestApplicationData>()->testScene009Id = GetId();
}

TestScene009::~TestScene009()
{}

void TestScene009::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene009::ProcessDeinitializeEvent, this, std::placeholders::_1));

    for (size_t i = 0; i < animatedSpriteSceneArray_.size(); ++i)
    {
        animatedSpriteSceneArray_[i]->HandleEvent(event);
    }

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene009::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene009::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene009:            " << GetId());

    // Texture Data ------------------------------------------------------------

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/32x32_gradiant.png"));
        GetRendererPtr()->CreateTexture(gradiant_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    {
        Project001::TextureData textureData;
        FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/12_6_numbers.png"));
        GetRendererPtr()->CreateTexture(numbers12x6_TextureId_, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel, false, false);
    }

    // Move Main Camera Entity
    // -------------------------------------------------------------------------
    {
        Project001::Camera* cameraPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::Camera>(cameraPtr, mainCameraEntityId_));
        if (cameraPtr)
        {
            cameraPtr->SetPosition(0.0f, 0.0f, 15.0f);
        }
    }

    // Creating Entities
    // -------------------------------------------------------------------------

    Create2DStarMeshes_1();

    Create2DStarMeshes_2();

    CreateCrownMeshes_1();

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    FAIL_CHECK(Project001::FontLoader::LoadFontDataFromMemory(
        font01_FontData,
        g_AntonioRegular_ssf,
        sizeof(g_AntonioRegular_ssf)
    ));

    Project001::TextureData font01_TextureData;
    FAIL_CHECK(Project001::TextureLoader::LoadTextureFromMemory(
        font01_TextureData,
        g_AntonioRegular_png,
        sizeof(g_AntonioRegular_png)
    ));
    unsigned int font01_TextureId = static_cast<unsigned int>(-1);
    GetRendererPtr()->CreateTexture(
        font01_TextureId,
        font01_TextureData.data,
        font01_TextureData.width,
        font01_TextureData.height,
        font01_TextureData.bytesPerPixel,
        true,
        false
    );

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests Mesh Generation.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureId = font01_TextureId;
    instructionSceneInfo.cameraEntityId = uiCameraEntityId_;
    instructionSceneInfo.cameraMask = s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructions = keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene009::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene009:            " << GetId());

    gradiant_TextureId_ = static_cast<unsigned int>(-1);
    numbers12x6_TextureId_ = static_cast<unsigned int>(-1);

    for (size_t i = 0; i < animatedMeshDataPtrArrayArray_.size(); ++i)
    {
        for (size_t j = 0; j < animatedMeshDataPtrArrayArray_[i].size(); ++j)
        {
            delete animatedMeshDataPtrArrayArray_[i][j];
        }
    }
    animatedMeshDataPtrArrayArray_.clear();
    animatedMeshFrameDurationArrayArray_.clear();

    for (size_t i = 0; i < animatedSpriteSceneArray_.size(); ++i)
    {
        animatedSpriteSceneArray_[i]->Deinitialize();
        delete animatedSpriteSceneArray_[i];
    }
    animatedSpriteSceneArray_.clear();
}

void TestScene009::Create2DStarMeshes_1()
{
    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;

    // positions
    for (int j = 9; j >= 4; --j)
    {
        for (int i = -4; i <= 4; ++i)
        {
            meshEntityPositions.emplace_back(static_cast<float>(i), static_cast<float>(j) - 0.5f, 0.0f);
        }
    }

    size_t positionIndex = 0;

    glm::vec4 color0(1.0f, 1.0f, 1.0f, 0.5f);

    // row 1 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarRing(
            *newMeshDataPtr, i,
            1.0f, 0.5f, 0.75f, 0.25f,
            0,
            0.0f, 0.0f, 0.0f, 0.0f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 2 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarRing(
            *newMeshDataPtr, i,
            1.0f, 0.5f, 0.75f, 0.25f,
            0,
            0.25f, 0.125f, 0.1875f, 0.0625f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 3 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarRing(
            *newMeshDataPtr, i,
            1.0f, 0.5f, 0.75f, 0.25f,
            0,
            0.5f, 0.25f, 0.375f, 0.125f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 4 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarBurst(
            *newMeshDataPtr, i,
            0.5f, 0.25f, 1.0f, 0.5f,
            glm::pi<float>() * 0.25f,
            0,
            0.0f, 0.0f, 0.0f, 0.0f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 5 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarBurst(
            *newMeshDataPtr, i,
            0.5f, 0.25f, 1.0f, 0.5f,
            glm::pi<float>() * 0.25f,
            0,
            0.125f, 0.0625f, 0.25f, 0.125f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 6 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarBurst(
            *newMeshDataPtr, i,
            0.5f, 0.25f, 1.0f, 0.5f,
            glm::pi<float>() * 0.25f,
            0,
            0.25f, 0.125f, 0.5f, 0.25f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }
}

void TestScene009::Create2DStarMeshes_2()
{
    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;

    // positions
    for (int j = 3; j >= -2; --j)
    {
        for (int i = -4; i <= 4; ++i)
        {
            meshEntityPositions.emplace_back(static_cast<float>(i), static_cast<float>(j) - 0.5f, 0.0f);
        }
    }

    size_t positionIndex = 0;

    glm::vec4 color0(1.0f, 1.0f, 1.0f, 1.0f);
    unsigned long long frameDuration0 = 100000000;

    animatedMeshDataPtrArrayArray_.resize(9);
    animatedMeshFrameDurationArrayArray_.resize(9);

    // row 1 -------------------------------------------------------------------
    for (size_t i = 0; i < 9; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        animatedMeshDataPtrArrayArray_[i].push_back(newMeshDataPtr);
        animatedMeshFrameDurationArrayArray_[i].emplace_back(frameDuration0);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarRing(
            *newMeshDataPtr, 8,
            0.3f, 0.0f, 0.2f, 0.0f,
            i,
            0.2f, 0.0f, 0.1f, 0.0f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 2 -------------------------------------------------------------------
    for (size_t i = 0; i < 9; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        animatedMeshDataPtrArrayArray_[i].push_back(newMeshDataPtr);
        animatedMeshFrameDurationArrayArray_[i].emplace_back(frameDuration0);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarRing(
            *newMeshDataPtr, 10,
            0.6f, 0.2f, 0.4f, 0.2f,
            i,
            0.4f, 0.1f, 0.3f, 0.1f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 3 -------------------------------------------------------------------
    for (size_t i = 0; i < 9; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        animatedMeshDataPtrArrayArray_[i].push_back(newMeshDataPtr);
        animatedMeshFrameDurationArrayArray_[i].emplace_back(frameDuration0);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarRing(
            *newMeshDataPtr, 10,
            0.8f, 0.4f, 0.7f, 0.3f,
            i,
            0.2f, 0.1f, 0.3f, 0.1f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 4 -------------------------------------------------------------------
    for (size_t i = 0; i < 9; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        animatedMeshDataPtrArrayArray_[i].push_back(newMeshDataPtr);
        animatedMeshFrameDurationArrayArray_[i].emplace_back(frameDuration0);
        FAIL_CHECK(Project001::MeshLoader::Generate2DStarBurst(
            *newMeshDataPtr, 10,
            0.8f, 0.5f, 1.0f, 0.8f,
            glm::pi<float>() * 0.1f,
            i,
            0.4f, 0.1f, 0.4f, 0.4f
        ));
        Project001::MeshLoader::ApplyPositionalTextureCoordinates(*newMeshDataPtr);
        Project001::MeshLoader::TranslateTextureCoordinates(*newMeshDataPtr, glm::vec2(0.5f, 0.5f));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(gradiant_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 5 -------------------------------------------------------------------
    for (size_t i = 0; i < 9; ++i)
    {
        TestAnimatedSpriteScene001::InitializationInfo initializationInfo = {};
        initializationInfo.frameCount = animatedMeshDataPtrArrayArray_[i].size();
        initializationInfo.frameMeshDataPtrsPtr = animatedMeshDataPtrArrayArray_[i].data();
        initializationInfo.frameDurations_ns_ptr = animatedMeshFrameDurationArrayArray_[i].data();
        initializationInfo.cameraMaskPtr = &s_mainCameraMask_;
        initializationInfo.positionPtr = &meshEntityPositions[positionIndex++];
        initializationInfo.textureIdPtr = &gradiant_TextureId_;

        animatedSpriteSceneArray_.emplace_back(new TestAnimatedSpriteScene001(GetApplicationPtr()));
        animatedSpriteSceneArray_.back()->Initialize(initializationInfo);
    }
}

void TestScene009::CreateCrownMeshes_1()
{
    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;

    // positions
    for (int j = 3; j >= -2; --j)
    {
        for (int i = 5; i <= 13; ++i)
        {
            meshEntityPositions.emplace_back(static_cast<float>(i), static_cast<float>(j) - 0.5f, 0.0f);
        }
    }

    size_t positionIndex = 0;

    glm::vec4 color0(1.0f, 1.0f, 1.0f, 1.0f);

    // row 1 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSpikeyCrown(
            *newMeshDataPtr,
            0.5f,
            1.0f,
            i,
            1.0f, 0.0f, 0.75f, -0.25f,
            0,
            0.0f, 0.0f, 0.0f, 0.0f
        ));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 2 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSpikeyCrown(
            *newMeshDataPtr,
            0.5f,
            1.0f,
            i,
            1.0f, 0.0f, 0.75f, -0.25f,
            0,
            0.25f, -0.125f, 0.25f, -0.125f
        ));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 3 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSpikeyCrown(
            *newMeshDataPtr,
            0.5f,
            1.0f,
            i,
            1.0f, 0.0f, 0.75f, -0.25f,
            0,
            0.5f, -0.25f, 0.5f, -0.25f
        ));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 4 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSpikeyCrown(
            *newMeshDataPtr,
            0.5f,
            1.0f,
            i,
            1.0f, 0.0f, 0.75f, -0.25f,
            0,
            0.0f, 0.0f, 0.0f, 0.0f,
            false
        ));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 5 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSpikeyCrown(
            *newMeshDataPtr,
            0.5f,
            1.0f,
            i,
            1.0f, 0.0f, 0.75f, -0.25f,
            0,
            0.25f, -0.125f, 0.25f, -0.125f,
            false
        ));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // row 6 -------------------------------------------------------------------
    for (size_t i = 2; i < 11; ++i)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateSpikeyCrown(
            *newMeshDataPtr,
            0.5f,
            1.0f,
            i,
            1.0f, 0.0f, 0.75f, -0.25f,
            0,
            0.5f, -0.25f, 0.5f, -0.25f,
            false
        ));
        Project001::MeshLoader::ScaleMesh(*newMeshDataPtr, glm::vec3(0.32f));

        unsigned int tempEntityId;
        GetComponentStoresPtr()->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetCameraMask(s_mainCameraMask_);
            renderedMeshPtr->SetPosition(meshEntityPositions[positionIndex++]);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetTextureId(numbers12x6_TextureId_);
            renderedMeshPtr->SetColor(color0);
            renderedMeshPtr->SetTranslucent(true);
        }
    }
}