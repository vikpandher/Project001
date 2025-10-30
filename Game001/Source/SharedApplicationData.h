// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2025-10-29

#pragma once



namespace Project001
{
    struct FontData;
    struct MeshData;
    struct SoundData;
    struct TextureData;
}

struct SharedApplicationData
{
    // Scene Ids ---------------------------------------------------------------

    unsigned int scene001Id = (unsigned int)-1;
    unsigned int scene002Id = (unsigned int)-1;

    // Game Info ---------------------------------------------------------------

    unsigned int score = 0;
    float nextHouseTurnOnWait_s = 0.0f;
    float remainingTime_s = 0.0f;

    // Game Constants ----------------------------------------------------------

    static const Project001::KeyCode keyCode_sprint = Project001::KeyCode::KEY_CODE_SPACE;
    static const Project001::MouseButton mouseButton_shine = Project001::MouseButton::MOUSE_BUTTON_LEFT;
    static const Project001::KeyCode keyCode_pause = Project001::KeyCode::KEY_CODE_ENTER;
    static const Project001::KeyCode keyCode_pauseReturn = Project001::KeyCode::KEY_CODE_ESCAPE;
    static const Project001::KeyCode keyCode_up = Project001::KeyCode::KEY_CODE_W;
    static const Project001::KeyCode keyCode_left = Project001::KeyCode::KEY_CODE_A;
    static const Project001::KeyCode keyCode_down = Project001::KeyCode::KEY_CODE_S;
    static const Project001::KeyCode keyCode_right = Project001::KeyCode::KEY_CODE_D;

    unsigned int randomNumberSeed = 777;

    float startingGameTime_s = 300.0f;

    float mainCamera_initialDistanceAway = 600.0f;

    float houseLight_radius = 152.0f;
    float lampLight_radius = 64.0f;
    float monsterVision_radius = 116.0f;
    float playerVision_radius = 320.0f;

    size_t house_initialOnCount = 4;
    size_t person_initalCount = 128;
    size_t monster_initialCount = 64;

    float house_randomTurnOnWait_s = 12.0f;
    float house_turnOffWait_s = 1.0f;

    float person_walkSpeed = 64.0f;
    float person_walkDurationMin_s = 2.0f;
    float person_walkDurationMax_s = 6.0f;

    float player_initialBattery_s = 12.0f;
    float player_initialStamina_s = 4.0f;
    float player_staminaRechargeRate = 0.2f;

    float player_walkSpeed = 64.0f;
    float player_runSpeed = 128.0f;

    float monster_walkSpeed = 64.0f;
    float monster_runSpeed = 96.0f;
    float monster_walkDurationMin_s = 2.0f;
    float monster_walkDurationMax_s = 6.0f;

    // Resources ---------------------------------------------------------------

    const Project001::FontData* pixelFont_FontDataPtr = nullptr;
    const Project001::TextureData* pixelFont_TextureDataPtr = nullptr;
    unsigned int pixelFont_TextureId = (unsigned int)-1;

    Project001::MeshData* introText_MeshDataPtr = nullptr;
    Project001::MeshData* authorText_MeshDataPtr = nullptr;

    Project001::MeshData* cursorCircle_MeshDataPtr = nullptr;

    Project001::MeshData* groundDark_MeshDataPtr = nullptr;
    Project001::TextureData* groundDark_TextureDataPtr = nullptr;
    unsigned int groundDark_TextureId = (unsigned int)-1;
    Project001::MeshData* groundLit_MeshDataPtr = nullptr;
    Project001::TextureData* groundLit_TextureDataPtr = nullptr;
    unsigned int groundLit_TextureId = (unsigned int)-1;
    Project001::MeshData* groundFog_MeshDataPtr = nullptr;
    Project001::MeshData* groundCollision_MeshDataPtr = nullptr;
    Project001::MeshData* groundCollisionQuadTree_MeshDataPtr = nullptr;

    Project001::MeshData* groundGrid_MeshDataPtr = nullptr;
    unsigned int groundGrid_MeshId = (unsigned int)-1;
    Project001::MeshData* groundGridLabels_MeshDataPtr = nullptr;
    unsigned int groundGridLabels_MeshId = (unsigned int)-1;

