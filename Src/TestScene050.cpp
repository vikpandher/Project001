#include "TestScene050.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RenderedMesh.h"
#include "Engine/Math/CoordinateSystems.h"
#include "Engine/Math/VectorUtilities.h"
#include "Engine/Application.h"
#include "Engine/ComponentStores.h"
#include "Engine/Event.h"
#include "Engine/Logger.h"
#include "Engine/MeshLoader.h"
#include "Engine/Renderer.h"
#include "Engine/SoundPlayer.h"
#include "Engine/SoundLoader.h"
#include "Engine/TextureLoader.h"
#include "Engine/Window.h"

#include <thread>
#include <chrono>



// public ----------------------------------------------------------------------

TestScene050::TestScene050()
{
    ClearResources();

    // TestSoundPlayer();
}

TestScene050::~TestScene050()
{}

const char* TestScene050::Name()
{
    return "TestScene050";
}

// protected -------------------------------------------------------------------

bool TestScene050::OnInitialize()
{
    bool success = TestSceneBase001::OnInitialize();

    // Load meshes
    // -------------------------------------------------------------------------

    {
        Project001::MeshData* newMeshDataPtr = new Project001::MeshData();
        meshDataPtrArray_.push_back(newMeshDataPtr);
        _FAIL_CHECK(Project001::MeshLoader::GenerateIcosphere(*newMeshDataPtr, 0.16f, 0, false));
    }

    // Load sounds
    // -------------------------------------------------------------------------

    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(soundData01, "../Sounds/f_congratulations.ogg"));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundBuffer(
        soundBufferId01,
        soundData01.data,
        soundData01.sizeInBytes,
        soundData01.numberOfChannels,
        soundData01.sampleRate_Hz,
        soundData01.bitsPerSample,
        soundData01.sizeInFrames
    ));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundSource(
        soundSourceId01,
        soundBufferId01
    ));

    _FAIL_CHECK(Project001::SoundLoader::LoadSoundWAV(soundData02, "../Sounds/bounce.wav"));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundBuffer(
        soundBufferId02,
        soundData02.data,
        soundData02.sizeInBytes,
        soundData02.numberOfChannels,
        soundData02.sampleRate_Hz,
        soundData02.bitsPerSample,
        soundData02.sizeInFrames
    ));

    _FAIL_CHECK(soundPlayerPtr_->CreateSoundSource(
        soundSourceId02,
        soundBufferId02
    ));

    _FAIL_CHECK(soundPlayerPtr_->SetSoundSourceLooping(soundSourceId02, true));

    // generated shape entity 01
    // -------------------------------------------------------------------------
    {
        unsigned int tempEntityId;
        _FAIL_CHECK(componentStoresPtr_->CreateEntity(tempEntityId));
        entityIds_.push_back(tempEntityId);

        _FAIL_CHECK(componentStoresPtr_->CreateComponent<Project001::RenderedMesh>(tempEntityId));
        Project001::RenderedMesh* renderedMeshPtr;
        _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(tempEntityId, renderedMeshPtr));
        renderedMeshPtr->SetPosition(0.0f, 0.0f, 0.0f);
        renderedMeshPtr->SetMeshDataPtr(meshDataPtrArray_[0]);
        soundPlayerPtr_->PlaySoundSource(soundSourceId02);
    }

    return success && true;
}

bool TestScene050::OnDeinitialize()
{
    bool success = TestSceneBase001::OnDeinitialize();

    ClearResources();

    return success && true;
}

void TestScene050::OnHandleEvent(Project001::Event& event)
{
    Project001::DispatchEvent<Project001::KeyEvent>(event, std::bind(&TestScene050::ProcessKeyEvent, this, std::placeholders::_1));
    Project001::DispatchEvent<Project001::UpdateEvent>(event, std::bind(&TestScene050::ProcessUpdateEvent, this, std::placeholders::_1));

    TestSceneBase001::OnHandleEvent(event);
}

void TestScene050::ClearResources()
{
    soundData01.Clear();
    soundBufferId01 = (unsigned int)-1;
    soundSourceId01 = (unsigned int)-1;

    soundData02.Clear();
    soundBufferId02 = (unsigned int)-1;
    soundSourceId02 = (unsigned int)-1;
}

void TestScene050::ProcessKeyEvent(Project001::KeyEvent& keyEvent)
{
    Project001::KeyCode& keyCode = keyEvent.keyCode;
    Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
    {
        if (keyCode == Project001::KeyCode::KEY_CODE_X)
        {
            SendEvent(Project001::SwitchSceneEvent("TestScene002"));
            if (!IsActiveScene())
            {
                Deinitialize();
                SendEvent(Project001::InitializeSceneEvent("TestScene002"));
            }
        }
    }
}

void TestScene050::ProcessUpdateEvent(Project001::UpdateEvent& updateEvent)
{
    unsigned long long timestep_ns = updateEvent.timestep_ns;

    UpdateCameraListenerPosition();
    // UpdateShape01EntityPosition(timestep_ns);
}

