#include "TestScene013.h"

#include "Components/Camera.h"
#include "Components/CollisionBody2D.h"
#include "Components/RenderedMesh.h"
#include "Math/Overlap2D.h"
#include "Application.h"
#include "CollisionBodyQuadTree2D.h"
#include "ComponentStores.h"
#include "Logger.h"
#include "MeshLoader.h"
#include "Renderer.h"

#include <random>



// public ----------------------------------------------------------------------

TestScene013::TestScene013(Project001::Application* applicationPtr)
    : TestSceneBase002(applicationPtr)
    , rectangleMeshId_((unsigned int)-1)
    , circleMeshId_((unsigned int)-1)
    , instructionScene_(applicationPtr)
{
    generateCollisionMarkerCollectionMesh_ = false;
    generateEnergyTextMesh_ = false;

    TestCollisionBodyQuadTree2D();
}

TestScene013::~TestScene013()
{}

void TestScene013::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::DeinitializeEvent>(event, std::bind(&TestScene013::ProcessDeinitializeEvent, this, std::placeholders::_1));

    TestSceneBase002::HandleEvent(event);

    Project001::DispatchEvent<Project001::InitializeEvent>(event, std::bind(&TestScene013::ProcessInitializeEvent, this, std::placeholders::_1));

    Project001::DispatchEvent<Project001::RenderEvent>(event, std::bind(&TestScene013::ProcessRenderEvent, this, std::placeholders::_1));

    instructionScene_.HandleEvent(event);
}

// protected -------------------------------------------------------------------

