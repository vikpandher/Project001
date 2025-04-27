// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-04-26

#include "TestScene023.h"

#include "TestApplicationData.h"
#include "TestResource_AntonioRegular_png.h"
#include "TestResource_AntonioRegular_ssf.h"

#include "Components/Camera.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap3D_UnitTests.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FontLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include "Window.h"



// public ----------------------------------------------------------------------

TestScene023::TestScene023(Project001::Application* applicationPtr)
    : TestSceneBase001(applicationPtr)
    , instructionScene_(applicationPtr)
{
    if (testApplicationDataPtr_ != nullptr)
    {
        testApplicationDataPtr_->testScene023Id = GetId();
    }

    Run_UnitTests();
}

TestScene023::~TestScene023()
{}

void TestScene023::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene023::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase001::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene023::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene023::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    LOG_INFO("INITIALIZING:   TestScene023:            " << GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    // OBB Box
    if (true)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, glm::vec3(-0.1f, -0.2f, -0.3f), glm::vec3(0.1f, 0.2f, 0.3f)));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());
        Project001::MeshLoader::RotateMeshY(*newMeshDataPtr, glm::half_pi<float>() +  glm::quarter_pi<float>());
        Project001::MeshLoader::TranslateMesh(*newMeshDataPtr, glm::vec3(0.3f, 0.2f, 0.1f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetColor(0.6f, 0.6f, 0.6f, 0.6f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // Line
    if (false)
    {
        glm::vec3 position00(0.1f, 0.2f, 0.3f);
        glm::vec3 direction00(-1.0f, 0.0f, 0.0f);

        glm::quat rotation1 = glm::rotate(
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quarter_pi<float>(),
            glm::vec3(0.0f, 0.0f, 1.0f));
        glm::quat rotation2 = glm::rotate(
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::half_pi<float>() + glm::quarter_pi<float>(),
            glm::vec3(0.0f, 1.0f, 0.0f));

        position00 = rotation2 * rotation1 * position00;
        direction00 = rotation2 * rotation1 * direction00;

        position00 += glm::vec3(0.3f, 0.2f, 0.1f);

        position00 = glm::vec3(0.0f, 0.4f, 0.0f);
        direction00 = glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f));

        // shape 2
        {
            Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
            meshDataPtrArray_.push_back(newMeshDataPtr);
            FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, position00, direction00 * 100.0f, 0.01f, 6));

            unsigned int tempEntityId;
            componentStoresPtr_->CreateEntity(tempEntityId);
            entityIds_.push_back(tempEntityId);

            FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetColorRGB(0.4f, 0.8f, 0.4f);
                renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
                renderedMeshPtr->SetLit(false);
            }
        }

        // shape 2
        {
            Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
            meshDataPtrArray_.push_back(newMeshDataPtr);
            FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, position00, direction00 * -100.0f, 0.01f, 6));

            unsigned int tempEntityId;
            componentStoresPtr_->CreateEntity(tempEntityId);
            entityIds_.push_back(tempEntityId);

            FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetColorRGB(0.8f, 0.4f, 0.4f);
                renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
                renderedMeshPtr->SetLit(false);
            }
        }
    }

    // LineSegment
    if (false)
    {
        glm::vec3 position00(-0.8f, 0.2f, 0.3f);
        glm::vec3 position01(0.8f, 0.2f, 0.3f);

        glm::quat rotation1 = glm::rotate(
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::quarter_pi<float>(),
            glm::vec3(0.0f, 0.0f, 1.0f));
        glm::quat rotation2 = glm::rotate(
            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            glm::half_pi<float>() + glm::quarter_pi<float>(),
            glm::vec3(0.0f, 1.0f, 0.0f));

        position00 = rotation2 * rotation1 * position00;
        position01 = rotation2 * rotation1 * position01;

        position00 += glm::vec3(0.3f, 0.2f, 0.1f);
        position01 += glm::vec3(0.3f, 0.2f, 0.1f);

        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, position00, position01, 0.01f, 6));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetColorRGB(0.4f, 0.4f, 0.8f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Box 2
    if (false)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, glm::vec3(-0.1f, -0.2f, -0.3f), glm::vec3(0.1f, 0.2f, 0.3f)));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetColor(0.6f, 0.6f, 0.6f, 0.6f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
            renderedMeshPtr->SetTranslucent(true);
        }
    }

    // Triangle
    if (false)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec3> positions;
        positions.emplace_back(-0.2f, -0.1f, 0.0f);
        positions.emplace_back(-0.3f, 0.0f, 0.0f);
        positions.emplace_back(-0.1f, 0.0f, 0.0f);
        FAIL_CHECK(Project001::MeshLoader::GenerateTriangles(*newMeshDataPtr, positions));
        // Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());
        // Project001::MeshLoader::RotateMeshY(*newMeshDataPtr, glm::half_pi<float>() + glm::quarter_pi<float>());
        // Project001::MeshLoader::TranslateMesh(*newMeshDataPtr, glm::vec3(0.3f, 0.2f, 0.1f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetColorRGB(0.8f, 0.4f, 0.4f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Triangle 2
    if (false)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec3> positions;
        positions.emplace_back(0.2f, 0.1f, 0.0f);
        positions.emplace_back(-0.3f, 0.0f, 0.0f);
        positions.emplace_back(0.1f, 0.0f, 0.0f);
        FAIL_CHECK(Project001::MeshLoader::GenerateTriangles(*newMeshDataPtr, positions));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetColorRGB(0.4f, 0.8f, 0.4f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }
    }

    // Point
    if (false)
    {

        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec3> positions;
        positions.emplace_back(1.0f, 0.0f, 0.0f);
        positions.emplace_back(0.0f, 1.0f, 0.0f);
        positions.emplace_back(0.0f, 0.0f, 1.0f);
        FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.02f, 6, 6));
        Project001::MeshLoader::TranslateMesh(*newMeshDataPtr,glm::vec3(1.0f, 1.0f, 1.0f) / 3.0f);

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetColorRGB(0.8f, 0.8f, 0.8f);
            renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedMeshPtr->SetLit(false);
        }
    }

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
    unsigned int font01_TextureId = (unsigned int)-1;
    rendererPtr_->CreateTexture(
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
        "This Scene runs unit tests for 3d Shape Overlaps.\n"
        "Use <WASD> to move the camera up, left, down, and right.\n"
        "Use <Q> to roll left and <E> to roll right.\n"
        "Use <Scroll> to move forward and back.\n"
        "<Left-Click> and drag the <Mouse> to move camera.\n"
        "Press <Esc> to return to Main Menu.\n"
        "Press <Tab> to hide instructions."
    );
    instructionSceneInfo.fontDataPtr = &font01_FontData;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene023::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    LOG_INFO("DEINITIALIZING: TestScene023:            " << GetId());
}

