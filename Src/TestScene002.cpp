#include "TestScene002.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshStores.h"
#include "Engine/Renderer.h"
#include "Engine/TextureStores.h"
#include "Engine/Window.h"



// public: ---------------------------------------------------------------------

TestScene002::TestScene002()
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
    TestSceneBase001::Initialize();

    // Load meshes
    // -------------------------------------------------------------------------

    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape01MeshIndex_, 0.64f, 0.48f, 0.32f, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape02MeshIndex_, 0.32f, 3, 2));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape03MeshIndex_, 0.32f, 4, 4));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape04MeshIndex_, 0.32f, 8, 8));

    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape05MeshIndex_, 0.32f, 3, 2, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape06MeshIndex_, 0.32f, 4, 4, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateSphere(shape07MeshIndex_, 0.32f, 8, 8, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape08MeshIndex_, 0.64f, 0.64f, 0.64f, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape09MeshIndex_, 0.32f, 0));
    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape10MeshIndex_, 0.32f, 1));
    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape11MeshIndex_, 0.32f, 2));

    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape12MeshIndex_, 0.32f, 0, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape13MeshIndex_, 0.32f, 1, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateIcosphere(shape14MeshIndex_, 0.32f, 2, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape15MeshIndex_, 0.64f, 0.48f, 0.32f));

    _FAIL_CHECK(meshStoresPtr_->GenerateCylinder(shape16MeshIndex_, 0.64f, 0.32f, 3));
    _FAIL_CHECK(meshStoresPtr_->GenerateCylinder(shape17MeshIndex_, 0.64f, 0.32f, 4));
    _FAIL_CHECK(meshStoresPtr_->GenerateCylinder(shape18MeshIndex_, 0.64f, 0.32f, 8));

    _FAIL_CHECK(meshStoresPtr_->GenerateCylinder(shape19MeshIndex_, 0.64f, 0.32f, 3, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateCylinder(shape20MeshIndex_, 0.64f, 0.32f, 4, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateCylinder(shape21MeshIndex_, 0.64f, 0.32f, 8, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape22MeshIndex_, 0.64f, 0.64f, 0.64f));

    _FAIL_CHECK(meshStoresPtr_->GenerateCapsule(shape23MeshIndex_, 0.32f, 0.16f, 3, 1));
    _FAIL_CHECK(meshStoresPtr_->GenerateCapsule(shape24MeshIndex_, 0.32f, 0.16f, 4, 2));
    _FAIL_CHECK(meshStoresPtr_->GenerateCapsule(shape25MeshIndex_, 0.32f, 0.16f, 8, 4));

    _FAIL_CHECK(meshStoresPtr_->GenerateCapsule(shape26MeshIndex_, 0.32f, 0.16f, 3, 1, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateCapsule(shape27MeshIndex_, 0.32f, 0.16f, 4, 2, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateCapsule(shape28MeshIndex_, 0.32f, 0.16f, 8, 4, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateBox(shape29MeshIndex_, 0.32f, 0.32f, 0.32f, false));

    _FAIL_CHECK(meshStoresPtr_->GenerateCone(shape30MeshIndex_, 0.64f, 0.32f, 3));
    _FAIL_CHECK(meshStoresPtr_->GenerateCone(shape31MeshIndex_, 0.64f, 0.32f, 4));
    _FAIL_CHECK(meshStoresPtr_->GenerateCone(shape32MeshIndex_, 0.64f, 0.32f, 8));

    _FAIL_CHECK(meshStoresPtr_->GenerateCone(shape33MeshIndex_, 0.64f, 0.32f, 3, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateCone(shape34MeshIndex_, 0.64f, 0.32f, 4, false));
    _FAIL_CHECK(meshStoresPtr_->GenerateCone(shape35MeshIndex_, 0.64f, 0.32f, 8, false));

    // test TurnInsideOut
    if (false)
    {
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape01MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape02MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape03MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape04MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape05MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape06MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape07MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape08MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape09MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape10MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape11MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape12MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape13MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape14MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape15MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape16MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape17MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape18MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape19MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape20MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape21MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape22MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape23MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape24MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape25MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape26MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape27MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape28MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape29MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape30MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape31MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape32MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape33MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape34MeshIndex_));
        _FAIL_CHECK(meshStoresPtr_->TurnInsideOut(shape35MeshIndex_));
    }

    // test SizeMeshAlongNormals
    if (false)
    {
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape01MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape02MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape03MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape04MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape05MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape06MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape07MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape08MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape09MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape10MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape11MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape12MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape13MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape14MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape15MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape16MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape17MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape18MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape19MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape20MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape21MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape22MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape23MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape24MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape25MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape26MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape27MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape28MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape29MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape30MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape31MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape32MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape33MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape34MeshIndex_, 0.16f));
        _FAIL_CHECK(meshStoresPtr_->SizeMeshAlongNormals(shape35MeshIndex_, 0.16f));
    }

    // Load textures
    // -------------------------------------------------------------------------

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(earthTextureIndex_, "../Textures/earth.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(earthTextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(earthTextureIndex_, 1, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
    }

    {
        _FAIL_CHECK(textureStoresPtr_->LoadTexture(rgb120x60TextureIndex_, "../Textures/120_60_rgb.png"));
        Project001::TextureData textureData;
        _FAIL_CHECK(textureStoresPtr_->GetTexture(rgb120x60TextureIndex_, textureData));
        _FAIL_CHECK(rendererPtr_->AddTexture(rgb120x60TextureIndex_, 2, textureData.data, textureData.width, textureData.height, textureData.numberOfComponents));
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
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape08EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape08MeshIndex_);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape09EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape09EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape09EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape09MeshIndex_);
        renderedModelPtr->SetTextureIndex(rgb120x60TextureIndex_);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape10EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape10EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape10EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape10MeshIndex_);
        renderedModelPtr->SetTextureIndex(rgb120x60TextureIndex_);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape11EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape11EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape11EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape11MeshIndex_);
        renderedModelPtr->SetTextureIndex(rgb120x60TextureIndex_);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape12EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape12EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape12EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape12MeshIndex_);
        renderedModelPtr->SetTextureIndex(rgb120x60TextureIndex_);
    }

    // generated shape entity 13
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape13EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape13EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape13EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape13MeshIndex_);
        renderedModelPtr->SetTextureIndex(rgb120x60TextureIndex_);
    }

    // generated shape entity 14
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape14EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape14EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape14EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape14MeshIndex_);
        renderedModelPtr->SetTextureIndex(rgb120x60TextureIndex_);
    }

    // generated shape entity 15
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape15EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape15EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape15EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape15MeshIndex_);
    }

    // generated shape entity 16
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape16EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape16EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape16EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape16MeshIndex_);
    }

    // generated shape entity 17
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape17EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape17EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape17EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape17MeshIndex_);
    }

    // generated shape entity 18
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape18EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape18EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape18EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape18MeshIndex_);
    }

    // generated shape entity 19
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape19EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape19EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape19EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape19MeshIndex_);
    }

    // generated shape entity 20
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape20EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape20EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape20EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape20MeshIndex_);
    }

    // generated shape entity 21
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape21EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape21EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape21EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape21MeshIndex_);
    }

    // generated shape entity 22
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape22EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape22EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape22EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape22MeshIndex_);
    }

    // generated shape entity 23
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape23EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape23EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape23EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape23MeshIndex_);
    }

    // generated shape entity 24
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape24EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape24EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape24EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape24MeshIndex_);
    }

    // generated shape entity 25
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape25EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape25EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape25EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape25MeshIndex_);
    }

    // generated shape entity 26
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape26EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape26EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape26EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape26MeshIndex_);
    }

    // generated shape entity 27
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape27EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape27EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape27EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape27MeshIndex_);
    }

    // generated shape entity 28
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape28EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape28EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape28EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape28MeshIndex_);
    }

    // generated shape entity 29
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape29EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape29EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape29EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape29MeshIndex_);
    }

    // generated shape entity 30
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape30EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape30EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape30EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape30MeshIndex_);
    }

    // generated shape entity 31
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape31EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape31EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape31EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape31MeshIndex_);
    }

    // generated shape entity 32
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape32EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape32EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape32EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape32MeshIndex_);
    }

    // generated shape entity 33
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape33EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape33EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape33EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape33MeshIndex_);
    }

    // generated shape entity 34
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape34EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape34EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape34EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape34MeshIndex_);
    }

    // generated shape entity 35
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape35EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape35EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape35EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshIndex(shape35MeshIndex_);
    }
}

