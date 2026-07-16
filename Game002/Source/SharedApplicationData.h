// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-15

#pragma once

#include "glm/gtc/constants.hpp"

#include "Event.h"



namespace Project001
{
    struct FontData;
    struct MeshData;
    struct SoundData;
    struct TextureData;

    class Window;
}

struct PlayerCreationInfo
{
    size_t playerNumber = 0;
    bool turnedOn = true;

    enum class ControlScheme
    {
        CONTROL_SCHEME_UNKNOWN,
        CONTROL_SCHEME_KEYBOARD_1,
        CONTROL_SCHEME_KEYBOARD_2,
        CONTROL_SCHEME_CONTROLLER_1,
        CONTROL_SCHEME_CONTROLLER_2,
        CONTROL_SCHEME_CONTROLLER_3,
        CONTROL_SCHEME_CONTROLLER_4
    };

    static PlayerCreationInfo::ControlScheme PlayerCreationInfo::StringToControlScheme(const std::string& str);

    ControlScheme controlScheme = ControlScheme::CONTROL_SCHEME_KEYBOARD_1;

    float axisDeadzone = 0.2f;

    unsigned int start_pressCount = 0;
    unsigned int pause_pressCount = 0;
    unsigned int quit_pressCount = 0;
    unsigned int left_pressCount = 0;
    unsigned int right_pressCount = 0;
    unsigned int up_pressCount = 0;
    unsigned int down_pressCount = 0;
    unsigned int grab_pressCount = 0;
    unsigned int throw_pressCount = 0;
    float leftRightAxisValue = 0.0f;
    float upDownAxisValue = 0.0f;
};

struct SharedApplicationData
{
    // Scene Ids ---------------------------------------------------------------

    unsigned int scene001Id = static_cast<unsigned int>(-1);
    unsigned int scene002Id = static_cast<unsigned int>(-1);

    // Game Info ---------------------------------------------------------------

    unsigned int score = 0;
    float gameTime_s = 0.0f;

    static constexpr size_t s_player_count = 4;
    PlayerCreationInfo playerCreationInfos[s_player_count] = {
        {0, true, PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_1},
        {1, true, PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_2},
        {2, true, PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_1},
        {3, true, PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_2}
    };

    void UpdateKeyboardButtonPresses(const Project001::KeyEvent& keyEvent);
    void UpdateMouseButtonPresses(const Project001::MouseButtonEvent& mouseButtonEvent);
    void UpdateButtonPressCounts(const Project001::Window* windowPtr);

    // Player Controls ---------------------------------------------------------

    Project001::KeyCode keyboard_1_start_keyCode = Project001::KeyCode::KEY_CODE_SPACE;
    Project001::KeyCode keyboard_1_pause_keyCode = Project001::KeyCode::KEY_CODE_ENTER;
    Project001::KeyCode keyboard_1_quit_keyCode = Project001::KeyCode::KEY_CODE_ESCAPE;
    Project001::KeyCode keyboard_1_left_keyCode = Project001::KeyCode::KEY_CODE_A;
    Project001::KeyCode keyboard_1_right_keyCode = Project001::KeyCode::KEY_CODE_D;
    Project001::KeyCode keyboard_1_up_keyCode = Project001::KeyCode::KEY_CODE_W;
    Project001::KeyCode keyboard_1_down_keyCode = Project001::KeyCode::KEY_CODE_S;
    Project001::KeyCode keyboard_1_grab_keyCode = Project001::KeyCode::KEY_CODE_SPACE;
    Project001::KeyCode keyboard_1_throw_keyCode = Project001::KeyCode::KEY_CODE_RIGHT_ALT;

    bool keyboard_1_start_pressed = false;
    bool keyboard_1_pause_pressed = false;
    bool keyboard_1_quit_pressed = false;
    bool keyboard_1_left_pressed = false;
    bool keyboard_1_right_pressed = false;
    bool keyboard_1_up_pressed = false;
    bool keyboard_1_down_pressed = false;
    bool keyboard_1_grab_pressed = false;
    bool keyboard_1_throw_pressed = false;

