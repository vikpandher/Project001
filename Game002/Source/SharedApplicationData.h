// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-01-12

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

    unsigned int scene001Id = static_cast<unsigned int>(-1);
    unsigned int scene002Id = static_cast<unsigned int>(-1);

    // Game Info ---------------------------------------------------------------

    unsigned int score = 0;
    float gameTime_s = 0.0f;

    // Player Controls ---------------------------------------------------------

    Project001::KeyCode snowball_keyCode = Project001::KeyCode::KEY_CODE_SPACE;

    Project001::KeyCode start_keyCode = Project001::KeyCode::KEY_CODE_SPACE;

    Project001::KeyCode pause_keyCode = Project001::KeyCode::KEY_CODE_ENTER;

    Project001::KeyCode quit_keyCode = Project001::KeyCode::KEY_CODE_ESCAPE;

    Project001::KeyCode up_keyCode = Project001::KeyCode::KEY_CODE_W;

    Project001::KeyCode left_keyCode = Project001::KeyCode::KEY_CODE_A;

    Project001::KeyCode down_keyCode = Project001::KeyCode::KEY_CODE_S;

    Project001::KeyCode right_keyCode = Project001::KeyCode::KEY_CODE_D;

    // Game Constants ----------------------------------------------------------

    unsigned int randomNumberSeed = 777;

    const float mainCamera_initialDistanceAway = 600.0f;

    const float ground_size = 512.0f;

    const float deadzone_size = 1024.0f;

    const float maxStage_size = 1088.0f;

    const float penguin_collisionRadius = 12.0f;
    const float penguin_grabOffset = 18.0f;
    const float penguin_grabRadius = 18.0f;

    // Resources ---------------------------------------------------------------

    // Pixel Font Resources
    const Project001::FontData* pixelFont_fontDataPtr = nullptr;
    const Project001::TextureData* pixelFont_textureDataPtr = nullptr;
    unsigned int pixelFont_textureId = static_cast<unsigned int>(-1);

    // General Resources
    Project001::MeshData* circle_meshDataPtr = nullptr;
    Project001::MeshData* circleWithArrow_meshDataPtr = nullptr;
    Project001::MeshData* hallowCircle_meshDataPtr = nullptr;

    // Main Menu Resources
    Project001::MeshData* authorText_meshDataPtr = nullptr;
    Project001::MeshData* introText_meshDataPtr = nullptr;
    Project001::MeshData* startText_meshDataPtr = nullptr;
    Project001::MeshData* titleText_meshDataPtr = nullptr;

    // Stage Resources
    Project001::MeshData* ground_meshDataPtr = nullptr;
    Project001::MeshData* groundCollision_meshDataPtr = nullptr;

    Project001::MeshData* water_meshDataPtr = nullptr;

    Project001::MeshData* deadZoneCollision_meshDataPtr = nullptr;

    // Stage Grid Resources
    Project001::MeshData* stageGrid_meshDataPtr = nullptr;
    unsigned int stageGrid_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* stageGridLabels_meshDataPtr = nullptr;
    unsigned int stageGridLabels_meshId = static_cast<unsigned int>(-1);

    Project001::MeshData* stageCollisionQuadTree_meshDataPtr = nullptr;

    // Actor Resources
    Project001::MeshData* penguin01_body_meshDataPtr = nullptr;
    unsigned int penguin01_body_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_flipper_right_meshDataPtr = nullptr;
    unsigned int penguin01_flipper_right_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_flipper_left_meshDataPtr = nullptr;
    unsigned int penguin01_flipper_left_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_foot_right_meshDataPtr = nullptr;
    unsigned int penguin01_foot_right_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_foot_left_meshDataPtr = nullptr;
    unsigned int penguin01_foot_left_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_head_meshDataPtr = nullptr;
    unsigned int penguin01_head_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_head_v2_meshDataPtr = nullptr;
    unsigned int penguin01_head_v2_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_beak_meshDataPtr = nullptr;
    unsigned int penguin01_beak_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_glasses_meshDataPtr = nullptr;
    unsigned int penguin01_glasses_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin01_glasses_v2_meshDataPtr = nullptr;
    unsigned int penguin01_glasses_v2_meshId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin01_textureDataPtr = nullptr;
    unsigned int penguin01_textureId = static_cast<unsigned int>(-1);

    Project001::MeshData* snowball_meshDataPtr = nullptr;
    unsigned int snowball_meshId = static_cast<unsigned int>(-1);

    // Ui Resources
    Project001::MeshData* uiLeftBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiLeftText01_meshDataPtr = nullptr;

    Project001::MeshData* uiMiddleBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiMiddleText01_meshDataPtr = nullptr;

    Project001::MeshData* uiRightBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiRightText01_meshDataPtr = nullptr;

    Project001::MeshData* uiPauseBackground_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText01_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText02_meshDataPtr = nullptr;
    Project001::MeshData* uiPauseText03_meshDataPtr = nullptr;
};