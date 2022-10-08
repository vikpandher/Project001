#include "TestScene012.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/CollisionBody2D.h"
#include "Engine/Components/RenderedModel.h"
#include "Engine/Math/Overlap2D.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"



// public: ---------------------------------------------------------------------

TestScene012::TestScene012()
{}

TestScene012::~TestScene012()
{}

const char* TestScene012::Name()
{
    return "TestScene012";
}

void TestScene012::Initialize()
{
    TestSceneBase002::Initialize();

    // Calculating positions
    // -------------------------------------------------------------------------

    std::vector<glm::vec3> modelEntityPositions;
    for (int i = 2; i >= -2; --i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            modelEntityPositions.emplace_back((float)j, (float)i, 0.0f);
        }
    }
    size_t positionPosition = 0;

    // -------------------------------------------------------------------------

    // point 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.01f, 10));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPoint(Project001::Point2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // line 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-100.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLine(Project001::Line2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // ray 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(0.0f, 0.0f);
        shapePoints.emplace_back(100.0f, 0.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f, true));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddRay(Project001::Ray2D());
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // ray 2
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(0.0f, 0.0f);
        shapePoints.emplace_back(0.0f, 100.0f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f, true));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddRay(Project001::Ray2D(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // lineSegment 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.8f);
        shapePoints.emplace_back(0.48f, 0.8f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, shapePoints, 0.01f));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddLineSegment(Project001::LineSegment2D(
            glm::vec2(-0.48f, -0.8f),
            glm::vec2(0.48f, 0.8f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // rectangle 1
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddRectangle(Project001::Rectangle2D(
            glm::vec2(-0.32f, -0.24f),
            glm::vec2(0.32f, 0.24f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // orientedRectangle 1
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddOrientedRectangle(Project001::OrientedRectangle2D(
            glm::vec2(0.48f, 0.32f),
            glm::vec2(),
            glm::quarter_pi<float>()
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // circle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, 0.32f, 24));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCircle(Project001::Circle2D(
            glm::vec2(),
            0.32f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // capsule 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DCapsule(*newMeshDataPtr, 0.80f, 0.24f, 8));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddCapsule(Project001::Capsule2D(
            glm::vec2(0.0f, -0.40f),
            glm::vec2(0.0f, 0.40f),
            0.12f
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // triangle 1
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> shapePoints;
        shapePoints.emplace_back(-0.48f, -0.48f);
        shapePoints.emplace_back(0.48f, 0.0f);
        shapePoints.emplace_back(0.0f, 0.48f);
        _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddTriangle(Project001::Triangle2D(
            glm::vec2(-0.48f, -0.48f),
            glm::vec2(0.48f, 0.0f),
            glm::vec2(0.0f, 0.48f)
        ));
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // polygon 1
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPolygon(loopPoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // polygon 2
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPolygon(fanPoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // polygon 3
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddPolygon(loopPoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // convexPolygon 1 (pentagon)
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddConvexPolygon(shapePoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // convexPolygon 2 (hexagon)
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
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddConvexPolygon(shapePoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // convexPolygon 3 (lineSegment)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> linePoints;
        linePoints.emplace_back(0.32f, 0.32f);
        linePoints.emplace_back(-0.32f, -0.32f);

        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, linePoints, 0.01f, true));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddConvexPolygon(linePoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }

    // convexPolygon 4 (lineSegment)
    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        std::vector<glm::vec2> linePoints;
        linePoints.emplace_back(0.32f, -0.32f);
        linePoints.emplace_back(-0.32f, 0.32f);

        _FAIL_CHECK(Project001::MeshLoader::Generate2DLine(*newMeshDataPtr, linePoints, 0.01f, true));

        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        glm::vec3 currentPosition = modelEntityPositions[positionPosition++];

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(tempEntityId));
        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(tempEntityId, renderedModelPtr));
        renderedModelPtr->SetPosition(currentPosition);
        renderedModelPtr->SetMeshDataPtr(newMeshDataPtr);
        renderedModelPtr->SetLit(false);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId));
        Project001::CollisionBody2D* collisionBody2DPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DPtr));
        collisionBody2DPtr->SetPosition(currentPosition);
        collisionBody2DPtr->AddConvexPolygon(linePoints);
        collisionBody2DPtr->CalculateBoundingRadius();
    }
}

void TestScene012::Deinitialize()
{
    TestSceneBase002::Deinitialize();
}

void TestScene012::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene012::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase002::OnEvent(event);
}

// protected: ------------------------------------------------------------------

void TestScene012::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene013"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene013"));
            }
        }
    }
}