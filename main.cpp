#include <iostream>
#include "window.h"
#include <wincodec.h>
#include "input.h"
#include "companion.h"

Shapes::Rectangle clRect;

Color rectCl = { 255,255,0,50};

float hinaScale = 0.75f;

Companion* hinature;

bool clicked = false;

float screenW, screenH;

float scaleX, scaleY;

float offsetNumX, offsetNumY;

void ResizeClRect(Image* img)
{
	float scaledImageWidth = img->width * hinaScale * hinaScale * scaleX;
	float scaledImageHeight = img->height * hinaScale * hinaScale * scaleY;

	clRect.width = scaledImageWidth;
	clRect.height = scaledImageHeight;
}

void mDraw(NewCanvasX* canvas)
{

	canvas->BeginDraw();

	canvas->Clear({0.0f,0.0f,0.0f,0.0f});

	

	//draw here--------------------------------------------------------

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
		
		float offsetX, offsetY;

		offsetX = offsetNumX * hinaScale * hinaScale;
		offsetY = -offsetNumY * hinaScale * hinaScale;
 
		//hinaScale += 0.0005;

		Image* currImg = hinature->GetCurrentImage();

		float drawX = clRect.x + (clRect.width * 0.5f) - (currImg->width * hinaScale * 0.5f) + offsetX;
		float drawY = clRect.y + (clRect.height * 0.5f) - (currImg->height * hinaScale * 0.5f) + offsetY;

		if (currImg) 
		{
			if (currImg->isValid())
			{
				canvas->DrawImg(*currImg, drawX, drawY, nullptr, 0.0f, hinaScale, hinaScale);
			}
		}
		ResizeClRect(currImg);

		if (clicked)
		{
			hinature->ChangeMood(Mood::EMBARRASSED);
		}
		else
		{
			hinature->ChangeMood(Mood::NEUTRAL);
		}

		Shapes::Circle drawCircle;
		drawCircle.radius = 10;
		drawCircle.x = drawX;
		drawCircle.y = drawY;

		//canvas->DrawCir(drawCircle, { 0,255,0,255 });

		Shapes::Circle rectCircle;
		drawCircle.radius = 10;
		drawCircle.x = clRect.x;
		drawCircle.y = clRect.y;

		//canvas->DrawCir(drawCircle, { 255,255,0,255 });

		Shapes::Rectangle imgRect;

		imgRect.x = drawX;
		imgRect.y = drawY;

		imgRect.width = currImg->width * hinaScale;
		imgRect.height = currImg->height * hinaScale;

		//canvas->DrawRectO(imgRect, { 255,0,0,255 });
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
	
	screenW = pWindow->GetScreenWidth();
	screenH = pWindow->GetScreenHeight();

	hinature = new Companion(pWindow->GetNewCanvas(), Variant::NIGHTWEAR);

	switch (hinature->GetCurrentVariant())
	{
		case Variant::BASE:
			offsetNumX = 30;
			offsetNumY = 40;
			scaleX = 0.17f;
			scaleY = 0.85f;
			break;
		case Variant::NIGHTWEAR:
			offsetNumX = -40;
			offsetNumY = 10;
			scaleX = 0.6;
			scaleY = 0.85;
			break;
		case Variant::SWIMSUIT:
			break;
		case Variant::DRESS:
			break;
	}

	ResizeClRect(hinature->GetCurrentImage());

	clRect.x = screenW - clRect.width;
	clRect.y = screenH - clRect.height;

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