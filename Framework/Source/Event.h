// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2026-08-18

#pragma once

#include <string>



namespace Project001
{
    enum class EventType
    {
        EVENT_TYPE_NONE = 0,

        EVENT_TYPE_KEY,
        EVENT_TYPE_MOUSE_BUTTON,
        EVENT_TYPE_CURSOR_POSITION,
        EVENT_TYPE_CURSOR_ENTER,
        EVENT_TYPE_SCROLL,
        EVENT_TYPE_WINDOW_CLOSE,
        EVENT_TYPE_WINDOW_FOCUS,
        EVENT_TYPE_WINDOW_SIZE,
        EVENT_TYPE_FRAMEBUFFER_SIZE,

        EVENT_TYPE_SWITCH_SCENE,
        EVENT_TYPE_INITIALIZE_SCENE,
        EVENT_TYPE_DEINITIALIZE_SCENE,
        EVENT_TYPE_UPDATE,
        EVENT_TYPE_RENDER
    };

    static std::string EventTypeToString(EventType eventType)
    {
        switch (eventType)
        {
        case EventType::EVENT_TYPE_NONE:
        {
            return std::string("EVENT_TYPE_NONE");
        }
        case EventType::EVENT_TYPE_KEY:
        {
            return std::string("EVENT_TYPE_KEY");
        }
        case EventType::EVENT_TYPE_MOUSE_BUTTON:
        {
            return std::string("EVENT_TYPE_MOUSE_BUTTON");
        }
        case EventType::EVENT_TYPE_CURSOR_POSITION:
        {
            return std::string("EVENT_TYPE_CURSOR_POSITION");
        }
        case EventType::EVENT_TYPE_CURSOR_ENTER:
        {
            return std::string("EVENT_TYPE_CURSOR_ENTER");
        }
        case EventType::EVENT_TYPE_SCROLL:
        {
            return std::string("EVENT_TYPE_SCROLL");
        }
        case EventType::EVENT_TYPE_WINDOW_CLOSE:
        {
            return std::string("EVENT_TYPE_WINDOW_CLOSE");
        }
        case EventType::EVENT_TYPE_WINDOW_FOCUS:
        {
            return std::string("EVENT_TYPE_WINDOW_FOCUS");
        }
        case EventType::EVENT_TYPE_WINDOW_SIZE:
        {
            return std::string("EVENT_TYPE_WINDOW_SIZE");
        }
        case EventType::EVENT_TYPE_FRAMEBUFFER_SIZE:
        {
            return std::string("EVENT_TYPE_FRAMEBUFFER_SIZE");
        }
        case EventType::EVENT_TYPE_SWITCH_SCENE:
        {
            return std::string("EVENT_TYPE_SWITCH_SCENE");
        }
        case EventType::EVENT_TYPE_INITIALIZE_SCENE:
        {
            return std::string("EVENT_TYPE_INITIALIZE_SCENE");
        }
        case EventType::EVENT_TYPE_DEINITIALIZE_SCENE:
        {
            return std::string("EVENT_TYPE_DEINITIALIZE_SCENE");
        }
        case EventType::EVENT_TYPE_UPDATE:
        {
            return std::string("EVENT_TYPE_UPDATE");
        }
        }
        return std::string("UNKNOWN");
    }

    // same as the GLFW key actions
    enum class ButtonAction
    {
        KEY_ACTION_RELEASE = 0,
        KEY_ACTION_PRESS = 1,
        KEY_ACTION_REPEAT = 2
    };

    static std::string ButtonActionToString(ButtonAction buttonAction)
    {
        switch (buttonAction)
        {
        case ButtonAction::KEY_ACTION_RELEASE:
        {
            return std::string("KEY_ACTION_RELEASE");
        }
        case ButtonAction::KEY_ACTION_PRESS:
        {
            return std::string("KEY_ACTION_PRESS");
        }
        case ButtonAction::KEY_ACTION_REPEAT:
        {
            return std::string("KEY_ACTION_REPEAT");
        }
        }
        return std::string("UNKNOWN");
    }

    // same as the GLFW keboard key codes
    enum class KeyCode
    {
        KEY_CODE_UNKNOWN = -1,

        KEY_CODE_SPACE = 32,

        KEY_CODE_APOSTROPHE = 39, /* ' */

        KEY_CODE_COMMA = 44, /* , */
        KEY_CODE_MINUS = 45, /* - */
        KEY_CODE_PERIOD = 46, /* . */
        KEY_CODE_SLASH = 47, /* / */
        KEY_CODE_0 = 48,
        KEY_CODE_1 = 49,
        KEY_CODE_2 = 50,
        KEY_CODE_3 = 51,
        KEY_CODE_4 = 52,
        KEY_CODE_5 = 53,
        KEY_CODE_6 = 54,
        KEY_CODE_7 = 55,
        KEY_CODE_8 = 56,
        KEY_CODE_9 = 57,

        KEY_CODE_SEMICOLON = 59, /* ; */

        KEY_CODE_EQUAL = 61, /* = */

        KEY_CODE_A = 65,
        KEY_CODE_B = 66,
        KEY_CODE_C = 67,
        KEY_CODE_D = 68,
        KEY_CODE_E = 69,
        KEY_CODE_F = 70,
        KEY_CODE_G = 71,
        KEY_CODE_H = 72,
        KEY_CODE_I = 73,
        KEY_CODE_J = 74,
        KEY_CODE_K = 75,
        KEY_CODE_L = 76,
        KEY_CODE_M = 77,
        KEY_CODE_N = 78,
        KEY_CODE_O = 79,
        KEY_CODE_P = 80,
        KEY_CODE_Q = 81,
        KEY_CODE_R = 82,
        KEY_CODE_S = 83,
        KEY_CODE_T = 84,
        KEY_CODE_U = 85,
        KEY_CODE_V = 86,
        KEY_CODE_W = 87,
        KEY_CODE_X = 88,
        KEY_CODE_Y = 89,
        KEY_CODE_Z = 90,
        KEY_CODE_LEFT_BRACKET = 91, /* [ */
        KEY_CODE_BACKSLASH = 92, /* \ */
        KEY_CODE_RIGHT_BRACKET = 93, /* ] */