    Project001::MeshData* houseLit_MeshDataPtr = nullptr;
    Project001::TextureData* houseLit_TextureDataPtr = nullptr;
    unsigned int houseLit_TextureId = (unsigned int)-1;
    Project001::MeshData* houseDark_MeshDataPtr = nullptr;
    Project001::TextureData* houseDark_TextureDataPtr = nullptr;
    unsigned int houseDark_TextureId = (unsigned int)-1;
    Project001::MeshData* houseText_MeshDataPtr = nullptr;
    Project001::MeshData* houseCollision_MeshDataPtr = nullptr;

    Project001::MeshData* houseLightBottom_MeshDataPtr = nullptr;
    Project001::MeshData* houseLightTop_MeshDataPtr = nullptr;
    Project001::MeshData* houseLightCollision_MeshDataPtr = nullptr;
    Project001::MeshData* houseDoorCollision_MeshDataPtr = nullptr;

    Project001::MeshData* lampLit_MeshDataPtr = nullptr;
    Project001::TextureData* lampLit_TextureDataPtr = nullptr;
    unsigned int lampLit_TextureId = (unsigned int)-1;
    Project001::MeshData* lampDark_MeshDataPtr = nullptr;
    Project001::TextureData* lampDark_TextureDataPtr = nullptr;
    unsigned int lampDark_TextureId = (unsigned int)-1;
    Project001::MeshData* lampCollision_MeshDataPtr = nullptr;

    Project001::MeshData* lampLightBottom_MeshDataPtr = nullptr;
    Project001::MeshData* lampLightTop_MeshDataPtr = nullptr;
    Project001::MeshData* lampLightCollision_MeshDataPtr = nullptr;

    Project001::MeshData* monsterLit_MeshDataPtr = nullptr;
    Project001::TextureData* monsterLit_TextureDataPtr = nullptr;
    unsigned int monsterLit_TextureId = (unsigned int)-1;
    Project001::MeshData* monsterDark_MeshDataPtr = nullptr;
    Project001::TextureData* monsterDark_TextureDataPtr = nullptr;
    unsigned int monsterDark_TextureId = (unsigned int)-1;
    Project001::MeshData* monsterCollision_MeshDataPtr = nullptr;

    Project001::MeshData* monsterVisionCollision_MeshDataPtr = nullptr;

    Project001::MeshData* personDark_MeshDataPtr = nullptr;
    Project001::TextureData* personDark_TextureDataPtr = nullptr;
    unsigned int personDark_TextureId = (unsigned int)-1;
    Project001::MeshData* personLit_MeshDataPtr = nullptr;
    Project001::TextureData* personLit_TextureDataPtr = nullptr;
    unsigned int personLit_TextureId = (unsigned int)-1;
    Project001::MeshData* personCollision_MeshDataPtr = nullptr;

    Project001::MeshData* playerLightBottom_MeshDataPtr = nullptr;
    Project001::MeshData* playerLightTop_MeshDataPtr = nullptr;
    Project001::MeshData* playerLightStrong_MeshDataPtr = nullptr;
    Project001::MeshData* playerLightCollision_MeshDataPtr = nullptr;

    Project001::MeshData* uiTopLeftText_MeshDataPtr = nullptr;
    Project001::MeshData* uiTopMiddleText_MeshDataPtr = nullptr;
    Project001::MeshData* uiTopRightText_MeshDataPtr = nullptr;

    Project001::SoundData* hitHurt_SoundDataPtr = nullptr;
    unsigned int hitHurt_SoundBufferId = (unsigned int)-1;
    unsigned int hitHurt_SoundSourceId = (unsigned int)-1;
    Project001::SoundData* pickupCoin_SoundDataPtr = nullptr;
    unsigned int pickupCoin_SoundBufferId = (unsigned int)-1;
    unsigned int pickupCoin_SoundSourceId = (unsigned int)-1;
    Project001::SoundData* dropCoin_SoundDataPtr = nullptr;
    unsigned int dropCoin_SoundBufferId = (unsigned int)-1;
    unsigned int dropCoin_SoundSourceId = (unsigned int)-1;
};