    Project001::KeyCode keyboard_2_start_keyCode = Project001::KeyCode::KEY_CODE_KP_0;
    Project001::KeyCode keyboard_2_pause_keyCode = Project001::KeyCode::KEY_CODE_KP_ENTER;
    Project001::KeyCode keyboard_2_quit_keyCode = Project001::KeyCode::KEY_CODE_KP_SUBTRACT;
    Project001::KeyCode keyboard_2_left_keyCode = Project001::KeyCode::KEY_CODE_LEFT;
    Project001::KeyCode keyboard_2_right_keyCode = Project001::KeyCode::KEY_CODE_RIGHT;
    Project001::KeyCode keyboard_2_up_keyCode = Project001::KeyCode::KEY_CODE_UP;
    Project001::KeyCode keyboard_2_down_keyCode = Project001::KeyCode::KEY_CODE_DOWN;
    Project001::KeyCode keyboard_2_grab_keyCode = Project001::KeyCode::KEY_CODE_KP_0;
    Project001::KeyCode keyboard_2_throw_keyCode = Project001::KeyCode::KEY_CODE_KP_DECIMAL;

    bool keyboard_2_start_pressed = false;
    bool keyboard_2_pause_pressed = false;
    bool keyboard_2_quit_pressed = false;
    bool keyboard_2_left_pressed = false;
    bool keyboard_2_right_pressed = false;
    bool keyboard_2_up_pressed = false;
    bool keyboard_2_down_pressed = false;
    bool keyboard_2_grab_pressed = false;
    bool keyboard_2_throw_pressed = false;

    unsigned int controller_1_start_buttonIndex = 0;
    unsigned int controller_1_pause_buttonIndex = 7;
    unsigned int controller_1_quit_buttonIndex = 1;
    unsigned int controller_1_left_buttonIndex = 13;
    unsigned int controller_1_right_buttonIndex = 11;
    unsigned int controller_1_up_buttonIndex = 10;
    unsigned int controller_1_down_buttonIndex = 12;
    unsigned int controller_1_grab_buttonIndex = 0;
    unsigned int controller_1_throw_buttonIndex = 5;
    unsigned int controller_1_moveRightLeft_axisIndex = 0;
    unsigned int controller_1_moveDownUp_axisIndex = 1;
    float controller_1_axisDeadzone = 0.2f;

    unsigned int controller_2_start_buttonIndex = 0;
    unsigned int controller_2_pause_buttonIndex = 7;
    unsigned int controller_2_quit_buttonIndex = 1;
    unsigned int controller_2_left_buttonIndex = 13;
    unsigned int controller_2_right_buttonIndex = 11;
    unsigned int controller_2_up_buttonIndex = 10;
    unsigned int controller_2_down_buttonIndex = 12;
    unsigned int controller_2_grab_buttonIndex = 0;
    unsigned int controller_2_throw_buttonIndex = 5;
    unsigned int controller_2_moveRightLeft_axisIndex = 0;
    unsigned int controller_2_moveDownUp_axisIndex = 1;
    float controller_2_axisDeadzone = 0.2f;

    unsigned int controller_3_start_buttonIndex = 0;
    unsigned int controller_3_pause_buttonIndex = 7;
    unsigned int controller_3_quit_buttonIndex = 1;
    unsigned int controller_3_left_buttonIndex = 13;
    unsigned int controller_3_right_buttonIndex = 11;
    unsigned int controller_3_up_buttonIndex = 10;
    unsigned int controller_3_down_buttonIndex = 12;
    unsigned int controller_3_grab_buttonIndex = 0;
    unsigned int controller_3_throw_buttonIndex = 5;
    unsigned int controller_3_moveRightLeft_axisIndex = 0;
    unsigned int controller_3_moveDownUp_axisIndex = 1;
    float controller_3_axisDeadzone = 0.2f;