        KEY_CODE_GRAVE_ACCENT = 96, /* ` */

        KEY_CODE_WORLD_1 = 161, /* non-US #1 */
        KEY_CODE_WORLD_2 = 162, /* non-US #2 */

        KEY_CODE_ESCAPE = 256,
        KEY_CODE_ENTER = 257,
        KEY_CODE_TAB = 258,
        KEY_CODE_BACKSPACE = 259,
        KEY_CODE_INSERT = 260,
        KEY_CODE_DELETE = 261,
        KEY_CODE_RIGHT = 262,
        KEY_CODE_LEFT = 263,
        KEY_CODE_DOWN = 264,
        KEY_CODE_UP = 265,
        KEY_CODE_PAGE_UP = 266,
        KEY_CODE_PAGE_DOWN = 267,
        KEY_CODE_HOME = 268,
        KEY_CODE_END = 269,

        KEY_CODE_CAPS_LOCK = 280,
        KEY_CODE_SCROLL_LOCK = 281,
        KEY_CODE_NUM_LOCK = 282,
        KEY_CODE_PRINT_SCREEN = 283,
        KEY_CODE_PAUSE = 284,

        KEY_CODE_F1 = 290,
        KEY_CODE_F2 = 291,
        KEY_CODE_F3 = 292,
        KEY_CODE_F4 = 293,
        KEY_CODE_F5 = 294,
        KEY_CODE_F6 = 295,
        KEY_CODE_F7 = 296,
        KEY_CODE_F8 = 297,
        KEY_CODE_F9 = 298,
        KEY_CODE_F10 = 299,
        KEY_CODE_F11 = 300,
        KEY_CODE_F12 = 301,
        KEY_CODE_F13 = 302,
        KEY_CODE_F14 = 303,
        KEY_CODE_F15 = 304,
        KEY_CODE_F16 = 305,
        KEY_CODE_F17 = 306,
        KEY_CODE_F18 = 307,
        KEY_CODE_F19 = 308,
        KEY_CODE_F20 = 309,
        KEY_CODE_F21 = 310,
        KEY_CODE_F22 = 311,
        KEY_CODE_F23 = 312,
        KEY_CODE_F24 = 313,
        KEY_CODE_F25 = 314,

        KEY_CODE_KP_0 = 320,
        KEY_CODE_KP_1 = 321,
        KEY_CODE_KP_2 = 322,
        KEY_CODE_KP_3 = 323,
        KEY_CODE_KP_4 = 324,
        KEY_CODE_KP_5 = 325,
        KEY_CODE_KP_6 = 326,
        KEY_CODE_KP_7 = 327,
        KEY_CODE_KP_8 = 328,
        KEY_CODE_KP_9 = 329,
        KEY_CODE_KP_DECIMAL = 330,
        KEY_CODE_KP_DIVIDE = 331,
        KEY_CODE_KP_MULTIPLY = 332,
        KEY_CODE_KP_SUBTRACT = 333,
        KEY_CODE_KP_ADD = 334,
        KEY_CODE_KP_ENTER = 335,
        KEY_CODE_KP_EQUAL = 336,

        KEY_CODE_LEFT_SHIFT = 340,
        KEY_CODE_LEFT_CONTROL = 341,
        KEY_CODE_LEFT_ALT = 342,
        KEY_CODE_LEFT_SUPER = 343,
        KEY_CODE_RIGHT_SHIFT = 344,
        KEY_CODE_RIGHT_CONTROL = 345,
        KEY_CODE_RIGHT_ALT = 346,
        KEY_CODE_RIGHT_SUPER = 347,
        KEY_CODE_MENU = 348
    };

