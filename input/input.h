#pragma once

#include <windows.h>
#include <unordered_map>
#include "input.h"
#include "myTypes.h"

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Q 0x51
#define VK_E 0x45
#define VK_R 0x52
#define VK_F 0x46
#define VK_Z 0x5A
#define VK_X 0x58
#define VK_C 0x43
#define VK_V 0x56
#define VK_B 0x42
#define VK_N 0x4E
#define VK_M 0x4D

#define VK_UP_ARROW    VK_UP
#define VK_DOWN_ARROW  VK_DOWN
#define VK_LEFT_ARROW  VK_LEFT
#define VK_RIGHT_ARROW VK_RIGHT

#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#define VK_SHIFT_LEFT   VK_LSHIFT
#define VK_SHIFT_RIGHT  VK_RSHIFT
#define VK_CTRL_LEFT    VK_LCONTROL
#define VK_CTRL_RIGHT   VK_RCONTROL
#define VK_ALT_LEFT     VK_LMENU
#define VK_ALT_RIGHT    VK_RMENU

#define VK_F1  0x70
#define VK_F2  0x71
#define VK_F3  0x72
#define VK_F4  0x73
#define VK_F5  0x74
#define VK_F6  0x75
#define VK_F7  0x76
#define VK_F8  0x77
#define VK_F9  0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B

#define VK_SPACEBAR  VK_SPACE
#define VK_ENTER     VK_RETURN
#define VK_ESCAPE    VK_ESCAPE
#define VK_TAB       VK_TAB
#define VK_BACKSPACE VK_BACK
#define VK_DELETE    VK_DELETE
#define VK_INSERT    VK_INSERT
#define VK_HOME      VK_HOME
#define VK_END       VK_END
#define VK_PAGE_UP   VK_PRIOR
#define VK_PAGE_DOWN VK_NEXT


class Input
{
private:

	std::unordered_map<int, bool> currentKeyState;
	std::unordered_map<int, bool> previousKeyState;

	bool currentLeftButton = false;
	bool previousLeftButton = false;

	bool currentRightButton = false;
	bool previousRightButton = false;

	bool currentMiddleButton = false;
	bool previousMiddleButton = false;

	Vector2D mousePosition = { 0, 0 };

	HWND hwnd;

public:

	Input(HWND hwnd);

	~Input();

	void Update();

	bool IsKeyPressed(int keyCode) const;
	bool IsKeyReleased(int keyCode) const;
	bool IsKeyHeld(int keyCode) const;

	Vector2D GetMousePosition() const;

	bool IsLeftMouseHeld() const;
	bool IsLeftMousePressed() const;
	bool IsLeftMouseReleased() const;

	bool IsRightMouseHeld() const;
	bool IsRightMousePressed() const;
	bool IsRightMouseReleased() const;

	bool IsMiddleMouseHeld() const;
	bool IsMiddleMousePressed() const;
	bool IsMiddleMouseReleased() const;
};