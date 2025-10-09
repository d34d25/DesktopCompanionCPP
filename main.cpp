#include <iostream>
#include "window.h"
#include <wincodec.h>
#include "input.h"
#include "companion.h"

Shapes::Rectangle clRect;

Color rectCl = { 255,255,0,50};

float hinaScaleX = 0.85f;
float hinaScaleY = 0.85f;

Image hiniatureA;
Image hiniatureB;

Image hana;

Companion* hinature;

bool clicked = false;

void mDraw(NewCanvasX* canvas)
{

	canvas->BeginDraw();

	canvas->Clear({0.0f,0.0f,0.0f,0.0f});

	

	//draw here--------------------------------------------------------

	float drawX = clRect.x + (clRect.width * 0.5f) - (hiniatureA.width * hinaScaleX * 0.5f) - 65;
	float drawY = clRect.y + clRect.height - (hiniatureA.height * hinaScaleY) - 40;

	if (hinature)
	{
		//reminder to self
		/*
		* Copy for image objects is disabled
		* so after doing Image img = LoadImg("path");
		* you can't copy it in any way
		* 
		* so doing Image img2 = img is not possible
		* for that you gotta do img2 = std::move(img);
		* 
		* but doing this will result of the original varibale
		* not be usebale unless you move it back again
		* (you're literally moving the variable from one place
		* to another)
		* 
		* another approach is using pointers
		* 
		*/

		//I hope I don't end up breaking my keyboard :)

		Image* currImg = hinature->GetCurrentImage();

		if (currImg->isValid() || currImg) 
		{
			canvas->DrawImg(*currImg, drawX, drawY, nullptr, 0.0f, hinaScaleX, hinaScaleY);
		}


		if (clicked)
		{
			hinature->ChangeMood(Mood::EMBARRASSED);
		}
		else
		{
			hinature->ChangeMood(Mood::NEUTRAL);
		}
	}

	//canvas->DrawRect(clRect, rectCl);

	//--------------------------------------------------------------------

	canvas->EndDraw();

	canvas->Present();

}

int main()
{
	bool dragging = false;

	Input* globalInput;

	Window* pWindow = new Window(1200, 680);
	
	hiniatureA = pWindow->GetNewCanvas()->LoadImg("D:\\ProgrammingProjects\\Cpp\\DesktopCompanion\\assets\\images\\base\\neutral00.png");
	hiniatureB = pWindow->GetNewCanvas()->LoadImg("D:\\ProgrammingProjects\\Cpp\\DesktopCompanion\\assets\\images\\base\\veryEmbarrassed10.png");
	
	hinature = new Companion(pWindow->GetNewCanvas(), Variant::BASE);

	clRect.width = 200 * hinaScaleX;
	clRect.height = 715 * hinaScaleY;

	clRect.x = pWindow->GetScreenWidth() - clRect.width;
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

		if (IsPointOnRect(mousePos, clRect) && globalInput->IsMiddleMouseHeld())
		{
			dragging = true;
		}
		else if(globalInput->IsMiddleMouseReleased())
		{
			dragging = false;
		}

		if (dragging)
		{
			clRect.x = (mousePos.x - clRect.width * 0.5f);
			clRect.y = (mousePos.y - clRect.height * 0.25f);
		}

		Sleep(20);
	}

	delete hinature;
	delete globalInput;
	delete pWindow;

	return 0;
}