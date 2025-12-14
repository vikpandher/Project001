// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-12-13

#include "TestAnimatedSpriteScene001.h"

#include "Components/RenderedMesh.h"
#include "ComponentStores.h"
#include "Logger.h"



// public ----------------------------------------------------------------------

TestAnimatedSpriteScene001::TestAnimatedSpriteScene001(Project001::Application* applicationPtr)
    : Scene(applicationPtr)
    , frameCount_(0)
    , frameMeshDataPtrsPtr_(nullptr)
    , frameDurations_ns_ptr_(nullptr)
    , spriteEntityId_(static_cast<unsigned int>(-1))
    , currentFrame_(0)
    , animationTimeDebt_ns_(0)
{}

TestAnimatedSpriteScene001::~TestAnimatedSpriteScene001()
{}

void TestAnimatedSpriteScene001::HandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestAnimatedSpriteScene001::ProcessUpdateEvent, this, std::placeholders::_1));
}

void TestAnimatedSpriteScene001::Initialize(const InitializationInfo& initializationInfo)
{
    LOG_INFO("INITIALIZING:   TestAnimatedSpriteScene001: " << GetId());

    frameCount_ = initializationInfo.frameCount;
    frameMeshDataPtrsPtr_ = initializationInfo.frameMeshDataPtrsPtr;
    frameDurations_ns_ptr_ = initializationInfo.frameDurations_ns_ptr;

    // -------------------------------------------------------------------------

    if (frameCount_ == 0)
    {
        return;
    }

    // -------------------------------------------------------------------------

    GetComponentStoresPtr()->CreateEntity(spriteEntityId_);

    FAIL_CHECK(GetComponentStoresPtr()->CreateComponent<Project001::RenderedMesh>(spriteEntityId_));
    Project001::RenderedMesh* renderedMeshPtr = nullptr;
    FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, spriteEntityId_));
    if (renderedMeshPtr != nullptr)
    {
        renderedMeshPtr->SetMeshDataPtr(frameMeshDataPtrsPtr_[currentFrame_]);
        renderedMeshPtr->SetCameraMask(*initializationInfo.cameraMaskPtr);
        renderedMeshPtr->SetPosition(*initializationInfo.positionPtr);
        renderedMeshPtr->SetTextureId(*initializationInfo.textureIdPtr);
    }
}

void TestAnimatedSpriteScene001::Deinitialize()
{
    LOG_INFO("DEINITIALIZING: TestAnimatedSpriteScene001: " << GetId());

    // -------------------------------------------------------------------------

    frameCount_ = 0;
    frameMeshDataPtrsPtr_ = nullptr;
    frameDurations_ns_ptr_ = nullptr;

    GetComponentStoresPtr()->DeleteEntity(spriteEntityId_);
    spriteEntityId_ = static_cast<unsigned int>(-1);

    currentFrame_ = 0;
    animationTimeDebt_ns_ = 0;
}

// protected -------------------------------------------------------------------

void TestAnimatedSpriteScene001::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    if (frameCount_ == 0)
    {
        return;
    }

    animationTimeDebt_ns_ += updateEvent.timestep_ns;

    if (animationTimeDebt_ns_ > frameDurations_ns_ptr_[currentFrame_])
    {
        animationTimeDebt_ns_ -= frameDurations_ns_ptr_[currentFrame_++];
        if (currentFrame_ >= frameCount_)
        {
            currentFrame_ = 0;
        }

        Project001::RenderedMesh* renderedMeshPtr = nullptr;
        FAIL_CHECK(GetComponentStoresPtr()->GetComponent<Project001::RenderedMesh>(renderedMeshPtr, spriteEntityId_));
        if (renderedMeshPtr != nullptr)
        {
            renderedMeshPtr->SetMeshDataPtr(frameMeshDataPtrsPtr_[currentFrame_]);
        }
    }
}