    static std::string KeyCodeToString(KeyCode keyCode, bool includePrefix = true)
    {
        std::string returnString;
        if (includePrefix)
        {
            returnString = "KEY_CODE_";
        }

        switch (keyCode)
        {
        case KeyCode::KEY_CODE_UNKNOWN:
        {
            returnString += "UNKNOWN";
            break;
        }
        case KeyCode::KEY_CODE_SPACE:
        {
            returnString += "SPACE";
            break;
        }
        case KeyCode::KEY_CODE_APOSTROPHE:
        {
            returnString += "APOSTROPHE";
            break;
        }
        case KeyCode::KEY_CODE_COMMA:
        {
            returnString += "COMMA";
            break;
        }
        case KeyCode::KEY_CODE_MINUS:
        {
            returnString += "MINUS";
            break;
        }
        case KeyCode::KEY_CODE_PERIOD:
        {
            returnString += "PERIOD";
            break;
        }
        case KeyCode::KEY_CODE_SLASH:
        {
            returnString += "SLASH";
            break;
        }
        case KeyCode::KEY_CODE_0:
        {
            returnString += "0";
            break;
        }
        case KeyCode::KEY_CODE_1:
        {
            returnString += "1";
            break;
        }
        case KeyCode::KEY_CODE_2:
        {
            returnString += "2";
            break;
        }
        case KeyCode::KEY_CODE_3:
        {
            returnString += "3";
            break;
        }
        case KeyCode::KEY_CODE_4:
        {
            returnString += "4";
            break;
        }
        case KeyCode::KEY_CODE_5:
        {
            returnString += "5";
            break;
        }
        case KeyCode::KEY_CODE_6:
        {
            returnString += "6";
            break;
        }
        case KeyCode::KEY_CODE_7:
        {
            returnString += "7";
            break;
        }
        case KeyCode::KEY_CODE_8:
        {
            returnString += "8";
            break;
        }
        case KeyCode::KEY_CODE_9:
        {
            returnString += "9";
            break;
        }
        case KeyCode::KEY_CODE_SEMICOLON:
        {
            returnString += "SEMICOLON";
            break;
        }
        case KeyCode::KEY_CODE_EQUAL:
        {
            returnString += "EQUAL";
            break;
        }
        case KeyCode::KEY_CODE_A:
        {
            returnString += "A";
            break;
        }
        case KeyCode::KEY_CODE_B:
        {
            returnString += "B";
            break;
        }
        case KeyCode::KEY_CODE_C:
        {
            returnString += "C";
            break;
        }
        case KeyCode::KEY_CODE_D:
        {
            returnString += "D";
            break;
        }
        case KeyCode::KEY_CODE_E:
        {
            returnString += "E";
            break;
        }
        case KeyCode::KEY_CODE_F:
        {
            returnString += "F";
            break;
        }
        case KeyCode::KEY_CODE_G:
        {
            returnString += "G";
            break;
        }
        case KeyCode::KEY_CODE_H:
        {
            returnString += "H";
            break;
        }
        case KeyCode::KEY_CODE_I:
        {
            returnString += "I";
            break;
        }
        case KeyCode::KEY_CODE_J:
        {
            returnString += "J";
            break;
        }
        case KeyCode::KEY_CODE_K:
        {
            returnString += "K";
            break;
        }
        case KeyCode::KEY_CODE_L:
        {
            returnString += "L";
            break;
        }
        case KeyCode::KEY_CODE_M:
        {
            returnString += "M";
            break;
        }
        case KeyCode::KEY_CODE_N:
        {
            returnString += "N";
            break;
        }
        case KeyCode::KEY_CODE_O:
        {
            returnString += "O";
            break;
        }
        case KeyCode::KEY_CODE_P:
        {
            returnString += "P";
            break;
        }
        case KeyCode::KEY_CODE_Q:
        {
            returnString += "Q";
            break;
        }
        case KeyCode::KEY_CODE_R:
        {
            returnString += "R";
            break;
        }
        case KeyCode::KEY_CODE_S:
        {
            returnString += "S";
            break;
        }
        case KeyCode::KEY_CODE_T:
        {
            returnString += "T";
            break;
        }
        case KeyCode::KEY_CODE_U:
        {
            returnString += "U";
            break;
        }
        case KeyCode::KEY_CODE_V:
        {
            returnString += "V";
            break;
        }
        case KeyCode::KEY_CODE_W:
        {
            returnString += "W";
            break;
        }
        case KeyCode::KEY_CODE_X:
        {
            returnString += "X";
            break;
        }
        case KeyCode::KEY_CODE_Y:
        {
            returnString += "Y";
            break;
        }
        case KeyCode::KEY_CODE_Z:
        {
            returnString += "Z";
            break;
        }
        case KeyCode::KEY_CODE_LEFT_BRACKET:
        {
            returnString += "LEFT_BRACKET";
            break;
        }
        case KeyCode::KEY_CODE_BACKSLASH:
        {
            returnString += "BACKSLASH";
            break;
        }
        case KeyCode::KEY_CODE_RIGHT_BRACKET:
        {
            returnString += "RIGHT_BRACKET";
            break;
        }
        case KeyCode::KEY_CODE_GRAVE_ACCENT:
        {
            returnString += "GRAVE_ACCENT";
            break;
        }
        case KeyCode::KEY_CODE_WORLD_1:
        {
            returnString += "WORLD_1";
            break;
        }
        case KeyCode::KEY_CODE_WORLD_2:
        {
            returnString += "WORLD_2";
            break;
        }
        case KeyCode::KEY_CODE_ESCAPE:
        {
            returnString += "ESCAPE";
            break;
        }
        case KeyCode::KEY_CODE_ENTER:
        {
            returnString += "ENTER";
            break;
        }
        case KeyCode::KEY_CODE_TAB:
        {
            returnString += "TAB";
            break;
        }
        case KeyCode::KEY_CODE_BACKSPACE:
        {
            returnString += "BACKSPACE";
            break;
        }
        case KeyCode::KEY_CODE_INSERT:
        {
            returnString += "INSERT";
            break;
        }
        case KeyCode::KEY_CODE_DELETE:
        {
            returnString += "DELETE";
            break;
        }
        case KeyCode::KEY_CODE_RIGHT:
        {
            returnString += "RIGHT";
            break;
        }
        case KeyCode::KEY_CODE_LEFT:
        {
            returnString += "LEFT";
            break;
        }
        case KeyCode::KEY_CODE_DOWN:
        {
            returnString += "DOWN";
            break;
        }
        case KeyCode::KEY_CODE_UP:
        {
            returnString += "UP";
            break;
        }
        case KeyCode::KEY_CODE_PAGE_UP:
        {
            returnString += "PAGE_UP";
            break;
        }
        case KeyCode::KEY_CODE_PAGE_DOWN:
        {
            returnString += "PAGE_DOWN";
            break;
        }
        case KeyCode::KEY_CODE_HOME:
        {
            returnString += "HOME";
            break;
        }
        case KeyCode::KEY_CODE_END:
        {
            returnString += "END";
            break;
        }
        case KeyCode::KEY_CODE_CAPS_LOCK:
        {
            returnString += "CAPS_LOCK";
            break;
        }
        case KeyCode::KEY_CODE_SCROLL_LOCK:
        {
            returnString += "SCROLL_LOCK";
            break;
        }
        case KeyCode::KEY_CODE_NUM_LOCK:
        {
            returnString += "NUM_LOCK";
            break;
        }
        case KeyCode::KEY_CODE_PRINT_SCREEN:
        {
            returnString += "PRINT_SCREEN";
            break;
        }
        case KeyCode::KEY_CODE_PAUSE:
        {
            returnString += "PAUSE";
            break;
        }
        case KeyCode::KEY_CODE_F1:
        {
            returnString += "F1";
            break;
        }
        case KeyCode::KEY_CODE_F2:
        {
            returnString += "F2";
            break;
        }
        case KeyCode::KEY_CODE_F3:
        {
            returnString += "F3";
            break;
        }
        case KeyCode::KEY_CODE_F4:
        {
            returnString += "F4";
            break;
        }
        case KeyCode::KEY_CODE_F5:
        {
            returnString += "F5";
            break;
        }
        case KeyCode::KEY_CODE_F6:
        {
            returnString += "F6";
            break;
        }
        case KeyCode::KEY_CODE_F7:
        {
            returnString += "F7";
            break;
        }
        case KeyCode::KEY_CODE_F8:
        {
            returnString += "F8";
            break;
        }
        case KeyCode::KEY_CODE_F9:
        {
            returnString += "F9";
            break;
        }
        case KeyCode::KEY_CODE_F10:
        {
            returnString += "F10";
            break;
        }
        case KeyCode::KEY_CODE_F11:
        {
            returnString += "F11";
            break;
        }
        case KeyCode::KEY_CODE_F12:
        {
            returnString += "F12";
            break;
        }
        case KeyCode::KEY_CODE_F13:
        {
            returnString += "F13";
            break;
        }
        case KeyCode::KEY_CODE_F14:
        {
            returnString += "F14";
            break;
        }
        case KeyCode::KEY_CODE_F15:
        {
            returnString += "F15";
            break;
        }
        case KeyCode::KEY_CODE_F16:
        {
            returnString += "F16";
            break;
        }
        case KeyCode::KEY_CODE_F17:
        {
            returnString += "F17";
            break;
        }
        case KeyCode::KEY_CODE_F18:
        {
            returnString += "F18";
            break;
        }
        case KeyCode::KEY_CODE_F19:
        {
            returnString += "F19";
            break;
        }
        case KeyCode::KEY_CODE_F20:
        {
            returnString += "F20";
            break;
        }
        case KeyCode::KEY_CODE_F21:
        {
            returnString += "F21";
            break;
        }
        case KeyCode::KEY_CODE_F22:
        {
            returnString += "F22";
            break;
        }
        case KeyCode::KEY_CODE_F23:
        {
            returnString += "F23";
            break;
        }
        case KeyCode::KEY_CODE_F24:
        {
            returnString += "F24";
            break;
        }
        case KeyCode::KEY_CODE_F25:
        {
            returnString += "F25";
            break;
        }
        case KeyCode::KEY_CODE_KP_0:
        {
            returnString += "KP_0";
            break;
        }
        case KeyCode::KEY_CODE_KP_1:
        {
            returnString += "KP_1";
            break;
        }
        case KeyCode::KEY_CODE_KP_2:
        {
            returnString += "KP_2";
            break;
        }
        case KeyCode::KEY_CODE_KP_3:
        {
            returnString += "KP_3";
            break;
        }
        case KeyCode::KEY_CODE_KP_4:
        {
            returnString += "KP_4";
            break;
        }
        case KeyCode::KEY_CODE_KP_5:
        {
            returnString += "KP_5";
            break;
        }
        case KeyCode::KEY_CODE_KP_6:
        {
            returnString += "KP_6";
            break;
        }
        case KeyCode::KEY_CODE_KP_7:
        {
            returnString += "KP_7";
            break;
        }
        case KeyCode::KEY_CODE_KP_8:
        {
            returnString += "KP_8";
            break;
        }
        case KeyCode::KEY_CODE_KP_9:
        {
            returnString += "KP_9";
            break;
        }
        case KeyCode::KEY_CODE_KP_DECIMAL:
        {
            returnString += "KP_DECIMAL";
            break;
        }
        case KeyCode::KEY_CODE_KP_DIVIDE:
        {
            returnString += "KP_DIVIDE";
            break;
        }
        case KeyCode::KEY_CODE_KP_MULTIPLY:
        {
            returnString += "KP_MULTIPLY";
            break;
        }
        case KeyCode::KEY_CODE_KP_SUBTRACT:
        {
            returnString += "KP_SUBTRACT";
            break;
        }
        case KeyCode::KEY_CODE_KP_ADD:
        {
            returnString += "KP_ADD";
            break;
        }
        case KeyCode::KEY_CODE_KP_ENTER:
        {
            returnString += "KP_ENTER";
            break;
        }
        case KeyCode::KEY_CODE_KP_EQUAL:
        {
            returnString += "KP_EQUAL";
            break;
        }
        case KeyCode::KEY_CODE_LEFT_SHIFT:
        {
            returnString += "LEFT_SHIFT";
            break;
        }
        case KeyCode::KEY_CODE_LEFT_CONTROL:
        {
            returnString += "LEFT_CONTROL";
            break;
        }
        case KeyCode::KEY_CODE_LEFT_ALT:
        {
            returnString += "LEFT_ALT";
            break;
        }
        case KeyCode::KEY_CODE_LEFT_SUPER:
        {
            returnString += "LEFT_SUPER";
            break;
        }
        case KeyCode::KEY_CODE_RIGHT_SHIFT:
        {
            returnString += "RIGHT_SHIFT";
            break;
        }
        case KeyCode::KEY_CODE_RIGHT_CONTROL:
        {
            returnString += "RIGHT_CONTROL";
            break;
        }
        case KeyCode::KEY_CODE_RIGHT_ALT:
        {
            returnString += "RIGHT_ALT";
            break;
        }
        case KeyCode::KEY_CODE_RIGHT_SUPER:
        {
            returnString += "RIGHT_SUPER";
            break;
        }
        case KeyCode::KEY_CODE_MENU:
        {
            returnString += "MENU";
            break;
        }
        default:
        {
            returnString += "UNKNOWN";
            break;
        }
        }

        return returnString;
    }