// private ---------------------------------------------------------------------

void TestScene023::Run_UnitTests() const
{
    int result;

    result = Project001::UnitTest_Check3D_Point_Point_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Point_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_Line_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Line_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_Ray_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Ray_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_LineSegment_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_LineSegment_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_Plane_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Plane_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_Triangle_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Triangle_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Sphere_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Point_Capsule_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Point_Capsule_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Line_Line_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_Line_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_Ray_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_Ray_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_LineSegment_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_LineSegment_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_Plane_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_Plane_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_Triangle_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_Triangle_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Line_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Line_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Ray_Ray_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_Ray_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Ray_LineSegment_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_LineSegment_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Ray_Plane_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_Plane_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Ray_Triangle_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_Triangle_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Ray_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Ray_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Ray_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Ray_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_LineSegment_LineSegment_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_LineSegment_LineSegment_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Plane_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_LineSegment_Plane_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Triangle_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_LineSegment_Triangle_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_LineSegment_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_LineSegment_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_LineSegment_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Plane_Plane_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Plane_Plane_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Plane_Triangle_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Plane_Triangle_Ovelap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Plane_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Plane_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Plane_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Plane_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Plane_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Plane_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Triangle_Triangle_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Triangle_Triangle_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Triangle_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Triangle_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Triangle_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Triangle_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_Triangle_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Triangle_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_AABB_AABB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_AABB_AABB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_AABB_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_AABB_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_AABB_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_AABB_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_OBB_OBB_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_OBB_OBB_Overlap FAILED " << result);
    }

    result = Project001::UnitTest_Check3D_OBB_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_OBB_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Sphere_Sphere_Overlap();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Check3D_Sphere_Sphere_Overlap FAILED " << result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Get3D_Point_Line_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_Line_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_Ray_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_Ray_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_LineSegment_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_LineSegment_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_Plane_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_Plane_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_Triangle_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_Triangle_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_AABB_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_AABB_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_OBB_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_OBB_ClosestPoint FAILED " << result);
    }

    result = Project001::UnitTest_Get3D_Point_Sphere_ClosestPoint();
    if (result != 0)
    {
        LOG_INFO("UnitTest_Get3D_Point_Sphere_ClosestPoint FAILED " << result);
    }
}