void TestScene013::ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent)
{
    _LOG_MESSAGE("INITIALIZING:   TestScene013:            %u", GetId());

    // Creating Entities
    // -------------------------------------------------------------------------

    // std::random_device randomDevice;
    // std::mt19937 randomNumberEngine(randomDevice());
    std::mt19937 randomNumberEngine(777);
    std::uniform_real_distribution<float> distributionX(-8.0f, 8.0f);
    std::uniform_real_distribution<float> distributionY(-6.0f, 6.0f);

    Project001::CollisionBody2DCreationInfo collisionBody2DCreationInfo;
    collisionBody2DCreationInfo.physicsType = Project001::CollisionBody2D::PhysicsType::PHYSICS_TYPE_OVERLAP_ONLY;
    collisionBody2DCreationInfo.collisionGroupMask = mainCollisionGroupMask_;

    // Rectangles
    // -------------------------------------------------------------------------
    if (false)
    {
        glm::vec2 rectangleMin(-0.32f, -0.24f);
        glm::vec2 rectangleMax(0.32f, 0.24f);
        float rectangleBoundingRadius;
        {
            Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
            meshDataPtrArray_.push_back(newMeshDataPtr);
            std::vector<glm::vec2> shapePoints;
            shapePoints.emplace_back(-0.32f, 0.24f);
            shapePoints.emplace_back(-0.32f, -0.24f);
            shapePoints.emplace_back(0.32f, -0.24f);
            shapePoints.emplace_back(0.32f, 0.24f);
            _FAIL_CHECK(Project001::MeshLoader::Generate2DTriangleFan(*newMeshDataPtr, shapePoints));
            rendererPtr_->CreateMesh(
                rectangleMeshId_,
                newMeshDataPtr->meshVertexArray.data(),
                (unsigned int)newMeshDataPtr->meshVertexArray.size(),
                newMeshDataPtr->meshIndexArray.data(),
                (unsigned int)newMeshDataPtr->meshIndexArray.size()
            );
            rectangleBoundingRadius = newMeshDataPtr->maxBoundingRadius;
        }

        size_t count;
#ifndef NDEBUG
        count = 500;
#else
        count = 1500;
#endif
        for (size_t i = 0; i < count; ++i)
        {
            glm::vec3 currentPosition(distributionX(randomNumberEngine), distributionY(randomNumberEngine), 0.0f);

            unsigned int tempEntityId;
            componentStoresPtr_->CreateEntity(tempEntityId);
            entityIds_.push_back(tempEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetPosition(currentPosition);
                renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(rectangleMeshId_, rectangleBoundingRadius);
                renderedMeshPtr->SetLit(false);
            }

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
            if (collisionBody2DPtr != nullptr)
            {
                collisionBody2DPtr->SetPosition(currentPosition);
                std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
                collisionRectangles.emplace_back(
                    glm::vec2(-0.32f, -0.24f),
                    glm::vec2(0.32f, 0.24f)
                );
            }
        }
    }

    // Circles
    // -------------------------------------------------------------------------
    if (true)
    {
        float circleRadius = 0.08f;
        float circleBoundingRadius;
        {
            Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
            meshDataPtrArray_.push_back(newMeshDataPtr);
            _FAIL_CHECK(Project001::MeshLoader::Generate2DRegularPolygon(*newMeshDataPtr, circleRadius, 12));
            rendererPtr_->CreateMesh(
                circleMeshId_,
                newMeshDataPtr->meshVertexArray.data(),
                (unsigned int)newMeshDataPtr->meshVertexArray.size(),
                newMeshDataPtr->meshIndexArray.data(),
                (unsigned int)newMeshDataPtr->meshIndexArray.size()
            );
            circleBoundingRadius = newMeshDataPtr->maxBoundingRadius;
        }

        size_t count;
#ifndef NDEBUG
        count = 1000;
#else
        count = 5000;
#endif
        for (size_t i = 0; i < count; ++i)
        {
            glm::vec3 currentPosition(distributionX(randomNumberEngine), distributionY(randomNumberEngine), 0.0f);

            unsigned int tempEntityId;
            componentStoresPtr_->CreateEntity(tempEntityId);
            entityIds_.push_back(tempEntityId);

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
            Project001::RenderedMesh* renderedMeshPtr = nullptr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, tempEntityId));
            if (renderedMeshPtr != nullptr)
            {
                renderedMeshPtr->SetPosition(currentPosition);
                renderedMeshPtr->SetMeshIdAndMaxBoundingRadius(circleMeshId_, circleBoundingRadius);
                renderedMeshPtr->SetLit(false);
            }

            _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::CollisionBody2D>(tempEntityId, collisionBody2DCreationInfo));
            Project001::CollisionBody2D* collisionBody2DPtr = nullptr;
            _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::CollisionBody2D>(collisionBody2DPtr, tempEntityId));
            if (collisionBody2DPtr != nullptr)
            {
                collisionBody2DPtr->SetPosition(currentPosition);
                std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
                collisionCircles.emplace_back(
                    glm::vec2(),
                    circleRadius
                );
                // Testing removing in-group collision
                // collisionBody2DPtr->SetCollisionGroupMask(collisionBody2DPtr->GetCollisionGroupMask() << 1);
                // collisionBody2DPtr->SetAllowedCollisionFilterMask(~collisionBody2DPtr->GetCollisionGroupMask());
            }
        }
    }

    float uiCameraHalfHeight = 0.0f;
    float uiCameraHalfWidth = 0.0f;

    {
        Project001::Camera* cameraPtr = nullptr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(cameraPtr, uiCameraEntityId_));
        if (cameraPtr != nullptr)
        {
            uiCameraHalfHeight = cameraPtr->GetTopCutoff();
            uiCameraHalfWidth = cameraPtr->GetRightCutoff();
        }
    }

    // Member Scenes -----------------------------------------------------------

    const Project001::KeyCode keyCode_toggleInstructions = Project001::KeyCode::KEY_CODE_TAB;

    TestInstructionScene001::InitializationInfo instructionSceneInfo;
    instructionSceneInfo.hiddenInstructionString = std::string("Press <Tab> to show instructions.");
    instructionSceneInfo.instructionString = std::string(
        "This Scene tests overlap for many 2d Shapes at once.\n"
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
    instructionSceneInfo.fontDataPtr = font01_FontDataPtr_;
    instructionSceneInfo.fontTextureIdPtr = &font01_TextureId_;
    instructionSceneInfo.cameraEntityIdPtr = &uiCameraEntityId_;
    instructionSceneInfo.cameraMaskPtr = &s_uiCameraMask_;
    instructionSceneInfo.keyCode_toggleInstructionsPtr = &keyCode_toggleInstructions;
    instructionScene_.Initialize(instructionSceneInfo);
}

void TestScene013::ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent)
{
    instructionScene_.Deinitialize();

    _LOG_MESSAGE("DEINITIALIZING: TestScene013:            %u", GetId());

    // Mesh Data ---------------------------------------------------------------

    // dont need to delete these here since they are added to meshDataPtrArray_

    rectangleMeshId_ = (unsigned int)-1;

    circleMeshId_ = (unsigned int)-1;
}

// private ---------------------------------------------------------------------