    static KeyCode StringToKeyCode(const std::string& keyString)
    {
        const std::string prefix = "KEY_CODE_";
        if (keyString.length() < prefix.length() ||
            keyString.find(prefix, 0) != 0)
        {
            return KeyCode::KEY_CODE_UNKNOWN;
        }

        std::string value = keyString.substr(prefix.length());

        if (value == "SPACE")
        {
            return KeyCode::KEY_CODE_SPACE;
        }
        else if (value == "APOSTROPHE")
        {
            return KeyCode::KEY_CODE_APOSTROPHE;
        }
        else if (value == "COMMA")
        {
            return KeyCode::KEY_CODE_COMMA;
        }
        else if (value == "MINUS")
        {
            return KeyCode::KEY_CODE_MINUS;
        }
        else if (value == "PERIOD")
        {
            return KeyCode::KEY_CODE_PERIOD;
        }
        else if (value == "SLASH")
        {
            return KeyCode::KEY_CODE_SLASH;
        }
        else if (value == "0")
        {
            return KeyCode::KEY_CODE_0;
        }
        else if (value == "1")
        {
            return KeyCode::KEY_CODE_1;
        }
        else if (value == "2")
        {
            return KeyCode::KEY_CODE_2;
        }
        else if (value == "3")
        {
            return KeyCode::KEY_CODE_3;
        }
        else if (value == "4")
        {
            return KeyCode::KEY_CODE_4;
        }
        else if (value == "5")
        {
            return KeyCode::KEY_CODE_5;
        }
        else if (value == "6")
        {
            return KeyCode::KEY_CODE_6;
        }
        else if (value == "7")
        {
            return KeyCode::KEY_CODE_7;
        }
        else if (value == "8")
        {
            return KeyCode::KEY_CODE_8;
        }
        else if (value == "9")
        {
            return KeyCode::KEY_CODE_9;
        }
        else if (value == "SEMICOLON")
        {
            return KeyCode::KEY_CODE_SEMICOLON;
        }
        else if (value == "EQUAL")
        {
            return KeyCode::KEY_CODE_EQUAL;
        }
        else if (value == "A")
        {
            return KeyCode::KEY_CODE_A;
        }
        else if (value == "B")
        {
            return KeyCode::KEY_CODE_B;
        }
        else if (value == "C")
        {
            return KeyCode::KEY_CODE_C;
        }
        else if (value == "D")
        {
            return KeyCode::KEY_CODE_D;
        }
        else if (value == "E")
        {
            return KeyCode::KEY_CODE_E;
        }
        else if (value == "F")
        {
            return KeyCode::KEY_CODE_F;
        }
        else if (value == "G")
        {
            return KeyCode::KEY_CODE_G;
        }
        else if (value == "H")
        {
            return KeyCode::KEY_CODE_H;
        }
        else if (value == "I")
        {
            return KeyCode::KEY_CODE_I;
        }
        else if (value == "J")
        {
            return KeyCode::KEY_CODE_J;
        }
        else if (value == "K")
        {
            return KeyCode::KEY_CODE_K;
        }
        else if (value == "L")
        {
            return KeyCode::KEY_CODE_L;
        }
        else if (value == "M")
        {
            return KeyCode::KEY_CODE_M;
        }
        else if (value == "N")
        {
            return KeyCode::KEY_CODE_N;
        }
        else if (value == "O")
        {
            return KeyCode::KEY_CODE_O;
        }
        else if (value == "P")
        {
            return KeyCode::KEY_CODE_P;
        }
        else if (value == "Q")
        {
            return KeyCode::KEY_CODE_Q;
        }
        else if (value == "R")
        {
            return KeyCode::KEY_CODE_R;
        }
        else if (value == "S")
        {
            return KeyCode::KEY_CODE_S;
        }
        else if (value == "T")
        {
            return KeyCode::KEY_CODE_T;
        }
        else if (value == "U")
        {
            return KeyCode::KEY_CODE_U;
        }
        else if (value == "V")
        {
            return KeyCode::KEY_CODE_V;
        }
        else if (value == "W")
        {
            return KeyCode::KEY_CODE_W;
        }
        else if (value == "X")
        {
            return KeyCode::KEY_CODE_X;
        }
        else if (value == "Y")
        {
            return KeyCode::KEY_CODE_Y;
        }
        else if (value == "Z")
        {
            return KeyCode::KEY_CODE_Z;
        }
        else if (value == "LEFT_BRACKET")
        {
            return KeyCode::KEY_CODE_LEFT_BRACKET;
        }
        else if (value == "BACKSLASH")
        {
            return KeyCode::KEY_CODE_BACKSLASH;
        }
        else if (value == "RIGHT_BRACKET")
        {
            return KeyCode::KEY_CODE_RIGHT_BRACKET;
        }
        else if (value == "GRAVE_ACCENT")
        {
            return KeyCode::KEY_CODE_GRAVE_ACCENT;
        }
        else if (value == "WORLD_1")
        {
            return KeyCode::KEY_CODE_WORLD_1;
        }
        else if (value == "WORLD_2")
        {
            return KeyCode::KEY_CODE_WORLD_2;
        }
        else if (value == "ESCAPE")
        {
            return KeyCode::KEY_CODE_ESCAPE;
        }
        else if (value == "ENTER")
        {
            return KeyCode::KEY_CODE_ENTER;
        }
        else if (value == "TAB")
        {
            return KeyCode::KEY_CODE_TAB;
        }
        else if (value == "BACKSPACE")
        {
            return KeyCode::KEY_CODE_BACKSPACE;
        }
        else if (value == "INSERT")
        {
            return KeyCode::KEY_CODE_INSERT;
        }
        else if (value == "DELETE")
        {
            return KeyCode::KEY_CODE_DELETE;
        }
        else if (value == "RIGHT")
        {
            return KeyCode::KEY_CODE_RIGHT;
        }
        else if (value == "LEFT")
        {
            return KeyCode::KEY_CODE_LEFT;
        }
        else if (value == "DOWN")
        {
            return KeyCode::KEY_CODE_DOWN;
        }
        else if (value == "UP")
        {
            return KeyCode::KEY_CODE_UP;
        }
        else if (value == "PAGE_UP")
        {
            return KeyCode::KEY_CODE_PAGE_UP;
        }
        else if (value == "PAGE_DOWN")
        {
            return KeyCode::KEY_CODE_PAGE_DOWN;
        }
        else if (value == "HOME")
        {
            return KeyCode::KEY_CODE_HOME;
        }
        else if (value == "END")
        {
            return KeyCode::KEY_CODE_END;
        }
        else if (value == "CAPS_LOCK")
        {
            return KeyCode::KEY_CODE_CAPS_LOCK;
        }
        else if (value == "SCROLL_LOCK")
        {
            return KeyCode::KEY_CODE_SCROLL_LOCK;
        }
        else if (value == "NUM_LOCK")
        {
            return KeyCode::KEY_CODE_NUM_LOCK;
        }
        else if (value == "PRINT_SCREEN")
        {
            return KeyCode::KEY_CODE_PRINT_SCREEN;
        }
        else if (value == "PAUSE")
        {
            return KeyCode::KEY_CODE_PAUSE;
        }
        else if (value == "F1")
        {
            return KeyCode::KEY_CODE_F1;
        }
        else if (value == "F2")
        {
            return KeyCode::KEY_CODE_F2;
        }
        else if (value == "F3")
        {
            return KeyCode::KEY_CODE_F3;
        }
        else if (value == "F4")
        {
            return KeyCode::KEY_CODE_F4;
        }
        else if (value == "F5")
        {
            return KeyCode::KEY_CODE_F5;
        }
        else if (value == "F6")
        {
            return KeyCode::KEY_CODE_F6;
        }
        else if (value == "F7")
        {
            return KeyCode::KEY_CODE_F7;
        }
        else if (value == "F8")
        {
            return KeyCode::KEY_CODE_F8;
        }
        else if (value == "F9")
        {
            return KeyCode::KEY_CODE_F9;
        }
        else if (value == "F10")
        {
            return KeyCode::KEY_CODE_F10;
        }
        else if (value == "F11")
        {
            return KeyCode::KEY_CODE_F11;
        }
        else if (value == "F12")
        {
            return KeyCode::KEY_CODE_F12;
        }
        else if (value == "F13")
        {
            return KeyCode::KEY_CODE_F13;
        }
        else if (value == "F14")
        {
            return KeyCode::KEY_CODE_F14;
        }
        else if (value == "F15")
        {
            return KeyCode::KEY_CODE_F15;
        }
        else if (value == "F16")
        {
            return KeyCode::KEY_CODE_F16;
        }
        else if (value == "F17")
        {
            return KeyCode::KEY_CODE_F17;
        }
        else if (value == "F18")
        {
            return KeyCode::KEY_CODE_F18;
        }
        else if (value == "F19")
        {
            return KeyCode::KEY_CODE_F19;
        }
        else if (value == "F20")
        {
            return KeyCode::KEY_CODE_F20;
        }
        else if (value == "F21")
        {
            return KeyCode::KEY_CODE_F21;
        }
        else if (value == "F22")
        {
            return KeyCode::KEY_CODE_F22;
        }
        else if (value == "F23")
        {
            return KeyCode::KEY_CODE_F23;
        }
        else if (value == "F24")
        {
            return KeyCode::KEY_CODE_F24;
        }
        else if (value == "F25")
        {
            return KeyCode::KEY_CODE_F25;
        }
        else if (value == "KP_0")
        {
            return KeyCode::KEY_CODE_KP_0;
        }
        else if (value == "KP_1")
        {
            return KeyCode::KEY_CODE_KP_1;
        }
        else if (value == "KP_2")
        {
            return KeyCode::KEY_CODE_KP_2;
        }
        else if (value == "KP_3")
        {
            return KeyCode::KEY_CODE_KP_3;
        }
        else if (value == "KP_4")
        {
            return KeyCode::KEY_CODE_KP_4;
        }
        else if (value == "KP_5")
        {
            return KeyCode::KEY_CODE_KP_5;
        }
        else if (value == "KP_6")
        {
            return KeyCode::KEY_CODE_KP_6;
        }
        else if (value == "KP_7")
        {
            return KeyCode::KEY_CODE_KP_7;
        }
        else if (value == "KP_8")
        {
            return KeyCode::KEY_CODE_KP_8;
        }
        else if (value == "KP_9")
        {
            return KeyCode::KEY_CODE_KP_9;
        }
        else if (value == "KP_DECIMAL")
        {
            return KeyCode::KEY_CODE_KP_DECIMAL;
        }
        else if (value == "KP_DIVIDE")
        {
            return KeyCode::KEY_CODE_KP_DIVIDE;
        }
        else if (value == "KP_MULTIPLY")
        {
            return KeyCode::KEY_CODE_KP_MULTIPLY;
        }
        else if (value == "KP_SUBTRACT")
        {
            return KeyCode::KEY_CODE_KP_SUBTRACT;
        }
        else if (value == "KP_ADD")
        {
            return KeyCode::KEY_CODE_KP_ADD;
        }
        else if (value == "KP_ENTER")
        {
            return KeyCode::KEY_CODE_KP_ENTER;
        }
        else if (value == "KP_EQUAL")
        {
            return KeyCode::KEY_CODE_KP_EQUAL;
        }
        else if (value == "LEFT_SHIFT")
        {
            return KeyCode::KEY_CODE_LEFT_SHIFT;
        }
        else if (value == "LEFT_CONTROL")
        {
            return KeyCode::KEY_CODE_LEFT_CONTROL;
        }
        else if (value == "LEFT_ALT")
        {
            return KeyCode::KEY_CODE_LEFT_ALT;
        }
        else if (value == "LEFT_SUPER")
        {
            return KeyCode::KEY_CODE_LEFT_SUPER;
        }
        else if (value == "RIGHT_SHIFT")
        {
            return KeyCode::KEY_CODE_RIGHT_SHIFT;
        }
        else if (value == "RIGHT_CONTROL")
        {
            return KeyCode::KEY_CODE_RIGHT_CONTROL;
        }
        else if (value == "RIGHT_ALT")
        {
            return KeyCode::KEY_CODE_RIGHT_ALT;
        }
        else if (value == "RIGHT_SUPER")
        {
            return KeyCode::KEY_CODE_RIGHT_SUPER;
        }
        else if (value == "MENU")
        {
            return KeyCode::KEY_CODE_MENU;
        }

        return KeyCode::KEY_CODE_UNKNOWN;
    }

