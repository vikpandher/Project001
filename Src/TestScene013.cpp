#include "TestScene013.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap3D_UnitTests.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene013::TestScene013()
{
    ClearResources();

    Run_UnitTests();
}

TestScene013::~TestScene013()
{}

const char* TestScene013::Name()
{
    return "TestScene013";
}

void TestScene013::Initialize()
{
    TestSceneBase001::Initialize();

    // Calculating positions
    // -------------------------------------------------------------------------

    // std::vector<glm::vec3> modelEntityPositions;
    // for (int i = 1; i >= -1; --i)
    // {
    //     for (int j = -2; j <= 2; ++j)
    //     {
    //         modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
    //     }
    // }
    // size_t positionPosition = 0;

    // -------------------------------------------------------------------------

    // OBB Box
    if (true)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, glm::vec3(-0.1f, -0.2f, -0.3f), glm::vec3(0.1f, 0.2f, 0.3f)));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());
        Project001::MeshLoader::RotateMeshY(*newMeshDataPtr, glm::half_pi<float>() +  glm::quarter_pi<float>());
        Project001::MeshLoader::TranslateMesh(*newMeshDataPtr, glm::vec3(0.3f, 0.2f, 0.1f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetColor(0.6f, 0.6f, 0.6f, 0.6f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
        renderedModelPtr->SetTranslucent(true);
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
            _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, position00, direction00 * 100.0f, 0.01f, 6));

            unsigned int tempEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
            entityIds_.push_back(tempEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
            renderedModelPtr->SetColorRGB(0.4f, 0.8f, 0.4f);
            renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedModelPtr->SetLit(false);
        }

        // shape 2
        {
            Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
            meshDataPtrArray_.push_back(newMeshDataPtr);
            _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, position00, direction00 * -100.0f, 0.01f, 6));

            unsigned int tempEntityId;
            _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
            entityIds_.push_back(tempEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
            Project001::RenderedModel* renderedModelPtr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
            renderedModelPtr->SetColorRGB(0.8f, 0.4f, 0.4f);
            renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
            renderedModelPtr->SetLit(false);
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
        _FAIL_CHECK(Project001::MeshLoader::GenerateTube(*newMeshDataPtr, position00, position01, 0.01f, 6));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetColorRGB(0.4f, 0.4f, 0.8f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
    }

    // Box 2
    if (false)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateBox(*newMeshDataPtr, glm::vec3(-0.1f, -0.2f, -0.3f), glm::vec3(0.1f, 0.2f, 0.3f)));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetColor(0.6f, 0.6f, 0.6f, 0.6f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
        renderedModelPtr->SetTranslucent(true);
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
        _FAIL_CHECK(Project001::MeshLoader::GenerateTriangles(*newMeshDataPtr, positions));
        // Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());
        // Project001::MeshLoader::RotateMeshY(*newMeshDataPtr, glm::half_pi<float>() + glm::quarter_pi<float>());
        // Project001::MeshLoader::TranslateMesh(*newMeshDataPtr, glm::vec3(0.3f, 0.2f, 0.1f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetColorRGB(0.8f, 0.4f, 0.4f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
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
        _FAIL_CHECK(Project001::MeshLoader::GenerateTriangles(*newMeshDataPtr, positions));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetColorRGB(0.4f, 0.8f, 0.4f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
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
        _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(*newMeshDataPtr, 0.02f, 6, 6));
        Project001::MeshLoader::TranslateMesh(*newMeshDataPtr,glm::vec3(1.0f, 1.0f, 1.0f) / 3.0f);

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetColorRGB(0.8f, 0.8f, 0.8f);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);
    }
}

void TestScene013::Deinitialize()
{
    componentStoresPtr_->DeleteAllEntities();
    rendererPtr_->DeleteAllTextures();

    ClearResources();
}

void TestScene013::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene013::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene013::ClearResources()
{}

void TestScene013::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene020"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene020"));
            }
        }
    }
}

// private: ------------------------------------------------------------------

void TestScene013::Run_UnitTests() const
{
    int result;

    result = Project001::UnitTest_Check3D_Point_Point_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Point_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Line_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Line_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Ray_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Ray_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Sphere_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Point_Capsule_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Point_Capsule_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Line_Line_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Line_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Ray_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Ray_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Line_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Line_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Ray_Ray_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Ray_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Ray_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Ray_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_LineSegment_LineSegment_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_LineSegment_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_LineSegment_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_LineSegment_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Plane_Plane_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_Plane_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Plane_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_Triangle_Ovelap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Plane_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Plane_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Plane_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Plane_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Triangle_Triangle_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Triangle_Triangle_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Triangle_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Triangle_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Triangle_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Triangle_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_Triangle_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Triangle_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_AABB_AABB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_AABB_AABB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_AABB_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_AABB_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_AABB_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_AABB_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_OBB_OBB_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_OBB_OBB_Overlap FAILED (%d)", result);
    }

    result = Project001::UnitTest_Check3D_OBB_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_OBB_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Check3D_Sphere_Sphere_Overlap();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Check3D_Sphere_Sphere_Overlap FAILED (%d)", result);
    }

    // -------------------------------------------------------------------------

    result = Project001::UnitTest_Get3D_Point_Line_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Line_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Ray_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Ray_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_LineSegment_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_LineSegment_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Plane_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Plane_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Triangle_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Triangle_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_AABB_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_AABB_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_OBB_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_OBB_ClosestPoint FAILED (%d)", result);
    }

    result = Project001::UnitTest_Get3D_Point_Sphere_ClosestPoint();
    if (result != 0)
    {
        _LOG_MESSAGE("UnitTest_Get3D_Point_Sphere_ClosestPoint FAILED (%d)", result);
    }
}