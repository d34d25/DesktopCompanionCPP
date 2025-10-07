#include <iostream>
#include "window.h"
#include <wincodec.h>
#include "input.h"


Shapes::Rectangle clRect;


Color rectCl = { 255,255,0,50};

float hinaScaleX = 1.0f;
float hinaScaleY = 1.0f;

Image hiniatureA;
Image hiniatureB;

Image hana;

bool clicked = false;

void mDraw(NewCanvasX* canvas)
{

	canvas->BeginDraw();

	canvas->Clear({0.0f,0.0f,0.0f,0.0f});


	//draw here--------------------------------------------------------


	//canvas->DrawRect(clRect, rectCl, 0.0f, 1.0f, 1.0f);

	float drawX = clRect.x + (clRect.width / 2) - (hiniatureA.width * hinaScaleX / 2) + 20;
	float drawY = clRect.y + clRect.height - (hiniatureA.height * hinaScaleY) + 40;

	if (!clicked)
	{
		canvas->DrawImg(hiniatureA, drawX, drawY, nullptr, 0.0f, hinaScaleX, hinaScaleY);
	}
	else
	{
		canvas->DrawImg(hiniatureB, drawX, drawY, nullptr, 0.0f, hinaScaleX, hinaScaleY);
	}


	//--------------------------------------------------------------------

	canvas->EndDraw();

	canvas->Present();

}

int main()
{
	Input* globalInput;

	Window* pWindow = new Window(1200, 680);
	
	hiniatureA = pWindow->GetNewCanvas()->LoadImg("D:\\ProgrammingProjects\\Cpp\\DesktopCompanion\\assets\\images\\Hina_00.png");
	hiniatureB = pWindow->GetNewCanvas()->LoadImg("D:\\ProgrammingProjects\\Cpp\\DesktopCompanion\\assets\\images\\Hina_10.png");
	
	clRect.width = 200;
	clRect.height = 700;

	clRect.x = 700;//pWindow->GetScreenWidth() - clRect.width;
	clRect.y = pWindow->GetScreenHeight() - clRect.height;

	pWindow->SetNewDrawCallback(mDraw);

	globalInput = new Input(pWindow->GetHWND());

	bool running = true;

	while (running)
	{
		running = pWindow->ProcessMessages();

		globalInput->Update();

		Vector2D mousePos = globalInput->GetMousePosition();

		if (IsPointOnRect(mousePos,clRect))
		{
			rectCl = { 0,0,255,50 };

			LONG exStyle = GetWindowLong(pWindow->GetHWND(), GWL_EXSTYLE);
			exStyle &= ~WS_EX_TRANSPARENT;
			SetWindowLong(pWindow->GetHWND(), GWL_EXSTYLE, exStyle);

			SetWindowPos(pWindow->GetHWND(), nullptr, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);


		}
		else
		{
			rectCl = { 255,255,0,50 };
			LONG exStyle = GetWindowLong(pWindow->GetHWND(), GWL_EXSTYLE);
			exStyle |= WS_EX_TRANSPARENT;
			SetWindowLong(pWindow->GetHWND(), GWL_EXSTYLE, exStyle);

			// Apply the style change
			SetWindowPos(pWindow->GetHWND(), nullptr, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}

		if (IsPointOnRect(mousePos, clRect) && globalInput->IsLeftMouseHeld())
		{
			clicked = true;
		}
		else
		{
			clicked = false;
		}


		Sleep(20);
	}

	delete globalInput;
	delete pWindow;

	return 0;
}