    // same as the GLFW keyboard modifiers
    typedef unsigned int KeyModifier;
    const KeyModifier MASK_KEY_MODIFIER_SHIFT = (1 << 0);
    const KeyModifier MASK_KEY_MODIFIER_CONTROL = (1 << 1);
    const KeyModifier MASK_KEY_MODIFIER_ALT = (1 << 2);
    const KeyModifier MASK_KEY_MODIFIER_SUPER = (1 << 3);
    const KeyModifier MASK_KEY_MODIFIER_CAPS_LOCK = (1 << 4);
    const KeyModifier MASK_KEY_MODIFIER_NUM_LOCK = (1 << 5);

    static std::string KeyModifierToString(KeyModifier keyModifier)
    {
        std::string returnString("KeyModifiers:");
        if (keyModifier & MASK_KEY_MODIFIER_SHIFT)
        {
            returnString += " MASK_KEY_MODIFIER_SHIFT";
        }
        if (keyModifier & MASK_KEY_MODIFIER_CONTROL)
        {
            returnString += " MASK_KEY_MODIFIER_CONTROL";
        }
        if (keyModifier & MASK_KEY_MODIFIER_ALT)
        {
            returnString += " MASK_KEY_MODIFIER_ALT";
        }
        if (keyModifier & MASK_KEY_MODIFIER_SUPER)
        {
            returnString += " MASK_KEY_MODIFIER_SUPER";
        }
        if (keyModifier & MASK_KEY_MODIFIER_CAPS_LOCK)
        {
            returnString += " MASK_KEY_MODIFIER_CAPS_LOCK";
        }
        if (keyModifier & MASK_KEY_MODIFIER_NUM_LOCK)
        {
            returnString += " MASK_KEY_MODIFIER_NUM_LOCK";
        }
        if (returnString.size() == 0)
        {
            returnString += " NONE";
        }
        return returnString;
    }

