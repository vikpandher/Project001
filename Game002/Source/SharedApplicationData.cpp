// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-07-15

#include "SharedApplicationData.h"

#include "Window.h"



PlayerCreationInfo::ControlScheme PlayerCreationInfo::StringToControlScheme(const std::string& str)
{
    const std::string prefix = "CONTROL_SCHEME_";
    if (str.length() < prefix.length() ||
        str.find(prefix, 0) != 0)
    {
        return ControlScheme::CONTROL_SCHEME_UNKNOWN;
    }

    std::string value = str.substr(prefix.length());

    if (value == "KEYBOARD_1")
    {
        return ControlScheme::CONTROL_SCHEME_KEYBOARD_1;
    }
    else if (value == "KEYBOARD_2")
    {
        return ControlScheme::CONTROL_SCHEME_KEYBOARD_2;
    }
    else if (value == "CONTROLLER_1")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_1;
    }
    else if (value == "CONTROLLER_2")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_2;
    }
    else if (value == "CONTROLLER_3")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_3;
    }
    else if (value == "CONTROLLER_4")
    {
        return ControlScheme::CONTROL_SCHEME_CONTROLLER_4;
    }

    return ControlScheme::CONTROL_SCHEME_UNKNOWN;
}

void SharedApplicationData::UpdateKeyboardButtonPresses(const Project001::KeyEvent& keyEvent)
{
    const Project001::KeyCode& keyCode = keyEvent.keyCode;
    const Project001::ButtonAction& buttonAction = keyEvent.buttonAction;
    const Project001::KeyModifier& keyModifier = keyEvent.keyModifier;

    for (size_t i = 0; i < s_player_count; ++i)
    {
        PlayerCreationInfo& PlayerCreationInfo = playerCreationInfos[i];

        if (PlayerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_1)
        {
            if (keyCode == keyboard_1_start_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_start_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_start_pressed = false;
            }
            else if (keyCode == keyboard_1_pause_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_pause_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_pause_pressed = false;
            }
            else if (keyCode == keyboard_1_quit_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_quit_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_quit_pressed = false;
            }
            else if (keyCode == keyboard_1_left_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_left_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_left_pressed = false;
            }
            else if (keyCode == keyboard_1_right_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_right_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_right_pressed = false;
            }
            else if (keyCode == keyboard_1_up_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_up_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_up_pressed = false;
            }
            else if (keyCode == keyboard_1_down_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_down_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_down_pressed = false;
            }
            else if (keyCode == keyboard_1_grab_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_grab_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_grab_pressed = false;
            }
            else if (keyCode == keyboard_1_throw_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_1_throw_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_1_throw_pressed = false;
            }
        }
        else if (PlayerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_2)
        {
            if (keyCode == keyboard_2_start_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_start_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_start_pressed = false;
            }
            else if (keyCode == keyboard_2_pause_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_pause_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_pause_pressed = false;
            }
            else if (keyCode == keyboard_2_quit_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_quit_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_quit_pressed = false;
            }
            else if (keyCode == keyboard_2_left_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_left_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_left_pressed = false;
            }
            else if (keyCode == keyboard_2_right_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_right_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_right_pressed = false;
            }
            else if (keyCode == keyboard_2_up_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_up_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_up_pressed = false;
            }
            else if (keyCode == keyboard_2_down_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_down_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_down_pressed = false;
            }
            else if (keyCode == keyboard_2_grab_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_grab_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_grab_pressed = false;
            }
            else if (keyCode == keyboard_2_throw_keyCode)
            {
                if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) keyboard_2_throw_pressed = true;
                else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) keyboard_2_throw_pressed = false;
            }
        }
    }

    if (keyCode == debug_keyboard_toggleDebugCamera_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_toggleDebugCamera_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_toggleDebugCamera_pressed = false;
    }
    else if (keyCode == debug_keyboard_toggleCameraLock_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_toggleCameraLock_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_toggleCameraLock_pressed = false;
    }
    else if (keyCode == debug_keyboard_pitchCameraUp_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_pitchCameraUp_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_pitchCameraUp_pressed = false;
    }
    else if (keyCode == debug_keyboard_setCameraPitch1_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_setCameraPitch1_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_setCameraPitch1_pressed = false;
    }
    else if (keyCode == debug_keyboard_setCameraPitch2_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_setCameraPitch2_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_setCameraPitch2_pressed = false;
    }
    else if (keyCode == debug_keyboard_pitchCameraDown_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_pitchCameraDown_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_pitchCameraDown_pressed = false;
    }
    else if (keyCode == debug_keyboard_moveCameraLeft_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraLeft_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraLeft_pressed = false;
    }
    else if (keyCode == debug_keyboard_moveCameraRight_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraRight_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraRight_pressed = false;
    }
    else if (keyCode == debug_keyboard_moveCameraUp_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraUp_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraUp_pressed = false;
    }
    else if (keyCode == debug_keyboard_moveCameraDown_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_moveCameraDown_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_moveCameraDown_pressed = false;
    }
    else if (keyCode == debug_keyboard_left_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_left_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_left_pressed = false;
    }
    else if (keyCode == debug_keyboard_right_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_right_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_right_pressed = false;
    }
    else if (keyCode == debug_keyboard_up_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_up_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_up_pressed = false;
    }
    else if (keyCode == debug_keyboard_down_keyCode)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS) debug_keyboard_down_pressed = true;
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE) debug_keyboard_down_pressed = false;
    }
}

