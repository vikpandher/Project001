#pragma once

#include <string>



namespace Project001
{
	// NOTES:
	// GLFW Callback Functions:
	// ----------------------------------------------------------------------------
	// ERROR CALLBACKS: https://www.glfw.org/docs/3.3/group__init.html
	// GLFWerrorfun glfwSetErrorCallback (GLFWerrorfun callback) ---implemented---
	// 
	// INPUT CALLBACKS: https://www.glfw.org/docs/3.3/group__input.html
	// GLFWkeyfun glfwSetKeyCallback (GLFWwindow *window, GLFWkeyfun callback) ---implemented---
	// GLFWcharfun glfwSetCharCallback (GLFWwindow *window, GLFWcharfun callback)
	// GLFWcharmodsfun glfwSetCharModsCallback (GLFWwindow *window, GLFWcharmodsfun callback)
	// GLFWmousebuttonfun glfwSetMouseButtonCallback (GLFWwindow *window, GLFWmousebuttonfun callback) ---implemented---
	// GLFWcursorposfun glfwSetCursorPosCallback (GLFWwindow *window, GLFWcursorposfun callback) ---implemented---
	// GLFWcursorenterfun glfwSetCursorEnterCallback (GLFWwindow *window, GLFWcursorenterfun callback) ---implemented---
	// GLFWscrollfun glfwSetScrollCallback (GLFWwindow *window, GLFWscrollfun callback) ---implemented---
	// GLFWdropfun glfwSetDropCallback (GLFWwindow *window, GLFWdropfun callback)
	// GLFWjoystickfun glfwSetJoystickCallback (GLFWjoystickfun callback)
	// 
	// MONITOR CALLBACKS: https://www.glfw.org/docs/3.3/group__monitor.html
	// GLFWmonitorfun glfwSetMonitorCallback (GLFWmonitorfun callback)
	// 
	// WINDOW CALLBACKS: https://www.glfw.org/docs/3.3/group__window.html
	// GLFWwindowposfun glfwSetWindowPosCallback (GLFWwindow *window, GLFWwindowposfun callback)
	// GLFWwindowsizefun glfwSetWindowSizeCallback (GLFWwindow *window, GLFWwindowsizefun callback)
	// GLFWwindowclosefun glfwSetWindowCloseCallback (GLFWwindow *window, GLFWwindowclosefun callback) ---implemented---
	// GLFWwindowrefreshfun glfwSetWindowRefreshCallback (GLFWwindow *window, GLFWwindowrefreshfun callback)
	// GLFWwindowfocusfun glfwSetWindowFocusCallback (GLFWwindow *window, GLFWwindowfocusfun callback) ---implemented---
	// GLFWwindowiconifyfun glfwSetWindowIconifyCallback (GLFWwindow *window, GLFWwindowiconifyfun callback)
	// GLFWwindowmaximizefun glfwSetWindowMaximizeCallback (GLFWwindow *window, GLFWwindowmaximizefun callback)
	// GLFWframebuffersizefun glfwSetFramebufferSizeCallback (GLFWwindow *window, GLFWframebuffersizefun callback) ---implemented---
	// GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback (GLFWwindow *window, GLFWwindowcontentscalefun callback)
	
	enum class EventType
	{
		EVENT_TYPE_NONE = 0,
		EVENT_TYPE_KEY,
		EVENT_TYPE_MOUSE_BUTTON,
		EVENT_TYPE_CURSOR_POS,
		EVENT_TYPE_CURSOR_ENTER,
		EVENT_TYPE_SCROLL,
		EVENT_TYPE_WINDOW_CLOSE,
		EVENT_TYPE_WINDOW_FOCUS,
		EVENT_TYPE_FRAMEBUFFER_SIZE
	};