    // same as the GLFW mouse button inputs
    enum class MouseButton
    {
        MOUSE_BUTTON_UNKNOWN = -1,

        MOUSE_BUTTON_1 = 0,
        MOUSE_BUTTON_2 = 1,
        MOUSE_BUTTON_3 = 2,
        MOUSE_BUTTON_4 = 3,
        MOUSE_BUTTON_5 = 4,
        MOUSE_BUTTON_6 = 5,
        MOUSE_BUTTON_7 = 6,
        MOUSE_BUTTON_8 = 7,
        MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
        MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
        MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
        MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3
    };

    static std::string MouseButtonToString(MouseButton mouseButton)
    {
        switch (mouseButton)
        {
        case MouseButton::MOUSE_BUTTON_1:
        {
            return std::string("MOUSE_BUTTON_1");
        }
        case MouseButton::MOUSE_BUTTON_2:
        {
            return std::string("MOUSE_BUTTON_2");
        }
        case MouseButton::MOUSE_BUTTON_3:
        {
            return std::string("MOUSE_BUTTON_3");
        }
        case MouseButton::MOUSE_BUTTON_4:
        {
            return std::string("MOUSE_BUTTON_4");
        }
        case MouseButton::MOUSE_BUTTON_5:
        {
            return std::string("MOUSE_BUTTON_5");
        }
        case MouseButton::MOUSE_BUTTON_6:
        {
            return std::string("MOUSE_BUTTON_6");
        }
        case MouseButton::MOUSE_BUTTON_7:
        {
            return std::string("MOUSE_BUTTON_7");
        }
        case MouseButton::MOUSE_BUTTON_8:
        {
            return std::string("MOUSE_BUTTON_8");
        }
        }
        return std::string("UNKNOWN");
    }

