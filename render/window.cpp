#include "window.h"
#include <windowsx.h>
#include <iostream>

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wparam, LPARAM lparam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		Draw(hwnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hwnd, uMsg, wparam, lparam);
}

void Draw(HWND hwnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);

	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (window)
	{
		NewCanvasX* canvas = window->GetNewCanvas();

		if (!canvas)
		{
			EndPaint(hwnd, &ps);
			return;
		}

		if (auto callback = window->GetNewDrawCallback())
		{
			callback(canvas);
		}
	}

	EndPaint(hwnd, &ps);
}


Window::Window(int width, int height):
	m_hInstance(GetModuleHandle(nullptr))
{

	const wchar_t* CLASS_NAME = L"Desktop companion";

	WNDCLASS wndClass = {};

	wndClass.lpszClassName = CLASS_NAME;

	wndClass.hInstance = m_hInstance;

	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);

	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_POPUP;//WS_POPUP;

	exStyle = WS_EX_NOREDIRECTIONBITMAP | WS_EX_LAYERED;// | WS_EX_TOPMOST;

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT rect;
	rect.left = 0;
	rect.top = 0;

	rect.right = screenWidth;
	rect.bottom = screenHeight;

	AdjustWindowRect(&rect, style, false);

	m_hwnd = CreateWindowEx(
		exStyle,
		CLASS_NAME,
		L"",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	this->nCanvas = new NewCanvasX(m_hwnd);

	SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	ShowWindow(m_hwnd, SW_SHOW);
}

Window::~Window()
{
	if (this->nCanvas)
	{
		this->nCanvas = nullptr;
	}

	const wchar_t* CLASS_NAME = L"Desktop companion";

	UnregisterClass(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages()
{

	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (nUserDrawCallback)
	{
		nUserDrawCallback(nCanvas);
	}

	return true;
}

void Window::SetNewDrawCallback(nDrawCallback callback)
{
	this->nUserDrawCallback = callback;
}


HWND Window::GetHWND()
{
	return this->m_hwnd;
}

NewCanvasX* Window::GetNewCanvas()
{
	return this->nCanvas;
}

nDrawCallback Window::GetNewDrawCallback()
{
	return this->nUserDrawCallback;
}

int Window::GetScreenWidth()
{
	return this->screenWidth;
}

int Window::GetScreenHeight()
{
	return this->screenHeight;
}


/*



*/