	static std::string EventTypeToString(EventType eventType)
	{
		switch (eventType)
		{
		case EventType::EVENT_TYPE_NONE:
		{
			return std::string("EVENT_TYPE_NONE");
			break;
		}
		case EventType::EVENT_TYPE_KEY:
		{
			return std::string("EVENT_TYPE_KEY");
			break;
		}
		case EventType::EVENT_TYPE_MOUSE_BUTTON:
		{
			return std::string("EVENT_TYPE_MOUSE_BUTTON");
			break;
		}
		case EventType::EVENT_TYPE_CURSOR_POS:
		{
			return std::string("EVENT_TYPE_CURSOR_POS");
			break;
		}
		case EventType::EVENT_TYPE_CURSOR_ENTER:
		{
			return std::string("EVENT_TYPE_CURSOR_ENTER");
			break;
		}
		case EventType::EVENT_TYPE_SCROLL:
		{
			return std::string("EVENT_TYPE_SCROLL");
			break;
		}
		case EventType::EVENT_TYPE_WINDOW_CLOSE:
		{
			return std::string("EVENT_TYPE_WINDOW_CLOSE");
			break;
		}
		case EventType::EVENT_TYPE_WINDOW_FOCUS:
		{
			return std::string("EVENT_TYPE_WINDOW_FOCUS");
			break;
		}
		case EventType::EVENT_TYPE_FRAMEBUFFER_SIZE:
		{
			return std::string("EVENT_TYPE_FRAMEBUFFER_SIZE");
			break;
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
		switch(buttonAction)
		{
		case ButtonAction::KEY_ACTION_RELEASE:
		{
			return std::string("KEY_ACTION_RELEASE");
			break;
		}
		case ButtonAction::KEY_ACTION_PRESS:
		{
			return std::string("KEY_ACTION_PRESS");
			break;
		}
		case ButtonAction::KEY_ACTION_REPEAT:
		{
			return std::string("KEY_ACTION_REPEAT");
			break;
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
		KEY_CODE_MENU = 348,
	};

	static std::string KeyCodeToString(KeyCode keyCode)
	{
		switch (keyCode)
		{
		case KeyCode::KEY_CODE_UNKNOWN:
		{
			return std::string("KEY_CODE_UNKNOWN");
			break;
		}
		case KeyCode::KEY_CODE_SPACE:
		{
			return std::string("KEY_CODE_SPACE");
			break;
		}
		case KeyCode::KEY_CODE_APOSTROPHE:
		{
			return std::string("KEY_CODE_APOSTROPHE");
			break;
		}
		case KeyCode::KEY_CODE_COMMA:
		{
			return std::string("KEY_CODE_COMMA");
			break;
		}
		case KeyCode::KEY_CODE_MINUS:
		{
			return std::string("KEY_CODE_MINUS");
			break;
		}
		case KeyCode::KEY_CODE_PERIOD:
		{
			return std::string("KEY_CODE_PERIOD");
			break;
		}
		case KeyCode::KEY_CODE_SLASH:
		{
			return std::string("KEY_CODE_SLASH");
			break;
		}
		case KeyCode::KEY_CODE_0:
		{
			return std::string("KEY_CODE_0");
			break;
		}
		case KeyCode::KEY_CODE_1:
		{
			return std::string("KEY_CODE_1");
			break;
		}
		case KeyCode::KEY_CODE_2:
		{
			return std::string("KEY_CODE_2");
			break;
		}
		case KeyCode::KEY_CODE_3:
		{
			return std::string("KEY_CODE_3");
			break;
		}
		case KeyCode::KEY_CODE_4:
		{
			return std::string("KEY_CODE_4");
			break;
		}
		case KeyCode::KEY_CODE_5:
		{
			return std::string("KEY_CODE_5");
			break;
		}
		case KeyCode::KEY_CODE_6:
		{
			return std::string("KEY_CODE_6");
			break;
		}
		case KeyCode::KEY_CODE_7:
		{
			return std::string("KEY_CODE_7");
			break;
		}
		case KeyCode::KEY_CODE_8:
		{
			return std::string("KEY_CODE_8");
			break;
		}
		case KeyCode::KEY_CODE_9:
		{
			return std::string("KEY_CODE_9");
			break;
		}
		case KeyCode::KEY_CODE_SEMICOLON:
		{
			return std::string("KEY_CODE_SEMICOLON");
			break;
		}
		case KeyCode::KEY_CODE_EQUAL:
		{
			return std::string("KEY_CODE_EQUAL");
			break;
		}
		case KeyCode::KEY_CODE_A:
		{
			return std::string("KEY_CODE_A");
			break;
		}
		case KeyCode::KEY_CODE_B:
		{
			return std::string("KEY_CODE_B");
			break;
		}
		case KeyCode::KEY_CODE_C:
		{
			return std::string("KEY_CODE_C");
			break;
		}
		case KeyCode::KEY_CODE_D:
		{
			return std::string("KEY_CODE_D");
			break;
		}
		case KeyCode::KEY_CODE_E:
		{
			return std::string("KEY_CODE_E");
			break;
		}
		case KeyCode::KEY_CODE_F:
		{
			return std::string("KEY_CODE_F");
			break;
		}
		case KeyCode::KEY_CODE_G:
		{
			return std::string("KEY_CODE_G");
			break;
		}
		case KeyCode::KEY_CODE_H:
		{
			return std::string("KEY_CODE_H");
			break;
		}
		case KeyCode::KEY_CODE_I:
		{
			return std::string("KEY_CODE_I");
			break;
		}
		case KeyCode::KEY_CODE_J:
		{
			return std::string("KEY_CODE_J");
			break;
		}
		case KeyCode::KEY_CODE_K:
		{
			return std::string("KEY_CODE_K");
			break;
		}
		case KeyCode::KEY_CODE_L:
		{
			return std::string("KEY_CODE_L");
			break;
		}
		case KeyCode::KEY_CODE_M:
		{
			return std::string("KEY_CODE_M");
			break;
		}
		case KeyCode::KEY_CODE_N:
		{
			return std::string("KEY_CODE_N");
			break;
		}
		case KeyCode::KEY_CODE_O:
		{
			return std::string("KEY_CODE_O");
			break;
		}
		case KeyCode::KEY_CODE_P:
		{
			return std::string("KEY_CODE_P");
			break;
		}
		case KeyCode::KEY_CODE_Q:
		{
			return std::string("KEY_CODE_Q");
			break;
		}
		case KeyCode::KEY_CODE_R:
		{
			return std::string("KEY_CODE_R");
			break;
		}
		case KeyCode::KEY_CODE_S:
		{
			return std::string("KEY_CODE_S");
			break;
		}
		case KeyCode::KEY_CODE_T:
		{
			return std::string("KEY_CODE_T");
			break;
		}
		case KeyCode::KEY_CODE_U:
		{
			return std::string("KEY_CODE_U");
			break;
		}
		case KeyCode::KEY_CODE_V:
		{
			return std::string("KEY_CODE_V");
			break;
		}
		case KeyCode::KEY_CODE_W:
		{
			return std::string("KEY_CODE_W");
			break;
		}
		case KeyCode::KEY_CODE_X:
		{
			return std::string("KEY_CODE_X");
			break;
		}
		case KeyCode::KEY_CODE_Y:
		{
			return std::string("KEY_CODE_Y");
			break;
		}
		case KeyCode::KEY_CODE_Z:
		{
			return std::string("KEY_CODE_Z");
			break;
		}
		case KeyCode::KEY_CODE_LEFT_BRACKET:
		{
			return std::string("KEY_CODE_LEFT_BRACKET");
			break;
		}
		case KeyCode::KEY_CODE_BACKSLASH:
		{
			return std::string("KEY_CODE_BACKSLASH");
			break;
		}
		case KeyCode::KEY_CODE_RIGHT_BRACKET:
		{
			return std::string("KEY_CODE_RIGHT_BRACKET");
			break;
		}
		case KeyCode::KEY_CODE_GRAVE_ACCENT:
		{
			return std::string("KEY_CODE_GRAVE_ACCENT");
			break;
		}
		case KeyCode::KEY_CODE_WORLD_1:
		{
			return std::string("KEY_CODE_WORLD_1");
			break;
		}
		case KeyCode::KEY_CODE_WORLD_2:
		{
			return std::string("KEY_CODE_WORLD_2");
			break;
		}
		case KeyCode::KEY_CODE_ESCAPE:
		{
			return std::string("KEY_CODE_ESCAPE");
			break;
		}
		case KeyCode::KEY_CODE_ENTER:
		{
			return std::string("KEY_CODE_ENTER");
			break;
		}
		case KeyCode::KEY_CODE_TAB:
		{
			return std::string("KEY_CODE_TAB");
			break;
		}
		case KeyCode::KEY_CODE_BACKSPACE:
		{
			return std::string("KEY_CODE_BACKSPACE");
			break;
		}
		case KeyCode::KEY_CODE_INSERT:
		{
			return std::string("KEY_CODE_INSERT");
			break;
		}
		case KeyCode::KEY_CODE_DELETE:
		{
			return std::string("KEY_CODE_DELETE");
			break;
		}
		case KeyCode::KEY_CODE_RIGHT:
		{
			return std::string("KEY_CODE_RIGHT");
			break;
		}
		case KeyCode::KEY_CODE_LEFT:
		{
			return std::string("KEY_CODE_LEFT");
			break;
		}
		case KeyCode::KEY_CODE_DOWN:
		{
			return std::string("KEY_CODE_DOWN");
			break;
		}
		case KeyCode::KEY_CODE_UP:
		{
			return std::string("KEY_CODE_UP");
			break;
		}
		case KeyCode::KEY_CODE_PAGE_UP:
		{
			return std::string("KEY_CODE_PAGE_UP");
			break;
		}
		case KeyCode::KEY_CODE_PAGE_DOWN:
		{
			return std::string("KEY_CODE_PAGE_DOWN");
			break;
		}
		case KeyCode::KEY_CODE_HOME:
		{
			return std::string("KEY_CODE_HOME");
			break;
		}
		case KeyCode::KEY_CODE_END:
		{
			return std::string("KEY_CODE_END");
			break;
		}
		case KeyCode::KEY_CODE_CAPS_LOCK:
		{
			return std::string("KEY_CODE_CAPS_LOCK");
			break;
		}
		case KeyCode::KEY_CODE_SCROLL_LOCK:
		{
			return std::string("KEY_CODE_SCROLL_LOCK");
			break;
		}
		case KeyCode::KEY_CODE_NUM_LOCK:
		{
			return std::string("KEY_CODE_NUM_LOCK");
			break;
		}
		case KeyCode::KEY_CODE_PRINT_SCREEN:
		{
			return std::string("KEY_CODE_PRINT_SCREEN");
			break;
		}
		case KeyCode::KEY_CODE_PAUSE:
		{
			return std::string("KEY_CODE_PAUSE");
			break;
		}
		case KeyCode::KEY_CODE_F1:
		{
			return std::string("KEY_CODE_F1");
			break;
		}
		case KeyCode::KEY_CODE_F2:
		{
			return std::string("KEY_CODE_F2");
			break;
		}
		case KeyCode::KEY_CODE_F3:
		{
			return std::string("KEY_CODE_F3");
			break;
		}
		case KeyCode::KEY_CODE_F4:
		{
			return std::string("KEY_CODE_F4");
			break;
		}
		case KeyCode::KEY_CODE_F5:
		{
			return std::string("KEY_CODE_F5");
			break;
		}
		case KeyCode::KEY_CODE_F6:
		{
			return std::string("KEY_CODE_F6");
			break;
		}
		case KeyCode::KEY_CODE_F7:
		{
			return std::string("KEY_CODE_F7");
			break;
		}
		case KeyCode::KEY_CODE_F8:
		{
			return std::string("KEY_CODE_F8");
			break;
		}
		case KeyCode::KEY_CODE_F9:
		{
			return std::string("KEY_CODE_F9");
			break;
		}
		case KeyCode::KEY_CODE_F10:
		{
			return std::string("KEY_CODE_F10");
			break;
		}
		case KeyCode::KEY_CODE_F11:
		{
			return std::string("KEY_CODE_F11");
			break;
		}
		case KeyCode::KEY_CODE_F12:
		{
			return std::string("KEY_CODE_F12");
			break;
		}
		case KeyCode::KEY_CODE_F13:
		{
			return std::string("KEY_CODE_F13");
			break;
		}
		case KeyCode::KEY_CODE_F14:
		{
			return std::string("KEY_CODE_F14");
			break;
		}
		case KeyCode::KEY_CODE_F15:
		{
			return std::string("KEY_CODE_F15");
			break;
		}
		case KeyCode::KEY_CODE_F16:
		{
			return std::string("KEY_CODE_F16");
			break;
		}
		case KeyCode::KEY_CODE_F17:
		{
			return std::string("KEY_CODE_F17");
			break;
		}
		case KeyCode::KEY_CODE_F18:
		{
			return std::string("KEY_CODE_F18");
			break;
		}
		case KeyCode::KEY_CODE_F19:
		{
			return std::string("KEY_CODE_F19");
			break;
		}
		case KeyCode::KEY_CODE_F20:
		{
			return std::string("KEY_CODE_F20");
			break;
		}
		case KeyCode::KEY_CODE_F21:
		{
			return std::string("KEY_CODE_F21");
			break;
		}
		case KeyCode::KEY_CODE_F22:
		{
			return std::string("KEY_CODE_F22");
			break;
		}
		case KeyCode::KEY_CODE_F23:
		{
			return std::string("KEY_CODE_F23");
			break;
		}
		case KeyCode::KEY_CODE_F24:
		{
			return std::string("KEY_CODE_F24");
			break;
		}
		case KeyCode::KEY_CODE_F25:
		{
			return std::string("KEY_CODE_F25");
			break;
		}
		case KeyCode::KEY_CODE_KP_0:
		{
			return std::string("KEY_CODE_KP_0");
			break;
		}
		case KeyCode::KEY_CODE_KP_1:
		{
			return std::string("KEY_CODE_KP_1");
			break;
		}
		case KeyCode::KEY_CODE_KP_2:
		{
			return std::string("KEY_CODE_KP_2");
			break;
		}
		case KeyCode::KEY_CODE_KP_3:
		{
			return std::string("KEY_CODE_KP_3");
			break;
		}
		case KeyCode::KEY_CODE_KP_4:
		{
			return std::string("KEY_CODE_KP_4");
			break;
		}
		case KeyCode::KEY_CODE_KP_5:
		{
			return std::string("KEY_CODE_KP_5");
			break;
		}
		case KeyCode::KEY_CODE_KP_6:
		{
			return std::string("KEY_CODE_KP_6");
			break;
		}
		case KeyCode::KEY_CODE_KP_7:
		{
			return std::string("KEY_CODE_KP_7");
			break;
		}
		case KeyCode::KEY_CODE_KP_8:
		{
			return std::string("KEY_CODE_KP_8");
			break;
		}
		case KeyCode::KEY_CODE_KP_9:
		{
			return std::string("KEY_CODE_KP_9");
			break;
		}
		case KeyCode::KEY_CODE_KP_DECIMAL:
		{
			return std::string("KEY_CODE_KP_DECIMAL");
			break;
		}
		case KeyCode::KEY_CODE_KP_DIVIDE:
		{
			return std::string("KEY_CODE_KP_DIVIDE");
			break;
		}
		case KeyCode::KEY_CODE_KP_MULTIPLY:
		{
			return std::string("KEY_CODE_KP_MULTIPLY");
			break;
		}
		case KeyCode::KEY_CODE_KP_SUBTRACT:
		{
			return std::string("KEY_CODE_KP_SUBTRACT");
			break;
		}
		case KeyCode::KEY_CODE_KP_ADD:
		{
			return std::string("KEY_CODE_KP_ADD");
			break;
		}
		case KeyCode::KEY_CODE_KP_ENTER:
		{
			return std::string("KEY_CODE_KP_ENTER");
			break;
		}
		case KeyCode::KEY_CODE_KP_EQUAL:
		{
			return std::string("KEY_CODE_KP_EQUAL");
			break;
		}
		case KeyCode::KEY_CODE_LEFT_SHIFT:
		{
			return std::string("KEY_CODE_LEFT_SHIFT");
			break;
		}
		case KeyCode::KEY_CODE_LEFT_CONTROL:
		{
			return std::string("KEY_CODE_LEFT_CONTROL");
			break;
		}
		case KeyCode::KEY_CODE_LEFT_ALT:
		{
			return std::string("KEY_CODE_LEFT_ALT");
			break;
		}
		case KeyCode::KEY_CODE_LEFT_SUPER:
		{
			return std::string("KEY_CODE_LEFT_SUPER");
			break;
		}
		case KeyCode::KEY_CODE_RIGHT_SHIFT:
		{
			return std::string("KEY_CODE_RIGHT_SHIFT");
			break;
		}
		case KeyCode::KEY_CODE_RIGHT_CONTROL:
		{
			return std::string("KEY_CODE_RIGHT_CONTROL");
			break;
		}
		case KeyCode::KEY_CODE_RIGHT_ALT:
		{
			return std::string("KEY_CODE_RIGHT_ALT");
			break;
		}
		case KeyCode::KEY_CODE_RIGHT_SUPER:
		{
			return std::string("KEY_CODE_RIGHT_SUPER");
			break;
		}
		case KeyCode::KEY_CODE_MENU:
		{
			return std::string("KEY_CODE_MENU");
			break;
		}
		}
		return std::string("UNKNOWN");
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
		std::string returnString;
		if (keyModifier & MASK_KEY_MODIFIER_SHIFT)
		{
			returnString += "MASK_KEY_MODIFIER_SHIFT";
		}
		if (keyModifier & MASK_KEY_MODIFIER_CONTROL)
		{
			returnString += "MASK_KEY_MODIFIER_CONTROL";
		}
		if (keyModifier & MASK_KEY_MODIFIER_ALT)
		{
			returnString += "MASK_KEY_MODIFIER_ALT";
		}
		if (keyModifier & MASK_KEY_MODIFIER_SUPER)
		{
			returnString += "MASK_KEY_MODIFIER_SUPER";
		}
		if (keyModifier & MASK_KEY_MODIFIER_CAPS_LOCK)
		{
			returnString += "MASK_KEY_MODIFIER_CAPS_LOCK";
		}
		if (keyModifier & MASK_KEY_MODIFIER_NUM_LOCK)
		{
			returnString += "MASK_KEY_MODIFIER_NUM_LOCK";
		}
		if (returnString.size() == 0)
		{
			returnString += "NONE";
		}
		return returnString;
	}

	// same as the GLFW mouse button inputs
	enum class MouseButton
	{
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
			break;
		}
		case MouseButton::MOUSE_BUTTON_2:
		{
			return std::string("MOUSE_BUTTON_2");
			break;
		}
		case MouseButton::MOUSE_BUTTON_3:
		{
			return std::string("MOUSE_BUTTON_3");
			break;
		}
		case MouseButton::MOUSE_BUTTON_4:
		{
			return std::string("MOUSE_BUTTON_4");
			break;
		}
		case MouseButton::MOUSE_BUTTON_5:
		{
			return std::string("MOUSE_BUTTON_5");
			break;
		}
		case MouseButton::MOUSE_BUTTON_6:
		{
			return std::string("MOUSE_BUTTON_6");
			break;
		}
		case MouseButton::MOUSE_BUTTON_7:
		{
			return std::string("MOUSE_BUTTON_7");
			break;
		}
		case MouseButton::MOUSE_BUTTON_8:
		{
			return std::string("MOUSE_BUTTON_8");
			break;
		}
		}
		return std::string("UNKNOWN");
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

	struct Event
	{
		Event()
			: handled(false)
		{}

		virtual EventType GetEventType() const = 0;

		bool handled;
	};

	// Maybe I'll use this...
	template<typename T, typename F>
	static bool DispatchEvent(Event& event, const F& function)
	{
		if (event.GetEventType() == T::GetEventType())
		{
			event.handled = function(static_cast<T>(event));
			return true;
		}
		return false;
	}

	struct KeyEvent : Event
	{
		KeyEvent(KeyCode keyCode, ButtonAction buttonAction, KeyModifier keyModifier)
			: Event()
			, keyCode(keyCode)
			, buttonAction(buttonAction)
			, keyModifier(keyModifier)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_KEY;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_KEY;
		}

		KeyCode keyCode;
		ButtonAction buttonAction;
		KeyModifier keyModifier;
	};

	struct MouseButtonEvent : Event
	{
		MouseButtonEvent(MouseButton mouseButton, ButtonAction buttonAction, KeyModifier keyModifier)
			: Event()
			, mouseButton(mouseButton)
			, buttonAction(buttonAction)
			, keyModifier(keyModifier)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_MOUSE_BUTTON;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_MOUSE_BUTTON;
		}

		MouseButton mouseButton;
		ButtonAction buttonAction;
		KeyModifier keyModifier;
	};

	struct CursorPosEvent : Event
	{
		CursorPosEvent(float xPos, float yPos)
			: Event()
			, xPos(xPos)
			, yPos(yPos)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_CURSOR_POS;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_CURSOR_POS;
		}

		// origin is at the top left
		float xPos, yPos;
	};