    static MouseButton StringToMouseButton(const std::string& buttonString)
    {
        const std::string prefix = "MOUSE_BUTTON_";
        if (buttonString.length() < prefix.length() ||
            buttonString.find(prefix, 0) != 0)
        {
            return MouseButton::MOUSE_BUTTON_UNKNOWN;
        }

        std::string value = buttonString.substr(prefix.length());

        if (value == "1")
        {
            return MouseButton::MOUSE_BUTTON_1;
        }
        else if (value == "2")
        {
            return MouseButton::MOUSE_BUTTON_2;
        }
        else if (value == "3")
        {
            return MouseButton::MOUSE_BUTTON_3;
        }
        else if (value == "4")
        {
            return MouseButton::MOUSE_BUTTON_4;
        }
        else if (value == "5")
        {
            return MouseButton::MOUSE_BUTTON_5;
        }
        else if (value == "6")
        {
            return MouseButton::MOUSE_BUTTON_6;
        }
        else if (value == "7")
        {
            return MouseButton::MOUSE_BUTTON_7;
        }
        else if (value == "8")
        {
            return MouseButton::MOUSE_BUTTON_8;
        }
        else if (value == "LAST")
        {
            return MouseButton::MOUSE_BUTTON_LAST;
        }
        else if (value == "LEFT")
        {
            return MouseButton::MOUSE_BUTTON_LEFT;
        }
        else if (value == "RIGHT")
        {
            return MouseButton::MOUSE_BUTTON_RIGHT;
        }
        else if (value == "MIDDLE")
        {
            return MouseButton::MOUSE_BUTTON_MIDDLE;
        }

        return MouseButton::MOUSE_BUTTON_UNKNOWN;
    }

