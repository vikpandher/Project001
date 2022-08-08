#include "TestScene002.h"

#include "Engine/Components/RenderedModel.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/TextureLoader.h"
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

    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(shape01Mesh_, 0.64f, 0.48f, 0.32f, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(shape02Mesh_, 0.32f, 3, 2));
    _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(shape03Mesh_, 0.32f, 4, 4));
    _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(shape04Mesh_, 0.32f, 8, 8));

    _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(shape05Mesh_, 0.32f, 3, 2, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(shape06Mesh_, 0.32f, 4, 4, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateSphere(shape07Mesh_, 0.32f, 8, 8, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(shape08Mesh_, 0.64f, 0.64f, 0.64f, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(shape09Mesh_, 0.32f, 0));
    _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(shape10Mesh_, 0.32f, 1));
    _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(shape11Mesh_, 0.32f, 2));

    _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(shape12Mesh_, 0.32f, 0, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(shape13Mesh_, 0.32f, 1, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(shape14Mesh_, 0.32f, 2, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(shape15Mesh_, 0.64f, 0.48f, 0.32f));

    _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(shape16Mesh_, 0.64f, 0.32f, 3));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(shape17Mesh_, 0.64f, 0.32f, 4));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(shape18Mesh_, 0.64f, 0.32f, 8));

    _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(shape19Mesh_, 0.64f, 0.32f, 3, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(shape20Mesh_, 0.64f, 0.32f, 4, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCylinder(shape21Mesh_, 0.64f, 0.32f, 8, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(shape22Mesh_, 0.64f, 0.64f, 0.64f));

    _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(shape23Mesh_, 0.32f, 0.16f, 3, 1));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(shape24Mesh_, 0.32f, 0.16f, 4, 2));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(shape25Mesh_, 0.32f, 0.16f, 8, 4));

    _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(shape26Mesh_, 0.32f, 0.16f, 3, 1, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(shape27Mesh_, 0.32f, 0.16f, 4, 2, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCapsule(shape28Mesh_, 0.32f, 0.16f, 8, 4, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateBox(shape29Mesh_, 0.32f, 0.32f, 0.32f, false));

    _FAIL_CHECK(Project001::MeshLoader::GenerateCone(shape30Mesh_, 0.64f, 0.32f, 3));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCone(shape31Mesh_, 0.64f, 0.32f, 4));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCone(shape32Mesh_, 0.64f, 0.32f, 8));

    _FAIL_CHECK(Project001::MeshLoader::GenerateCone(shape33Mesh_, 0.64f, 0.32f, 3, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCone(shape34Mesh_, 0.64f, 0.32f, 4, false));
    _FAIL_CHECK(Project001::MeshLoader::GenerateCone(shape35Mesh_, 0.64f, 0.32f, 8, false));

    // test TurnInsideOut
    if (false)
    {
        Project001::MeshLoader::TurnInsideOut(shape01Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape02Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape03Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape04Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape05Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape06Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape07Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape08Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape09Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape10Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape11Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape12Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape13Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape14Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape15Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape16Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape17Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape18Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape19Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape20Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape21Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape22Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape23Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape24Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape25Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape26Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape27Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape28Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape29Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape30Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape31Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape32Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape33Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape34Mesh_);
        Project001::MeshLoader::TurnInsideOut(shape35Mesh_);
    }

    // test SizeMeshAlongNormals
    if (false)
    {
        Project001::MeshLoader::SizeMeshAlongNormals(shape01Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape02Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape03Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape04Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape05Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape06Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape07Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape08Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape09Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape10Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape11Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape12Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape13Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape14Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape15Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape16Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape17Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape18Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape19Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape20Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape21Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape22Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape23Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape24Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape25Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape26Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape27Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape28Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape29Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape30Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape31Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape32Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape33Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape34Mesh_, 0.16f);
        Project001::MeshLoader::SizeMeshAlongNormals(shape35Mesh_, 0.16f);
    }

    // Load textures
    // -------------------------------------------------------------------------

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/earth.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(earthTextureId_, 1, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
    }

    {
        Project001::TextureData textureData;
        _FAIL_CHECK(Project001::TextureLoader::LoadTexture(textureData, "../Textures/120_60_rgb.png"));
        _FAIL_CHECK(rendererPtr_->CreateTexture(rgb120x60TextureId_, 2, textureData.data, textureData.width, textureData.height, textureData.bytesPerPixel));
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
        renderedModelPtr->SetMeshDataPtr(&shape01Mesh_);
    }

    // generated shape entity 02
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape02EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape02EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape02EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape02Mesh_);
        renderedModelPtr->SetTextureId(earthTextureId_);
    }

    // generated shape entity 03
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape03EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape03EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape03EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape03Mesh_);
        renderedModelPtr->SetTextureId(earthTextureId_);
    }

    // generated shape entity 04
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape04EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape04EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape04EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape04Mesh_);
        renderedModelPtr->SetTextureId(earthTextureId_);
    }

    // generated shape entity 05
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape05EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape05EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape05EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape05Mesh_);
        renderedModelPtr->SetTextureId(earthTextureId_);
    }

    // generated shape entity 06
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape06EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape06EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape06EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape06Mesh_);
        renderedModelPtr->SetTextureId(earthTextureId_);
    }

    // generated shape entity 07
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape07EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape07EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape07EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape07Mesh_);
        renderedModelPtr->SetTextureId(earthTextureId_);
    }

    // generated shape entity 08
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape08EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape08EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape08EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape08Mesh_);
    }

    // generated shape entity 09
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape09EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape09EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape09EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape09Mesh_);
        renderedModelPtr->SetTextureId(rgb120x60TextureId_);
    }

    // generated shape entity 10
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape10EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape10EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape10EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape10Mesh_);
        renderedModelPtr->SetTextureId(rgb120x60TextureId_);
    }

    // generated shape entity 11
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape11EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape11EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape11EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape11Mesh_);
        renderedModelPtr->SetTextureId(rgb120x60TextureId_);
    }

    // generated shape entity 12
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape12EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape12EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape12EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape12Mesh_);
        renderedModelPtr->SetTextureId(rgb120x60TextureId_);
    }

    // generated shape entity 13
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape13EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape13EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape13EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape13Mesh_);
        renderedModelPtr->SetTextureId(rgb120x60TextureId_);
    }

    // generated shape entity 14
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape14EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape14EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape14EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape14Mesh_);
        renderedModelPtr->SetTextureId(rgb120x60TextureId_);
    }

    // generated shape entity 15
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape15EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape15EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape15EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape15Mesh_);
    }

    // generated shape entity 16
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape16EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape16EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape16EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape16Mesh_);
    }

    // generated shape entity 17
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape17EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape17EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape17EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape17Mesh_);
    }

    // generated shape entity 18
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape18EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape18EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape18EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape18Mesh_);
    }

    // generated shape entity 19
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape19EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape19EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape19EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape19Mesh_);
    }

    // generated shape entity 20
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape20EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape20EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape20EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape20Mesh_);
    }

    // generated shape entity 21
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape21EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape21EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape21EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape21Mesh_);
    }

    // generated shape entity 22
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape22EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape22EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape22EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape22Mesh_);
    }

    // generated shape entity 23
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape23EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape23EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape23EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape23Mesh_);
    }

    // generated shape entity 24
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape24EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape24EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape24EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape24Mesh_);
    }

    // generated shape entity 25
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape25EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape25EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape25EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape25Mesh_);
    }

    // generated shape entity 26
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape26EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape26EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape26EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape26Mesh_);
    }

    // generated shape entity 27
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape27EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape27EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape27EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape27Mesh_);
    }

    // generated shape entity 28
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape28EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape28EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape28EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape28Mesh_);
    }

    // generated shape entity 29
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape29EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape29EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape29EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape29Mesh_);
    }

    // generated shape entity 30
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape30EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape30EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape30EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape30Mesh_);
    }

    // generated shape entity 31
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape31EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape31EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape31EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape31Mesh_);
    }

    // generated shape entity 32
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape32EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape32EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape32EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape32Mesh_);
    }

    // generated shape entity 33
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape33EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape33EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape33EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape33Mesh_);
    }

    // generated shape entity 34
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape34EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape34EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape34EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape34Mesh_);
    }

    // generated shape entity 35
    // -------------------------------------------------------------------------
    {
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(shape35EntityId_));
        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedModel>(shape35EntityId_));

        Project001::RenderedModel* renderedModelPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedModel>(shape35EntityId_, renderedModelPtr));
        renderedModelPtr->SetPosition(modelEntityPositions[positionPosition++]);
        renderedModelPtr->SetMeshDataPtr(&shape35Mesh_);
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
    shape01Mesh_.Clear();
    shape02Mesh_.Clear();
    shape03Mesh_.Clear();
    shape04Mesh_.Clear();
    shape05Mesh_.Clear();
    shape06Mesh_.Clear();
    shape07Mesh_.Clear();
    shape08Mesh_.Clear();
    shape09Mesh_.Clear();
    shape10Mesh_.Clear();
    shape11Mesh_.Clear();
    shape12Mesh_.Clear();
    shape13Mesh_.Clear();
    shape14Mesh_.Clear();
    shape15Mesh_.Clear();
    shape16Mesh_.Clear();
    shape17Mesh_.Clear();
    shape18Mesh_.Clear();
    shape19Mesh_.Clear();
    shape20Mesh_.Clear();
    shape21Mesh_.Clear();
    shape22Mesh_.Clear();
    shape23Mesh_.Clear();
    shape24Mesh_.Clear();
    shape25Mesh_.Clear();
    shape26Mesh_.Clear();
    shape27Mesh_.Clear();
    shape28Mesh_.Clear();
    shape29Mesh_.Clear();
    shape30Mesh_.Clear();
    shape31Mesh_.Clear();
    shape32Mesh_.Clear();
    shape33Mesh_.Clear();
    shape34Mesh_.Clear();
    shape35Mesh_.Clear();

    earthTextureId_ = (unsigned int)-1;
    rgb120x60TextureId_ = (unsigned int)-1;

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