    unsigned int controller_4_start_buttonIndex = 0;
    unsigned int controller_4_pause_buttonIndex = 7;
    unsigned int controller_4_quit_buttonIndex = 1;
    unsigned int controller_4_left_buttonIndex = 13;
    unsigned int controller_4_right_buttonIndex = 11;
    unsigned int controller_4_up_buttonIndex = 10;
    unsigned int controller_4_down_buttonIndex = 12;
    unsigned int controller_4_grab_buttonIndex = 0;
    unsigned int controller_4_throw_buttonIndex = 5;
    unsigned int controller_4_moveRightLeft_axisIndex = 0;
    unsigned int controller_4_moveDownUp_axisIndex = 1;
    float controller_4_axisDeadzone = 0.2f;

    static constexpr Project001::KeyCode debug_keyboard_toggleDebugCamera_keyCode = Project001::KeyCode::KEY_CODE_0;
    static constexpr Project001::KeyCode debug_keyboard_toggleCameraLock_keyCode = Project001::KeyCode::KEY_CODE_KP_5;
    static constexpr Project001::KeyCode debug_keyboard_setCameraPitch1_keyCode = Project001::KeyCode::KEY_CODE_KP_7;
    static constexpr Project001::KeyCode debug_keyboard_setCameraPitch2_keyCode = Project001::KeyCode::KEY_CODE_KP_9;
    static constexpr Project001::KeyCode debug_keyboard_pitchCameraUp_keyCode = Project001::KeyCode::KEY_CODE_PAGE_DOWN;
    static constexpr Project001::KeyCode debug_keyboard_pitchCameraDown_keyCode = Project001::KeyCode::KEY_CODE_PAGE_UP;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraLeft_keyCode = Project001::KeyCode::KEY_CODE_KP_4;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraRight_keyCode = Project001::KeyCode::KEY_CODE_KP_6;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraUp_keyCode = Project001::KeyCode::KEY_CODE_KP_8;
    static constexpr Project001::KeyCode debug_keyboard_moveCameraDown_keyCode = Project001::KeyCode::KEY_CODE_KP_2;
    static constexpr Project001::KeyCode debug_keyboard_left_keyCode = Project001::KeyCode::KEY_CODE_L;
    static constexpr Project001::KeyCode debug_keyboard_right_keyCode = Project001::KeyCode::KEY_CODE_APOSTROPHE;
    static constexpr Project001::KeyCode debug_keyboard_up_keyCode = Project001::KeyCode::KEY_CODE_P;
    static constexpr Project001::KeyCode debug_keyboard_down_keyCode = Project001::KeyCode::KEY_CODE_SEMICOLON;
    static constexpr Project001::MouseButton debug_mouse_grab_mouseButton = Project001::MouseButton::MOUSE_BUTTON_LEFT;

    bool debug_keyboard_toggleDebugCamera_pressed = false;
    bool debug_keyboard_toggleCameraLock_pressed = false;
    bool debug_keyboard_setCameraPitch1_pressed = false;
    bool debug_keyboard_setCameraPitch2_pressed = false;
    bool debug_keyboard_pitchCameraUp_pressed = false;
    bool debug_keyboard_pitchCameraDown_pressed = false;
    bool debug_keyboard_moveCameraLeft_pressed = false;
    bool debug_keyboard_moveCameraRight_pressed = false;
    bool debug_keyboard_moveCameraUp_pressed = false;
    bool debug_keyboard_moveCameraDown_pressed = false;
    bool debug_keyboard_left_pressed = false;
    bool debug_keyboard_right_pressed = false;
    bool debug_keyboard_up_pressed = false;
    bool debug_keyboard_down_pressed = false;
    bool debug_mouse_grab_pressed = false;
    bool debug_mouse_grab_released = true;