    struct Event
    {
        Event()
            : handled(false)
        {}

        virtual EventType GetEventType() const = 0;

        bool handled; // If am Event is handled, it will nolonger be Dispatched
    };

    template<typename DispatcherEvent, typename Function>
    static bool DispatchEvent(Event& event, const Function& function)
    {
        if (event.GetEventType() == DispatcherEvent::GetStaticEventType() &&
            !event.handled)
        {
            (function)(static_cast<DispatcherEvent&>(event));
            return true;
        }
        return false;
    }

#define EVENT_TYPE_FUNCTIONS(eventType)\
    static EventType GetStaticEventType()\
    {\
        return eventType;\
    }\
    \
    EventType GetEventType() const override\
    {\
        return GetStaticEventType();\
    }

    // Window Events:
    // ----------------------------------------------------------------------------

    struct KeyEvent : Event
    {
        KeyEvent(unsigned int windowId, KeyCode keyCode, ButtonAction buttonAction, KeyModifier keyModifier)
            : Event()
            , windowId(windowId)
            , keyCode(keyCode)
            , buttonAction(buttonAction)
            , keyModifier(keyModifier)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_KEY)

        unsigned int windowId;
        KeyCode keyCode;
        ButtonAction buttonAction;
        KeyModifier keyModifier;
    };

    struct MouseButtonEvent : Event
    {
        MouseButtonEvent(unsigned int windowId, MouseButton mouseButton, ButtonAction buttonAction, KeyModifier keyModifier)
            : Event()
            , windowId(windowId)
            , mouseButton(mouseButton)
            , buttonAction(buttonAction)
            , keyModifier(keyModifier)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_MOUSE_BUTTON)

        unsigned int windowId;
        MouseButton mouseButton;
        ButtonAction buttonAction;
        KeyModifier keyModifier;
    };

    struct CursorPositionEvent : Event
    {
        CursorPositionEvent(unsigned int windowId, float xPosition, float yPosition)
            : Event()
            , windowId(windowId)
            , xPosition(xPosition)
            , yPosition(yPosition)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_CURSOR_POSITION)

        unsigned int windowId;

        // origin is at the top left
        // moving to the right is positive x
        // moving down is positive y
        float xPosition, yPosition;
    };

    struct CursorEnterEvent : Event
    {
        CursorEnterEvent(unsigned int windowId, bool entered)
            : Event()
            , windowId(windowId)
            , entered(entered)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_CURSOR_ENTER)

        unsigned int windowId;
        bool entered;
    };

    struct ScrollEvent : Event
    {
        ScrollEvent(unsigned int windowId, float xOffset, float yOffset)
            : Event()
            , windowId(windowId)
            , xOffset(xOffset)
            , yOffset(yOffset)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_SCROLL)

        unsigned int windowId;
        float xOffset, yOffset;
    };

    struct WindowCloseEvent : Event
    {
        WindowCloseEvent(unsigned int windowId)
            : Event()
            , windowId(windowId)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_WINDOW_CLOSE)

        unsigned int windowId;
    };

    struct WindowFocusEvent : Event
    {
        WindowFocusEvent(unsigned int windowId, bool focused)
            : Event()
            , windowId(windowId)
            , focused(focused)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_WINDOW_FOCUS)

        unsigned int windowId;
        bool focused;
    };

    struct WindowSizeEvent : Event
    {
        WindowSizeEvent(unsigned int windowId, int width, int height)
            : Event()
            , windowId(windowId)
            , width(width)
            , height(height)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_WINDOW_SIZE)

        unsigned int windowId;
        int width, height;
    };

    struct FrameBufferSizeEvent : Event
    {
        FrameBufferSizeEvent(unsigned int windowId, int width, int height)
            : Event()
            , windowId(windowId)
            , width(width)
            , height(height)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_FRAMEBUFFER_SIZE)

        unsigned int windowId;
        int width, height;
    };

    // Scene Events:
    // -------------------------------------------------------------------------

    struct InitializeEvent : Event
    {
        InitializeEvent()
            : Event()
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_INITIALIZE_SCENE)
    };

    struct DeinitializeEvent : Event
    {
        DeinitializeEvent()
            : Event()
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_DEINITIALIZE_SCENE)
    };

    struct SwitchSceneEvent : Event
    {
        SwitchSceneEvent(unsigned int sceneId)
            : Event()
            , sceneId(sceneId)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_SWITCH_SCENE)

        unsigned int sceneId;
    };

    struct UpdateEvent : Event
    {
        UpdateEvent(unsigned long long timestep_ns)
            : Event()
            , timestep_ns(timestep_ns)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_UPDATE)

        unsigned long long timestep_ns;
    };

    struct RenderEvent : Event
    {
        RenderEvent(unsigned long long timestep_ns)
            : Event()
            , timestep_ns(timestep_ns)
        {}

        EVENT_TYPE_FUNCTIONS(EventType::EVENT_TYPE_RENDER)

        unsigned long long timestep_ns;
    };
}