	struct CursorEnterEvent : Event
	{
		CursorEnterEvent(bool entered)
			: Event()
			, entered(entered)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_CURSOR_ENTER;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_CURSOR_ENTER;
		}

		bool entered;
	};

	struct ScrollEvent : Event
	{
		ScrollEvent(float xOffset, float yOffset)
			: Event()
			, xOffset(xOffset)
			, yOffset(yOffset)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_SCROLL;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_SCROLL;
		}

		float xOffset, yOffset;
	};

	struct WindowCloseEvent : Event
	{
		WindowCloseEvent()
			: Event()
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_WINDOW_CLOSE;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_WINDOW_CLOSE;
		}
	};

	struct WindowFocusEvent : Event
	{
		WindowFocusEvent(bool focused)
			: Event()
			, focused(focused)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_WINDOW_FOCUS;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_WINDOW_FOCUS;
		}

		bool focused;
	};

	struct FrameBufferSizeEvent : Event
	{
		FrameBufferSizeEvent(int width, int height)
			: Event()
			, width(width)
			, height(height)
		{}

		EventType GetEventType() const override
		{
			return EventType::EVENT_TYPE_FRAMEBUFFER_SIZE;
		}

		static EventType GetStaticEventType()
		{
			return EventType::EVENT_TYPE_FRAMEBUFFER_SIZE;
		}

		int width, height;
	};
}