#include "TestScene012.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "Math/CoordinateSystems.h"
#include "Math/VectorUtilities.h"
#include "Application.h"
#include "ComponentStores.h"
#include "FreetypeTextLoader.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"



// public ----------------------------------------------------------------------

TestScene012::TestScene012(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , instructionScene_(applicationPtr)
{}

TestScene012::~TestScene012()
{}

void TestScene012::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene012::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene012::ProcessInitializeEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene012::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene012:            %u", GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    // Calculating positions ---------------------------------------------------

    std::vector<glm::vec3> meshEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            meshEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // -------------------------------------------------------------------------

    // Point 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.01f, 10));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
        collisionPoints.emplace_back();
    }

    // Line 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionLine2D>& collisionLines = collisionBody2DPtr->GetCollisionLines();
        collisionLines.emplace_back();
    }

    // Ray 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(0.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f, true));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionRay2D>& collisionRays = collisionBody2DPtr->GetCollisionRays();
        collisionRays.emplace_back();
    }

    // Ray 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(0.0f, 0.0f);
        shapePoints.emplace_back(0.0f, 100.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f, true));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionRay2D>& collisionRays = collisionBody2DPtr->GetCollisionRays();
        collisionRays.emplace_back(
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f)
        );
    }

    // LineSegment 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.8f);
        shapePoints.emplace_back(0.48f, 0.8f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionLineSegment2D>& collisionLineSegments = collisionBody2DPtr->GetCollisionLineSegments();
        collisionLineSegments.emplace_back(
            glm::vec2(-0.48f, -0.8f),
            glm::vec2(0.48f, 0.8f)
        );
    }

    // Rectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.32f, 0.24f);
        shapePoints.emplace_back(-0.32f, -0.24f);
        shapePoints.emplace_back(0.32f, -0.24f);
        shapePoints.emplace_back(0.32f, 0.24f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
        collisionRectangles.emplace_back(
            glm::vec2(-0.32f, -0.24f),
            glm::vec2(0.32f, 0.24f)
        );
    }

    // OrientedRectangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, 0.32f);
        shapePoints.emplace_back(-0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, -0.32f);
        shapePoints.emplace_back(0.48f, 0.32f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
        Project001::MeshLoader::RotateMeshZ(*newMeshDataPtr, glm::quarter_pi<float>());

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionOrientedRectangle2D>& collisionOrientedRectangles = collisionBody2DPtr->GetCollisionOrientedRectangles();
        collisionOrientedRectangles.emplace_back(
            glm::vec2(0.48f, 0.32f),
            glm::vec2(),
            glm::quarter_pi<float>()
        );
    }

    // Circle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
        collisionCircles.emplace_back(
            glm::vec2(),
            0.32f
        );
    }

    // Capsule 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.80f, 0.24f, 8));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionCapsule2D>& collisionCapsules = collisionBody2DPtr->GetCollisionCapsules();
        collisionCapsules.emplace_back(
            glm::vec2(0.0f, -0.40f),
            glm::vec2(0.0f, 0.40f),
            0.12f
        );
    }

    // Triangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.48f);
        shapePoints.emplace_back(0.48f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionTriangle2D>& collisionTriangles = collisionBody2DPtr->GetCollisionTriangles();
        collisionTriangles.emplace_back(
            glm::vec2(-0.48f, -0.48f),
            glm::vec2(0.48f, 0.0f),
            glm::vec2(0.0f, 0.48f)
        );
    }

    // Polygon 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> loopPoints;
        loopPoints.emplace_back(5.0f, -1.0f);
        loopPoints.emplace_back(4.0f, 0.0f);
        loopPoints.emplace_back(3.0f, -1.0f);
        loopPoints.emplace_back(2.0f, 0.0f);
        loopPoints.emplace_back(3.0f, 0.0f);
        loopPoints.emplace_back(3.0f, 1.0f);
        loopPoints.emplace_back(2.0f, 1.0f);
        loopPoints.emplace_back(2.0f, 0.0f);
        loopPoints.emplace_back(1.0f, 1.0f);
        loopPoints.emplace_back(2.0f, 1.0f);
        loopPoints.emplace_back(2.0f, 2.0f);
        loopPoints.emplace_back(1.0f, 2.0f);
        loopPoints.emplace_back(1.0f, 1.0f);
        loopPoints.emplace_back(0.0f, 1.0f);
        loopPoints.emplace_back(1.0f, 2.0f);
        loopPoints.emplace_back(0.0f, 3.0f);
        loopPoints.emplace_back(-1.0f, 2.0f);
        loopPoints.emplace_back(0.0f, 1.0f);
        loopPoints.emplace_back(-1.0f, 1.0f);
        loopPoints.emplace_back(-1.0f, 2.0f);
        loopPoints.emplace_back(-2.0f, 3.0f);
        loopPoints.emplace_back(-2.0f, 2.0f);
        loopPoints.emplace_back(-1.0f, 1.0f);
        loopPoints.emplace_back(-2.0f, 1.0f);
        loopPoints.emplace_back(-2.0f, 2.0f);
        loopPoints.emplace_back(-3.0f, 2.0f);
        loopPoints.emplace_back(-2.0f, 1.0f);
        loopPoints.emplace_back(-5.0f, 1.0f);
        loopPoints.emplace_back(-3.0f, -0.0f);
        loopPoints.emplace_back(-2.0f, -1.0f);
        loopPoints.emplace_back(-2.0f, -3.0f);
        loopPoints.emplace_back(-1.0f, -2.0f);
        loopPoints.emplace_back(-1.0f, -3.0f);
        loopPoints.emplace_back(0.0f, -2.0f);
        loopPoints.emplace_back(1.0f, -2.0f);
        loopPoints.emplace_back(2.0f, -3.0f);
        loopPoints.emplace_back(2.0f, -2.0f);
        loopPoints.emplace_back(3.0f, -3.0f);
        loopPoints.emplace_back(3.0f, -2.0f);
        loopPoints.emplace_back(4.0f, -1.0f);
        for (size_t i = 0; i < loopPoints.size(); ++i)
        {
            loopPoints[i] *= 0.16f;
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, loopPoints, 0.01f, true));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionPolygon2D>& collisionPolygons = collisionBody2DPtr->GetCollisionPolygons();
        collisionPolygons.emplace_back(loopPoints);
    }

    // Polygon 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> fanPoints;
        fanPoints.emplace_back(1.0f, 0.0f);
        fanPoints.emplace_back(2.0f, 1.0f);
        fanPoints.emplace_back(0.0f, 1.0f);
        fanPoints.emplace_back(-1.0f, 2.0f);
        fanPoints.emplace_back(-1.0f, 0.0f);
        fanPoints.emplace_back(-2.0f, -1.0f);
        fanPoints.emplace_back(0.0f, -1.0f);
        fanPoints.emplace_back(1.0f, -2.0f);
        for (size_t i = 0; i < fanPoints.size(); ++i)
        {
            fanPoints[i] *= 0.08f;
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, fanPoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionPolygon2D>& collisionPolygons = collisionBody2DPtr->GetCollisionPolygons();
        collisionPolygons.emplace_back(fanPoints);
    }

    // Polygon 3
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> loopPoints;
        loopPoints.emplace_back(5.0f, 1.0f);
        loopPoints.emplace_back(5.0f, 2.0f);
        loopPoints.emplace_back(4.0f, 1.0f);
        loopPoints.emplace_back(3.0f, 1.0f);
        loopPoints.emplace_back(3.0f, 3.0f);
        loopPoints.emplace_back(2.0f, 1.0f);
        loopPoints.emplace_back(0.0f, 2.0f);
        loopPoints.emplace_back(2.0f, 0.0f);
        loopPoints.emplace_back(0.0f, 1.0f);
        loopPoints.emplace_back(-1.0f, 1.0f);
        loopPoints.emplace_back(-5.0f, -1.0f);
        loopPoints.emplace_back(-3.0f, -1.0f);
        loopPoints.emplace_back(-2.0f, -2.0f);
        loopPoints.emplace_back(-2.0f, -3.0f);
        loopPoints.emplace_back(-1.0f, -2.0f);
        loopPoints.emplace_back(-1.0f, -3.0f);
        loopPoints.emplace_back(0.0f, -2.0f);
        loopPoints.emplace_back(1.0f, -2.0f);
        loopPoints.emplace_back(2.0f, -3.0f);
        loopPoints.emplace_back(2.0f, -2.0f);
        loopPoints.emplace_back(3.0f, -3.0f);
        loopPoints.emplace_back(3.0f, -1.0f);
        for (size_t i = 0; i < loopPoints.size(); ++i)
        {
            loopPoints[i] *= 0.16f;
        }
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLineLoop(*newMeshDataPtr, loopPoints, 0.01f, true));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionPolygon2D>& collisionPolygons = collisionBody2DPtr->GetCollisionPolygons();
        collisionPolygons.emplace_back(loopPoints);
    }

    // ConvexPolygon 1 (pentagon)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;

        int sides = 5;
        float radius = 0.24f;
        float sectionAngle = 2.0f * glm::pi<float>() / (float)sides;

        glm::vec2 radialVector(0.0f, radius);
        shapePoints.push_back(radialVector);

        for (size_t i = 0; i < sides - 1; ++i)
        {
            radialVector = Project001::Rotate2DVector(radialVector, sectionAngle);
            shapePoints.push_back(radialVector);
        }

        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.emplace_back(shapePoints);
    }

    // ConvexPolygon 2 (hexagon)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;

        int sides = 6;
        float radius = 0.32f;
        float sectionAngle = 2.0f * glm::pi<float>() / (float)sides;

        glm::vec2 radialVector(0.0f, radius);
        shapePoints.push_back(radialVector);

        for (size_t i = 0; i < sides - 1; ++i)
        {
            radialVector = Project001::Rotate2DVector(radialVector, sectionAngle);
            shapePoints.push_back(radialVector);
        }

        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.emplace_back(shapePoints);
    }

    // ConvexPolygon 3 (lineSegment)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> linePoints;
        linePoints.emplace_back(0.32f, 0.32f);
        linePoints.emplace_back(-0.32f, -0.32f);

        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, linePoints, 0.01f, true));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.emplace_back(linePoints);
    }

    // ConvexPolygon 4 (lineSegment)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> linePoints;
        linePoints.emplace_back(0.32f, -0.32f);
        linePoints.emplace_back(-0.32f, 0.32f);

        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, linePoints, 0.01f, true));

        unsigned int tempEntityId;
        componentStoresPtr_->CreateEntity(tempEntityId);
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = meshEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
        renderedMeshPtr->SetPosition(currentPosition);
        renderedMeshPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedMeshPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
        collisionBody2DPtr->SetPosition(currentPosition);
        std::vector<Project001::CollisionConvexPolygon2D>& collisionConvexPolygons = collisionBody2DPtr->GetCollisionConvexPolygons();
        collisionConvexPolygons.emplace_back(linePoints);
    }

    // Member Scenes -----------------------------------------------------------

    Project001::FontData font01_FontData;
    Project001::TextureData font01_TextureData;
    unsigned int font01_TextureId = (unsigned int)-1;
    std::vector<unsigned char> characterList;
    for (unsigned char c = 32; c < 127; ++c) // ASCII characters
    {
        characterList.push_back(c);
    }
    _FAIL_CHECK(Project001::FreetypeTextLoader::LoadTextureDataAndFontData(
        font01_TextureData,
        font01_FontData,
        characterList,
        "../Fonts/Antonio-Regular.ttf",
        48
    ));
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
        "This Scene also tests 2d Shape Overlaps.\n"
        "<Left-Click> on a shape to select it.\n"
        "<Left-Click> and drag to move it.\n"
        "<Left-Click> on the background to de-select it.\n"
        "When no shape is selected:\n"
        "   Use <WASD> to move the camera up, left, down, and right.\n"
        "   Use <Q> to roll camera left and <E> to roll camera right.\n"
        "When a shape is selected:\n"
        "   Use <WASD> to move the shape up, left, down, and right.\n"
        "   Use <Q> to roll shape left and <E> to roll shape right.\n"
        "Press <B> and <N> to cycle between shapes.\n"
        "Use <Scroll> to zoom in and out.\n"
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

void TestScene012::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene012:            %u", GetId());
}