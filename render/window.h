#pragma once
#include <functional>
#include <Windows.h>
#include "newCanvasX.h"
#include "collisions.h"

using nDrawCallback = std::function<void(NewCanvasX*)>;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM, LPARAM lparam);

void Draw(HWND hwnd);

class Window
{
private:

	HINSTANCE m_hInstance = nullptr;

	HWND m_hwnd = nullptr;

	NewCanvasX* nCanvas = nullptr;

	nDrawCallback nUserDrawCallback;

	int screenWidth = 100;
	int screenHeight = 100;

public:

	DWORD exStyle;

	Window(int width, int height);

	~Window();

	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	bool ProcessMessages();

	void SetNewDrawCallback(nDrawCallback callback);

	HWND GetHWND();

	NewCanvasX* GetNewCanvas();

	nDrawCallback GetNewDrawCallback();

	int GetScreenWidth();

	int GetScreenHeight();
};