void TestScene050::UpdateCameraListenerPosition()
{
    Project001::Camera* cameraPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::Camera>(mainCameraEntityId_, cameraPtr));
    glm::vec3 currentPosition = cameraPtr->GetPosition();
    glm::vec3 currentForward = cameraPtr->GetForwardVector();
    glm::vec3 currentUp = cameraPtr->GetUpVector();

    soundPlayerPtr_->SetListenerPosition(currentPosition);
    soundPlayerPtr_->SetListenerForwardDirection(currentForward);
    soundPlayerPtr_->SetListenerUpDirection(currentUp);
}

void TestScene050::UpdateShape01EntityPosition(unsigned long long timestep_ns)
{
    float timestep_s = (float)(timestep_ns / 1000000) / 1000;

    Project001::RenderedMesh* renderedMeshPtr;
    _FAIL_CHECK(componentStoresPtr_->GetComponent<Project001::RenderedMesh>(entityIds_[0], renderedMeshPtr));
    glm::vec3 currentPosition = renderedMeshPtr->GetPosition();

    if (currentPosition == glm::vec3(0.0f, 0.0f, 0.0f))
    {
        currentPosition.x = 1.28f;
    }

    glm::vec2 currentPositionPolar = Project001::CartesianToPolar(currentPosition.x, currentPosition.y);
    currentPositionPolar.y += timestep_s;
    glm::vec2 newPosition = Project001::PolarToCartesian(currentPositionPolar);

    renderedMeshPtr->SetPosition(newPosition.x, newPosition.y, 0.0f);

    glm::vec3 velocity((newPosition.x - currentPosition.x) / timestep_s, (newPosition.y - currentPosition.y) / timestep_s, 0.0f);

    _FAIL_CHECK(soundPlayerPtr_->SetSoundSourcePosition(soundSourceId02, renderedMeshPtr->GetPosition()));
    _FAIL_CHECK(soundPlayerPtr_->SetSoundSourceVelocity(soundSourceId02, velocity));
}

// private: --------------------------------------------------------------------

void TestScene050::TestSoundPlayer()
{
    Project001::SoundPlayer* soundPlayerPtr = Project001::SoundPlayer::Create();

    Project001::SoundData soundData01;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundOGG(soundData01, "../Sounds/f_congratulations.ogg"));

    unsigned int soundBufferId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        soundBufferId01,
        soundData01.data,
        soundData01.sizeInBytes,
        soundData01.numberOfChannels,
        soundData01.sampleRate_Hz,
        soundData01.bitsPerSample,
        soundData01.sizeInFrames
    ));

    unsigned int soundSourceId01;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        soundSourceId01,
        soundBufferId01
    ));

    Project001::SoundData soundData02;
    _FAIL_CHECK(Project001::SoundLoader::LoadSoundWAV(soundData02, "../Sounds/bounce.wav"));

    unsigned int soundBufferId02;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundBuffer(
        soundBufferId02,
        soundData02.data,
        soundData02.sizeInBytes,
        soundData02.numberOfChannels,
        soundData02.sampleRate_Hz,
        soundData02.bitsPerSample,
        soundData02.sizeInFrames
    ));

    unsigned int soundSourceId02;
    _FAIL_CHECK(soundPlayerPtr->CreateSoundSource(
        soundSourceId02,
        soundBufferId02
    ));

    // =========================================================================

    // Playing sound 01 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s)
        )
    );

    // Playing sound 01 for half time, pausing, then playing

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->PauseSoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    // Playing the sound01 for half time, then stopping and playing it again

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->StopSoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s * 0.5f)
        )
    );

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData01.duration_s)
        )
    );

    // Playing sound 02 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData02.duration_s * 2.0f)
        )
    );

    // Play sound 01 going clockwise starting at 12

    glm::vec2 positionVector(0.0f, 10.0f);

    for (size_t i = 0; i < 9; ++i)
    {
        soundPlayerPtr->SetSoundSourcePosition(soundSourceId01, glm::vec3(positionVector, 0.0f));

        _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

        std::this_thread::sleep_for(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::duration<float>(soundData01.duration_s)
            )
        );

        positionVector = Project001::Rotate2DVector(positionVector, -glm::quarter_pi<float>());
    }

    // Playing sound 02 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData02.duration_s * 2.0f)
        )
    );

    soundPlayerPtr->SetSoundSourcePosition(soundSourceId01, glm::vec3(0.0f, 0.0f, 0.0f));

    // Play sound 01 with the listener going clockwise starting at 6

    positionVector = glm::vec2(0.0f, -10.0f);

    for (size_t i = 0; i < 9; ++i)
    {
        soundPlayerPtr->SetListenerPosition(glm::vec3(positionVector, 0.0f));

        _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId01));

        std::this_thread::sleep_for(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::duration<float>(soundData01.duration_s)
            )
        );

        positionVector = Project001::Rotate2DVector(positionVector, -glm::quarter_pi<float>());
    }

    soundPlayerPtr->SetListenerPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Playing sound 02 in the center

    _FAIL_CHECK(soundPlayerPtr->PlaySoundSource(soundSourceId02));

    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<float>(soundData02.duration_s * 2.0f)
        )
    );

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    delete soundPlayerPtr;
}