    unsigned int debug_keyboard_toggleDebugCamera_pressCount = 0;
    unsigned int debug_keyboard_toggleCameraLock_pressCount = 0;
    unsigned int debug_keyboard_setCameraPitch1_pressCount = 0;
    unsigned int debug_keyboard_setCameraPitch2_pressCount = 0;
    unsigned int debug_keyboard_pitchCameraUp_pressCount = 0;
    unsigned int debug_keyboard_pitchCameraDown_pressCount = 0;
    unsigned int debug_keyboard_moveCameraLeft_pressCount = 0;
    unsigned int debug_keyboard_moveCameraRight_pressCount = 0;
    unsigned int debug_keyboard_moveCameraUp_pressCount = 0;
    unsigned int debug_keyboard_moveCameraDown_pressCount = 0;
    unsigned int debug_keyboard_left_pressCount = 0;
    unsigned int debug_keyboard_right_pressCount = 0;
    unsigned int debug_keyboard_up_pressCount = 0;
    unsigned int debug_keyboard_down_pressCount = 0;
    unsigned int debug_mouse_grab_pressCount = 0;
    unsigned int debug_mouse_grab_releaseCount = 0;

    // Game Constants ----------------------------------------------------------

    unsigned int randomNumberSeed = 777;

    static constexpr float s_ground_size = 512.0f; // center to top

    static constexpr float s_stageSharkCircleOffset_size = 256.0f;

    static constexpr float s_deadzone_size = 1024.0f;

    static constexpr float s_maxStage_size = 1088.0f;

    static constexpr float s_penguin_collisionRadius = 12.0f;
    static constexpr float s_penguin_grabOffset = 22.0f;
    static constexpr float s_penguin_grabRadius = 18.0f;
    static constexpr float s_penguin_throwSpeed_s = 256.0f;

    static constexpr float s_snowball_landFriction = 16.0f;
    static constexpr float s_snowball_waterFriction = 32.0f;
    static constexpr float s_snowball_angularFriction = glm::pi<float>();

    // Resources ---------------------------------------------------------------

    // Pixel Font Resources
    const Project001::FontData* pixelFont_fontDataPtr = nullptr;
    const Project001::TextureData* pixelFont_textureDataPtr = nullptr;
    unsigned int pixelFont_textureId = static_cast<unsigned int>(-1);

    // General Resources
    Project001::MeshData* circle_meshDataPtr = nullptr;
    Project001::MeshData* orientationArrow_meshDataPtr = nullptr;
    Project001::MeshData* hallowCircle_meshDataPtr = nullptr;

    Project001::MeshData* player_grabZone_meshDataPtr = nullptr;

    static constexpr float s_maxAimLineLength = 4096.0f;
    static constexpr float s_aimLineWidth = 2.0f;
    Project001::MeshData* player1_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player1_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* player2_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player2_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* player3_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player3_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* player4_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* player4_aimRay2_meshDataPtr = nullptr;

    Project001::TextureData* dotted_1_1_textureDataPtr = nullptr;
    unsigned int dotted_1_1_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* dotted_1_3_textureDataPtr = nullptr;
    unsigned int dotted_1_3_textureId = static_cast<unsigned int>(-1);

    // Main Menu Resources
    Project001::MeshData* authorText_meshDataPtr = nullptr;
    Project001::MeshData* introText_meshDataPtr = nullptr;
    Project001::MeshData* startText_meshDataPtr = nullptr;
    Project001::MeshData* titleText_meshDataPtr = nullptr;