void SharedApplicationData::UpdateMouseButtonPresses(const Project001::MouseButtonEvent& mouseButtonEvent)
{
    const Project001::MouseButton& mouseButton = mouseButtonEvent.mouseButton;
    const Project001::ButtonAction& buttonAction = mouseButtonEvent.buttonAction;
    const Project001::KeyModifier& keyModifier = mouseButtonEvent.keyModifier;

    if (mouseButton == debug_mouse_grab_mouseButton)
    {
        if (buttonAction == Project001::ButtonAction::KEY_ACTION_PRESS)
        {
            debug_mouse_grab_pressed = true;
            debug_mouse_grab_released = false;
        }
        else if (buttonAction == Project001::ButtonAction::KEY_ACTION_RELEASE)
        {
            debug_mouse_grab_pressed = false;
            debug_mouse_grab_released = true;
        }
    }
}

void SharedApplicationData::UpdateButtonPressCounts(const Project001::Window* windowPtr)
{
    for (size_t i = 0; i < s_player_count; ++i)
    {
        PlayerCreationInfo& playerCreationInfo = playerCreationInfos[i];

        if (playerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_1)
        {
            if (keyboard_1_start_pressed) playerCreationInfo.start_pressCount++;
            else playerCreationInfo.start_pressCount = 0;

            if (keyboard_1_pause_pressed) playerCreationInfo.pause_pressCount++;
            else playerCreationInfo.pause_pressCount = 0;

            if (keyboard_1_quit_pressed) playerCreationInfo.quit_pressCount++;
            else playerCreationInfo.quit_pressCount = 0;

            if (keyboard_1_left_pressed) playerCreationInfo.left_pressCount++;
            else playerCreationInfo.left_pressCount = 0;

            if (keyboard_1_right_pressed) playerCreationInfo.right_pressCount++;
            else playerCreationInfo.right_pressCount = 0;

            if (keyboard_1_up_pressed) playerCreationInfo.up_pressCount++;
            else playerCreationInfo.up_pressCount = 0;

            if (keyboard_1_down_pressed) playerCreationInfo.down_pressCount++;
            else playerCreationInfo.down_pressCount = 0;

            if (keyboard_1_grab_pressed) playerCreationInfo.grab_pressCount++;
            else playerCreationInfo.grab_pressCount = 0;

            if (keyboard_1_throw_pressed) playerCreationInfo.throw_pressCount++;
            else playerCreationInfo.throw_pressCount = 0;
        }
        else if (playerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_KEYBOARD_2)
        {
            if (keyboard_2_start_pressed) playerCreationInfo.start_pressCount++;
            else playerCreationInfo.start_pressCount = 0;

            if (keyboard_2_pause_pressed) playerCreationInfo.pause_pressCount++;
            else playerCreationInfo.pause_pressCount = 0;

            if (keyboard_2_quit_pressed) playerCreationInfo.quit_pressCount++;
            else playerCreationInfo.quit_pressCount = 0;

            if (keyboard_2_left_pressed) playerCreationInfo.left_pressCount++;
            else playerCreationInfo.left_pressCount = 0;

            if (keyboard_2_right_pressed) playerCreationInfo.right_pressCount++;
            else playerCreationInfo.right_pressCount = 0;

            if (keyboard_2_up_pressed) playerCreationInfo.up_pressCount++;
            else playerCreationInfo.up_pressCount = 0;

            if (keyboard_2_down_pressed) playerCreationInfo.down_pressCount++;
            else playerCreationInfo.down_pressCount = 0;

            if (keyboard_2_grab_pressed) playerCreationInfo.grab_pressCount++;
            else playerCreationInfo.grab_pressCount = 0;

            if (keyboard_2_throw_pressed) playerCreationInfo.throw_pressCount++;
            else playerCreationInfo.throw_pressCount = 0;
        }
        else if (playerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_1)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(0, buttonValues);

            if (buttonValues.size() > controller_1_start_buttonIndex && buttonValues[controller_1_start_buttonIndex]) playerCreationInfo.start_pressCount++;
            else  playerCreationInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_1_pause_buttonIndex && buttonValues[controller_1_pause_buttonIndex]) playerCreationInfo.pause_pressCount++;
            else playerCreationInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_1_quit_buttonIndex && buttonValues[controller_1_quit_buttonIndex]) playerCreationInfo.quit_pressCount++;
            else playerCreationInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_1_left_buttonIndex && buttonValues[controller_1_left_buttonIndex]) playerCreationInfo.left_pressCount++;
            else playerCreationInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_1_right_buttonIndex && buttonValues[controller_1_right_buttonIndex]) playerCreationInfo.right_pressCount++;
            else playerCreationInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_1_up_buttonIndex && buttonValues[controller_1_up_buttonIndex]) playerCreationInfo.up_pressCount++;
            else playerCreationInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_1_down_buttonIndex && buttonValues[controller_1_down_buttonIndex]) playerCreationInfo.down_pressCount++;
            else playerCreationInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_1_grab_buttonIndex && buttonValues[controller_1_grab_buttonIndex]) playerCreationInfo.grab_pressCount++;
            else playerCreationInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_1_throw_buttonIndex && buttonValues[controller_1_throw_buttonIndex]) playerCreationInfo.throw_pressCount++;
            else playerCreationInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(0, axisValues);

            if (axisValues.size() > controller_1_moveRightLeft_axisIndex) playerCreationInfo.leftRightAxisValue = axisValues[controller_1_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_1_moveDownUp_axisIndex) playerCreationInfo.upDownAxisValue = -axisValues[controller_1_moveDownUp_axisIndex];
        }
        else if (playerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_2)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(1, buttonValues);

            if (buttonValues.size() > controller_2_start_buttonIndex && buttonValues[controller_2_start_buttonIndex]) playerCreationInfo.start_pressCount++;
            else  playerCreationInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_2_pause_buttonIndex && buttonValues[controller_2_pause_buttonIndex]) playerCreationInfo.pause_pressCount++;
            else playerCreationInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_2_quit_buttonIndex && buttonValues[controller_2_quit_buttonIndex]) playerCreationInfo.quit_pressCount++;
            else playerCreationInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_2_left_buttonIndex && buttonValues[controller_2_left_buttonIndex]) playerCreationInfo.left_pressCount++;
            else playerCreationInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_2_right_buttonIndex && buttonValues[controller_2_right_buttonIndex]) playerCreationInfo.right_pressCount++;
            else playerCreationInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_2_up_buttonIndex && buttonValues[controller_2_up_buttonIndex]) playerCreationInfo.up_pressCount++;
            else playerCreationInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_2_down_buttonIndex && buttonValues[controller_2_down_buttonIndex]) playerCreationInfo.down_pressCount++;
            else playerCreationInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_2_grab_buttonIndex && buttonValues[controller_2_grab_buttonIndex]) playerCreationInfo.grab_pressCount++;
            else playerCreationInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_2_throw_buttonIndex && buttonValues[controller_2_throw_buttonIndex]) playerCreationInfo.throw_pressCount++;
            else playerCreationInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(1, axisValues);

            if (axisValues.size() > controller_2_moveRightLeft_axisIndex) playerCreationInfo.leftRightAxisValue = axisValues[controller_2_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_2_moveDownUp_axisIndex) playerCreationInfo.upDownAxisValue = -axisValues[controller_2_moveDownUp_axisIndex];
        }
        else if (playerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_3)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(2, buttonValues);

            if (buttonValues.size() > controller_3_start_buttonIndex && buttonValues[controller_3_start_buttonIndex]) playerCreationInfo.start_pressCount++;
            else  playerCreationInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_3_pause_buttonIndex && buttonValues[controller_3_pause_buttonIndex]) playerCreationInfo.pause_pressCount++;
            else playerCreationInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_3_quit_buttonIndex && buttonValues[controller_3_quit_buttonIndex]) playerCreationInfo.quit_pressCount++;
            else playerCreationInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_3_left_buttonIndex && buttonValues[controller_3_left_buttonIndex]) playerCreationInfo.left_pressCount++;
            else playerCreationInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_3_right_buttonIndex && buttonValues[controller_3_right_buttonIndex]) playerCreationInfo.right_pressCount++;
            else playerCreationInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_3_up_buttonIndex && buttonValues[controller_3_up_buttonIndex]) playerCreationInfo.up_pressCount++;
            else playerCreationInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_3_down_buttonIndex && buttonValues[controller_3_down_buttonIndex]) playerCreationInfo.down_pressCount++;
            else playerCreationInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_3_grab_buttonIndex && buttonValues[controller_3_grab_buttonIndex]) playerCreationInfo.grab_pressCount++;
            else playerCreationInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_3_throw_buttonIndex && buttonValues[controller_3_throw_buttonIndex]) playerCreationInfo.throw_pressCount++;
            else playerCreationInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(2, axisValues);

            if (axisValues.size() > controller_3_moveRightLeft_axisIndex) playerCreationInfo.leftRightAxisValue = axisValues[controller_3_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_3_moveDownUp_axisIndex) playerCreationInfo.upDownAxisValue = -axisValues[controller_3_moveDownUp_axisIndex];
        }
        else if (playerCreationInfo.controlScheme == PlayerCreationInfo::ControlScheme::CONTROL_SCHEME_CONTROLLER_4)
        {
            std::vector<bool> buttonValues;
            windowPtr->GetJoystickButtons(3, buttonValues);

            if (buttonValues.size() > controller_4_start_buttonIndex && buttonValues[controller_4_start_buttonIndex]) playerCreationInfo.start_pressCount++;
            else  playerCreationInfo.start_pressCount = 0;

            if (buttonValues.size() > controller_4_pause_buttonIndex && buttonValues[controller_4_pause_buttonIndex]) playerCreationInfo.pause_pressCount++;
            else playerCreationInfo.pause_pressCount = 0;

            if (buttonValues.size() > controller_4_quit_buttonIndex && buttonValues[controller_4_quit_buttonIndex]) playerCreationInfo.quit_pressCount++;
            else playerCreationInfo.quit_pressCount = 0;

            if (buttonValues.size() > controller_4_left_buttonIndex && buttonValues[controller_4_left_buttonIndex]) playerCreationInfo.left_pressCount++;
            else playerCreationInfo.left_pressCount = 0;

            if (buttonValues.size() > controller_4_right_buttonIndex && buttonValues[controller_4_right_buttonIndex]) playerCreationInfo.right_pressCount++;
            else playerCreationInfo.right_pressCount = 0;

            if (buttonValues.size() > controller_4_up_buttonIndex && buttonValues[controller_4_up_buttonIndex]) playerCreationInfo.up_pressCount++;
            else playerCreationInfo.up_pressCount = 0;

            if (buttonValues.size() > controller_4_down_buttonIndex && buttonValues[controller_4_down_buttonIndex]) playerCreationInfo.down_pressCount++;
            else playerCreationInfo.down_pressCount = 0;

            if (buttonValues.size() > controller_4_grab_buttonIndex && buttonValues[controller_4_grab_buttonIndex]) playerCreationInfo.grab_pressCount++;
            else playerCreationInfo.grab_pressCount = 0;

            if (buttonValues.size() > controller_4_throw_buttonIndex && buttonValues[controller_4_throw_buttonIndex]) playerCreationInfo.throw_pressCount++;
            else playerCreationInfo.throw_pressCount = 0;

            std::vector<float> axisValues;
            windowPtr->GetJoystickAxis(3, axisValues);

            if (axisValues.size() > controller_4_moveRightLeft_axisIndex) playerCreationInfo.leftRightAxisValue = axisValues[controller_4_moveRightLeft_axisIndex];

            if (axisValues.size() > controller_4_moveDownUp_axisIndex) playerCreationInfo.upDownAxisValue = -axisValues[controller_4_moveDownUp_axisIndex];
        }
    }

    if (debug_keyboard_toggleDebugCamera_pressed) debug_keyboard_toggleDebugCamera_pressCount++;
    else debug_keyboard_toggleDebugCamera_pressCount = 0;

    if (debug_keyboard_toggleCameraLock_pressed) debug_keyboard_toggleCameraLock_pressCount++;
    else debug_keyboard_toggleCameraLock_pressCount = 0;

    if (debug_keyboard_setCameraPitch1_pressed) debug_keyboard_setCameraPitch1_pressCount++;
    else debug_keyboard_setCameraPitch1_pressCount = 0;

    if (debug_keyboard_setCameraPitch2_pressed) debug_keyboard_setCameraPitch2_pressCount++;
    else debug_keyboard_setCameraPitch2_pressCount = 0;

    if (debug_keyboard_pitchCameraUp_pressed) debug_keyboard_pitchCameraUp_pressCount++;
    else debug_keyboard_pitchCameraUp_pressCount = 0;

    if (debug_keyboard_pitchCameraDown_pressed) debug_keyboard_pitchCameraDown_pressCount++;
    else debug_keyboard_pitchCameraDown_pressCount = 0;

    if (debug_keyboard_moveCameraLeft_pressed) debug_keyboard_moveCameraLeft_pressCount++;
    else debug_keyboard_moveCameraLeft_pressCount = 0;

    if (debug_keyboard_moveCameraRight_pressed) debug_keyboard_moveCameraRight_pressCount++;
    else debug_keyboard_moveCameraRight_pressCount = 0;

    if (debug_keyboard_moveCameraUp_pressed) debug_keyboard_moveCameraUp_pressCount++;
    else debug_keyboard_moveCameraUp_pressCount = 0;

    if (debug_keyboard_moveCameraDown_pressed) debug_keyboard_moveCameraDown_pressCount++;
    else debug_keyboard_moveCameraDown_pressCount = 0;

    if (debug_keyboard_left_pressed) debug_keyboard_left_pressCount++;
    else debug_keyboard_left_pressCount = 0;

    if (debug_keyboard_right_pressed) debug_keyboard_right_pressCount++;
    else debug_keyboard_right_pressCount = 0;

    if (debug_keyboard_up_pressed) debug_keyboard_up_pressCount++;
    else debug_keyboard_up_pressCount = 0;

    if (debug_keyboard_down_pressed) debug_keyboard_down_pressCount++;
    else debug_keyboard_down_pressCount = 0;

    if (debug_mouse_grab_pressed) debug_mouse_grab_pressCount++;
    else debug_mouse_grab_pressCount = 0;

    if (debug_mouse_grab_released) debug_mouse_grab_releaseCount++;
    else debug_mouse_grab_releaseCount = 0;
}