void TestScene002::Deinitialize()
{
    TestSceneBase001::Deinitialize();

    ClearIndiciesAndEntityIds();
}

void TestScene002::OnEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene002::ProcessKeyEvent, this, std::placeholders::_1));

    TestSceneBase001::OnEvent(event);
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
    shape09MeshIndex_ = (unsigned int)-1;
    shape10MeshIndex_ = (unsigned int)-1;
    shape11MeshIndex_ = (unsigned int)-1;
    shape12MeshIndex_ = (unsigned int)-1;
    shape13MeshIndex_ = (unsigned int)-1;
    shape14MeshIndex_ = (unsigned int)-1;
    shape15MeshIndex_ = (unsigned int)-1;
    shape16MeshIndex_ = (unsigned int)-1;
    shape17MeshIndex_ = (unsigned int)-1;
    shape18MeshIndex_ = (unsigned int)-1;
    shape19MeshIndex_ = (unsigned int)-1;
    shape20MeshIndex_ = (unsigned int)-1;
    shape21MeshIndex_ = (unsigned int)-1;
    shape22MeshIndex_ = (unsigned int)-1;
    shape23MeshIndex_ = (unsigned int)-1;
    shape24MeshIndex_ = (unsigned int)-1;
    shape25MeshIndex_ = (unsigned int)-1;
    shape26MeshIndex_ = (unsigned int)-1;
    shape27MeshIndex_ = (unsigned int)-1;
    shape28MeshIndex_ = (unsigned int)-1;
    shape29MeshIndex_ = (unsigned int)-1;
    shape30MeshIndex_ = (unsigned int)-1;
    shape31MeshIndex_ = (unsigned int)-1;
    shape32MeshIndex_ = (unsigned int)-1;
    shape33MeshIndex_ = (unsigned int)-1;
    shape34MeshIndex_ = (unsigned int)-1;
    shape35MeshIndex_ = (unsigned int)-1;

    earthTextureIndex_ = (unsigned int)-1;
    rgb120x60TextureIndex_ = (unsigned int)-1;

    shape01EntityId_ = (unsigned int)-1;
    shape02EntityId_ = (unsigned int)-1;
    shape03EntityId_ = (unsigned int)-1;
    shape04EntityId_ = (unsigned int)-1;
    shape05EntityId_ = (unsigned int)-1;
    shape06EntityId_ = (unsigned int)-1;
    shape07EntityId_ = (unsigned int)-1;
    shape08EntityId_ = (unsigned int)-1;
    shape09EntityId_ = (unsigned int)-1;
    shape10EntityId_ = (unsigned int)-1;
    shape11EntityId_ = (unsigned int)-1;
    shape12EntityId_ = (unsigned int)-1;
    shape13EntityId_ = (unsigned int)-1;
    shape14EntityId_ = (unsigned int)-1;
    shape15EntityId_ = (unsigned int)-1;
    shape16EntityId_ = (unsigned int)-1;
    shape17EntityId_ = (unsigned int)-1;
    shape18EntityId_ = (unsigned int)-1;
    shape19EntityId_ = (unsigned int)-1;
    shape20EntityId_ = (unsigned int)-1;
    shape21EntityId_ = (unsigned int)-1;
    shape22EntityId_ = (unsigned int)-1;
    shape23EntityId_ = (unsigned int)-1;
    shape24EntityId_ = (unsigned int)-1;
    shape25EntityId_ = (unsigned int)-1;
    shape26EntityId_ = (unsigned int)-1;
    shape27EntityId_ = (unsigned int)-1;
    shape28EntityId_ = (unsigned int)-1;
    shape29EntityId_ = (unsigned int)-1;
    shape30EntityId_ = (unsigned int)-1;
    shape31EntityId_ = (unsigned int)-1;
    shape32EntityId_ = (unsigned int)-1;
    shape33EntityId_ = (unsigned int)-1;
    shape34EntityId_ = (unsigned int)-1;
    shape35EntityId_ = (unsigned int)-1;
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
            SendEvent(Project001::SwitchSceneEvent("TestScene003"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene003"));
            }
        }
    }
}