    // Cursor Resources
    Project001::MeshData* cursorHandOpen_meshDataPtr = nullptr;
    Project001::MeshData* cursorHandPointer_meshDataPtr = nullptr;
    Project001::MeshData* cursorHandGrab_meshDataPtr = nullptr;
    Project001::TextureData* cursor_textureDataPtr = nullptr;
    unsigned int cursor_textureId = static_cast<unsigned int>(-1);
    Project001::MeshData* cursor_marker_meshDataPtr = nullptr;
    Project001::MeshData* cursor_aimRay1_meshDataPtr = nullptr;
    Project001::MeshData* cursor_aimRay2_meshDataPtr = nullptr;
    Project001::MeshData* cursor_aimRay3_meshDataPtr = nullptr;

    // Stage Resources
    Project001::MeshData* ground_meshDataPtr = nullptr;
    Project001::MeshData* water_meshDataPtr = nullptr;

    Project001::MeshData* deadZone_meshDataPtr = nullptr;
    Project001::TextureData* hazard_textureDataPtr = nullptr;
    unsigned int hazard_textureId = static_cast<unsigned int>(-1);

    Project001::MeshData* pathPoints_meshDataPtr = nullptr;

    // Stage Grid Resources
    Project001::MeshData* stageGrid_meshDataPtr = nullptr;
    unsigned int stageGrid_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* stageGridLabels_meshDataPtr = nullptr;
    unsigned int stageGridLabels_meshId = static_cast<unsigned int>(-1);

    Project001::MeshData* stageCollisionQuadTree_meshDataPtr = nullptr;

    // Actor Resources
    Project001::MeshData* penguin_body_meshDataPtr = nullptr;
    unsigned int penguin_body_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_flipper_right_meshDataPtr = nullptr;
    unsigned int penguin_flipper_right_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_flipper_left_meshDataPtr = nullptr;
    unsigned int penguin_flipper_left_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_foot_right_meshDataPtr = nullptr;
    unsigned int penguin_foot_right_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_foot_left_meshDataPtr = nullptr;
    unsigned int penguin_foot_left_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_head_meshDataPtr = nullptr;
    unsigned int penguin_head_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_eyes_meshDataPtr = nullptr;
    unsigned int penguin_eyes_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_eyes_v2_meshDataPtr = nullptr;
    unsigned int penguin_eyes_v2_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_beak_meshDataPtr = nullptr;
    unsigned int penguin_beak_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_glasses_meshDataPtr = nullptr;
    unsigned int penguin_glasses_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_glasses_v2_meshDataPtr = nullptr;
    unsigned int penguin_glasses_v2_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* penguin_glasses_v3_meshDataPtr = nullptr;
    unsigned int penguin_glasses_v3_meshId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_textureDataPtr = nullptr;
    unsigned int penguin_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_v2_textureDataPtr = nullptr;
    unsigned int penguin_v2_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_v3_textureDataPtr = nullptr;
    unsigned int penguin_v3_textureId = static_cast<unsigned int>(-1);
    Project001::TextureData* penguin_v4_textureDataPtr = nullptr;
    unsigned int penguin_v4_textureId = static_cast<unsigned int>(-1);

    Project001::MeshData* snowball_meshDataPtr = nullptr;
    unsigned int snowball_meshId = static_cast<unsigned int>(-1);

    Project001::MeshData* shark_back_meshDataPtr = nullptr;
    unsigned int shark_back_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* shark_front_meshDataPtr = nullptr;
    unsigned int shark_front_meshId = static_cast<unsigned int>(-1);
    Project001::MeshData* shark_jaw_meshDataPtr = nullptr;
    unsigned int shark_jaw_meshId = static_cast<unsigned int>(-1);
    Project001::TextureData* shark_textureDataPtr = nullptr;
    unsigned int shark_textureId = static_cast<unsigned int>(-1);

    Project001::MeshData* shark_back_collision_meshDataPtr = nullptr;
    Project001::MeshData* shark_front_collision_meshDataPtr = nullptr;
    Project001::MeshData* shark_jaw_collision_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay1_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay2_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay3_meshDataPtr = nullptr;
    Project001::MeshData* shark_attackRay4_meshDataPtr = nullptr;

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