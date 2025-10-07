#include "input.h"

Input::Input(HWND hwnd)
{
	this->hwnd = hwnd;

	for (int key = 0; key <= 255; ++key)
	{
		currentKeyState[key] = false;
		previousKeyState[key] = false;
	}

	currentLeftButton = false;
	previousLeftButton = false;

	currentRightButton = false;
	previousRightButton = false;

	currentMiddleButton = false;
	previousMiddleButton = false;

	mousePosition = { 0, 0 };
}

Input::~Input()
{
}

void Input::Update()
{
	previousKeyState = currentKeyState;

	for (int key = 0; key <= 255; ++key)
	{
		currentKeyState[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
	}

	previousLeftButton = currentLeftButton;
	previousRightButton = currentRightButton;

	previousMiddleButton = currentMiddleButton;

	currentLeftButton = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	currentRightButton = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
	currentMiddleButton = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

	POINT mousePos;

	GetCursorPos(&mousePos);

	ScreenToClient(this->hwnd, &mousePos);

	mousePosition = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
}

bool Input::IsKeyPressed(int keyCode) const
{
	auto curr = currentKeyState.find(keyCode);
	auto prev = previousKeyState.find(keyCode);

	bool currState = (curr != currentKeyState.end()) ? curr->second : false;
	bool prevState = (prev != previousKeyState.end()) ? prev->second : false;

	return currState && !prevState;
}

bool Input::IsKeyReleased(int keyCode) const
{
	auto curr = currentKeyState.find(keyCode);
	auto prev = previousKeyState.find(keyCode);

	bool currState = (curr != currentKeyState.end()) ? curr->second : false;
	bool prevState = (prev != previousKeyState.end()) ? prev->second : false;

	return !currState && prevState;
}

bool Input::IsKeyHeld(int keyCode) const
{
	auto it = this->currentKeyState.find(keyCode);
	return (it != this->currentKeyState.end()) ? it->second : false;
}

Vector2D Input::GetMousePosition() const
{
	return this->mousePosition;
}

bool Input::IsLeftMouseHeld() const
{
	return this->currentLeftButton;
}

bool Input::IsLeftMousePressed() const
{
	return this->currentLeftButton && !this->previousLeftButton;
}

bool Input::IsLeftMouseReleased() const
{
	return !this->currentLeftButton && this->previousLeftButton;
}

bool Input::IsRightMouseHeld() const
{
	return this->currentRightButton;
}

bool Input::IsRightMousePressed() const
{
	return this->currentRightButton && !this->previousRightButton;
}

bool Input::IsRightMouseReleased() const
{
	return !this->currentRightButton && this->previousRightButton;
}

bool Input::IsMiddleMouseHeld() const
{
	return this->currentMiddleButton;
}

bool Input::IsMiddleMousePressed() const
{
	return this->currentMiddleButton && !this->previousMiddleButton;
}

bool Input::IsMiddleMouseReleased() const
{
	return !this->currentMiddleButton && this->previousMiddleButton;
}