void TestScene013::TestCollisionBodyQuadTree2D()
{
    Project001::CollisionBodyQuadTree2D collisionBodyQuadTree2D(glm::vec2(-1.6f, -0.8f), glm::vec2(1.6f, 0.8f), 3, 4);

    std::vector<Project001::CollisionBody2D*> collisionBodyPtrs;

    std::vector<glm::vec3> positions;

    // -------------------------------------------------------------------------
    // Adding rectangles and seeing how they populate the quad tree.

    positions.emplace_back(0.0f, -1.1f, 0.0f);  // [00] Goes out of bounds
    positions.emplace_back(0.0f, -1.0f, 0.0f);  // [01] Goes in because radius ~= 0.224
    positions.emplace_back(0.8f, 1.6f, 0.0f);   // [02] Goes out of bounds
    positions.emplace_back(1.6f, 0.8f, 0.0f);   // [03] Goes in
    positions.emplace_back(-0.8f, -0.4f, 0.0f); // [04] Goes in
    positions.emplace_back(0.2f, 0.1f, 0.0f);   // [05] Goes in
    positions.emplace_back(1.4f, 0.1f, 0.0f);   // [06] Goes in
    positions.emplace_back(1.8f, 0.3f, 0.0f);   // [07] Goes in
    positions.emplace_back(0.3f, 1.0f, 0.0f);   // [08] Goes in
    positions.emplace_back(1.1f, 0.6f, 0.0f);   // [09] Goes in
    positions.emplace_back(1.0f, 0.2f, 0.0f);   // [10] Goes in
    positions.emplace_back(1.8f, 0.5f, 0.0f);   // [11] Goes in

    for (size_t i = 0; i < positions.size(); ++i)
    {
        const glm::vec3& currentPosition = positions[i];

        Project001::CollisionBody2D* collisionBody2DPtr = new Project001::CollisionBody2D();
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionRectangle2D>& collisionRectangles = collisionBody2DPtr->GetCollisionRectangles();
            collisionRectangles.emplace_back(
                glm::vec2(-0.2f, -0.1f), 
                glm::vec2(0.2f, 0.1f)
            );
        }
        collisionBodyPtrs.push_back(collisionBody2DPtr);

        collisionBodyQuadTree2D.Insert(collisionBody2DPtr);
    }

    positions.clear();

    collisionBodyQuadTree2D.Clear();

    for (size_t i = 0; i < collisionBodyPtrs.size(); ++i)
    {
        delete collisionBodyPtrs[i];
    }
    collisionBodyPtrs.clear();

    // -------------------------------------------------------------------------
    // When the collision bodies are cleared, the allocated nodes should remain,
    // they just won't be used until they are needed.

    positions.emplace_back(0.1f, 0.1f, 0.0f);   // [00] Goes in
    positions.emplace_back(1.5f, 0.1f, 0.0f);   // [01] Goes in
    positions.emplace_back(0.1f, 0.7f, 0.0f);   // [02] Goes in
    positions.emplace_back(1.6f, 0.8f, 0.0f);   // [03] Goes in
    positions.emplace_back(1.8f, 0.9f, 0.0f);   // [04] Goes out
    positions.emplace_back(1.8f, 0.7f, 0.0f);   // [05] Goes out
    positions.emplace_back(1.8f, 0.5f, 0.0f);   // [06] Goes out
    positions.emplace_back(1.8f, 0.0f, 0.0f);   // [07] Goes out
    positions.emplace_back(0.0f, 0.9f, 0.0f);   // [08] Goes in
    positions.emplace_back(0.2f, 0.9f, 0.0f);   // [09] Goes in
    positions.emplace_back(0.7f, 0.7f, 0.0f);   // [10] Goes in
    positions.emplace_back(-0.1f, 0.4f, 0.0f);  // [11] Goes in
    positions.emplace_back(-0.2f, 0.6f, 0.0f);  // [12] Goes in
    positions.emplace_back(1.2f, -0.2f, 0.0f);  // [13] Goes in
    positions.emplace_back(0.4f, -0.2f, 0.0f);  // [14] Goes in
    positions.emplace_back(0.4f, -0.6f, 0.0f);  // [15] Goes in

    for (size_t i = 0; i < positions.size(); ++i)
    {
        const glm::vec3& currentPosition = positions[i];

        Project001::CollisionBody2D* collisionBody2DPtr = new Project001::CollisionBody2D();;
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(0.0f, 0.0f),
                0.1f
            );
        }
        collisionBodyPtrs.push_back(collisionBody2DPtr);

        collisionBodyQuadTree2D.Insert(collisionBody2DPtr);
    }

    positions.clear();

    collisionBodyQuadTree2D.Clear();

    for (size_t i = 0; i < collisionBodyPtrs.size(); ++i)
    {
        delete collisionBodyPtrs[i];
    }
    collisionBodyPtrs.clear();

    // -------------------------------------------------------------------------
    // Resetting the tree should recreate everything.

    collisionBodyQuadTree2D.Reset(glm::vec2(-0.8f, -1.6f), glm::vec2(0.8f, 1.6f), 4, 1);

    positions.emplace_back(0.0f, 0.0f, 0.0f);   // [00] Goes in
    positions.emplace_back(0.0f, 0.0f, 0.0f);   // [01] Goes in

    for (size_t i = 0; i < positions.size(); ++i)
    {
        const glm::vec3& currentPosition = positions[i];

        Project001::CollisionBody2D* collisionBody2DPtr = new Project001::CollisionBody2D();;
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints.emplace_back(
                glm::vec2(0.0f, 0.0f)
            );
        }
        collisionBodyPtrs.push_back(collisionBody2DPtr);

        collisionBodyQuadTree2D.Insert(collisionBody2DPtr);
    }

    positions.clear();

    collisionBodyQuadTree2D.Clear();

    for (size_t i = 0; i < collisionBodyPtrs.size(); ++i)
    {
        delete collisionBodyPtrs[i];
    }
    collisionBodyPtrs.clear();

    // -------------------------------------------------------------------------
    // Testing weird case where the bounds of the quad tree area are
    // min = 0,0 and max = 0.0.

    collisionBodyQuadTree2D.Reset(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 3, 1);

    positions.emplace_back(0.0f, 0.0f, 0.0f);   // [00] Goes in
    positions.emplace_back(0.0f, 0.0f, 0.0f);   // [01] Goes in

    for (size_t i = 0; i < positions.size(); ++i)
    {
        const glm::vec3& currentPosition = positions[i];

        Project001::CollisionBody2D* collisionBody2DPtr = new Project001::CollisionBody2D();;
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionPoint2D>& collisionPoints = collisionBody2DPtr->GetCollisionPoints();
            collisionPoints.emplace_back(
                glm::vec2(0.0f, 0.0f)
            );
        }
        collisionBodyPtrs.push_back(collisionBody2DPtr);

        collisionBodyQuadTree2D.Insert(collisionBody2DPtr);
    }

    positions.clear();

    collisionBodyQuadTree2D.Clear();

    for (size_t i = 0; i < collisionBodyPtrs.size(); ++i)
    {
        delete collisionBodyPtrs[i];
    }
    collisionBodyPtrs.clear();

    // -------------------------------------------------------------------------
    // Testing a depth of 1 and max nodes of 1.
    // (input of 0 should be interperted as 1)

    collisionBodyQuadTree2D.Reset(glm::vec2(-1.6f, -0.8f), glm::vec2(1.6f, 0.8f), 0, 0);

    positions.emplace_back(0.1f, 0.1f, 0.0f);   // [00] Goes in
    positions.emplace_back(1.5f, 0.1f, 0.0f);   // [01] Goes in
    positions.emplace_back(0.1f, 0.7f, 0.0f);   // [02] Goes in
    positions.emplace_back(1.6f, 0.8f, 0.0f);   // [03] Goes in
    positions.emplace_back(1.8f, 0.9f, 0.0f);   // [04] Goes out
    positions.emplace_back(1.8f, 0.7f, 0.0f);   // [05] Goes out
    positions.emplace_back(1.8f, 0.5f, 0.0f);   // [06] Goes out
    positions.emplace_back(1.8f, 0.0f, 0.0f);   // [07] Goes out
    positions.emplace_back(0.0f, 0.9f, 0.0f);   // [08] Goes in
    positions.emplace_back(0.2f, 0.9f, 0.0f);   // [09] Goes in
    positions.emplace_back(0.7f, 0.7f, 0.0f);   // [10] Goes in
    positions.emplace_back(-0.1f, 0.4f, 0.0f);  // [11] Goes in
    positions.emplace_back(-0.2f, 0.6f, 0.0f);  // [12] Goes in
    positions.emplace_back(1.2f, -0.2f, 0.0f);  // [13] Goes in
    positions.emplace_back(0.4f, -0.2f, 0.0f);  // [14] Goes in
    positions.emplace_back(0.4f, -0.6f, 0.0f);  // [15] Goes in

    for (size_t i = 0; i < positions.size(); ++i)
    {
        const glm::vec3& currentPosition = positions[i];

        Project001::CollisionBody2D* collisionBody2DPtr = new Project001::CollisionBody2D();;
        if (collisionBody2DPtr != nullptr)
        {
            collisionBody2DPtr->SetPosition(currentPosition);
            std::vector<Project001::CollisionCircle2D>& collisionCircles = collisionBody2DPtr->GetCollisionCircles();
            collisionCircles.emplace_back(
                glm::vec2(0.0f, 0.0f),
                0.1f
            );
        }
        collisionBodyPtrs.push_back(collisionBody2DPtr);

        collisionBodyQuadTree2D.Insert(collisionBody2DPtr);
    }

    positions.clear();

    collisionBodyQuadTree2D.Clear();

    for (size_t i = 0; i < collisionBodyPtrs.size(); ++i)
    {
        delete collisionBodyPtrs[i];
    }
    collisionBodyPtrs.clear();
}