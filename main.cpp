#include <iostream>
#include "window.h"
#include <wincodec.h>
#include "input.h"
#include "companion.h"
#include "uiElement.h"

Shapes::Rectangle clRect;

Color rectCl = { 255,255,0,50};

Companion* hinature = nullptr;

float hinaScale = 1.0f;

Variant newVariant;

Input* globalInput = nullptr;

bool clicked = false;

bool dragging = false;

float screenW, screenH;

float rectScaleX, rectScaleY;

float offsetNumX, offsetNumY;

Button scaleUpBtn;
Button scaleDownBtn;

void ResizeClRect(Image* img)
{
	float scaledImageWidth = img->width * hinaScale * hinaScale * rectScaleX;
	float scaledImageHeight = img->height * hinaScale * hinaScale * rectScaleY;

	clRect.width = scaledImageWidth;
	clRect.height = scaledImageHeight;
}

void SetBtnPos(Button& btn, Shapes::Rectangle& rect, float offX = 0.0f, float offY = 0.0f)
{
	btn.button.x = rect.x + rect.width + offX;
	btn.button.y = rect.y + offY;

	btn.element.hitbox.x = btn.button.x - btn.button.radius;
	btn.element.hitbox.y = btn.button.y - btn.button.radius;
}

void UpdateScaleForVariant()
{
	switch (hinature->GetCurrentVariant())
	{
	case Variant::BASE:
		offsetNumX = 30;
		offsetNumY = 40;
		rectScaleX = 0.17f;
		rectScaleY = 0.85f;
		break;
	case Variant::NIGHTWEAR:
		offsetNumX = -20;
		offsetNumY = 35;
		rectScaleX = 0.4;
		rectScaleY = 0.85;
		break;
	case Variant::SWIMSUIT:
		break;
	case Variant::DRESS:
		break;
	}

	ResizeClRect(hinature->GetCurrentImage());
}

void Update()
{
	newVariant = Variant::NIGHTWEAR;

	if ((hinature->GetCurrentVariant() != newVariant) && globalInput->IsKeyPressed(VK_2))
	{
		hinature->ChangeVariant(newVariant);

		UpdateScaleForVariant();
	}

	if (clicked)
	{
		hinature->ChangeMood(Mood::EMBARRASSED, 1.0f);
	}

	hinature->UpdateTimer();
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

		canvas->DrawCir(scaleUpBtn.button, scaleUpBtn.element.uiColor);
		canvas->DrawCir(scaleDownBtn.button, scaleUpBtn.element.uiColor);


		//debug draw (temporal)

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
	dragging = false;

	Window* pWindow = new Window(1200, 680);
	
	screenW = pWindow->GetScreenWidth();
	screenH = pWindow->GetScreenHeight();

	hinature = new Companion(pWindow->GetNewCanvas(), Variant::BASE);

	UpdateScaleForVariant();

	clRect.x = screenW - clRect.width;
	clRect.y = screenH - clRect.height;

	pWindow->SetNewDrawCallback(mDraw);

	globalInput = new Input(pWindow->GetHWND());

	scaleUpBtn = Button();
	scaleUpBtn.button.radius = 15;
	SetBtnPos(scaleUpBtn, clRect, scaleUpBtn.button.radius * 2, scaleUpBtn.button.radius * 2);
	scaleUpBtn.element.hitbox = CalculateBtnRect(scaleUpBtn.button);
	scaleUpBtn.element.uiColor = { 100,100,255,0 };

	scaleDownBtn = Button();
	scaleDownBtn.button.radius = 15;
	SetBtnPos(scaleDownBtn, clRect, scaleDownBtn.button.radius * 2, (scaleDownBtn.button.radius * 4) + 10);
	scaleDownBtn.element.hitbox = CalculateBtnRect(scaleDownBtn.button);
	scaleDownBtn.element.uiColor = { 100,100,255,0 };

	bool running = true;

	while (running)
	{
		running = pWindow->ProcessMessages();

		globalInput->Update();

		Vector2D mousePos = globalInput->GetMousePosition();

		bool mouseInClickableArea = IsPointOnRect(mousePos, clRect);

		bool mouseInScaleUpBtnArea = IsPointOnRect(mousePos, scaleUpBtn.element.hitbox);
		bool mouseInScaleDownBtnArea = IsPointOnRect(mousePos, scaleDownBtn.element.hitbox);

		bool leftMousePressed = globalInput->IsLeftMousePressed();

		bool leftMouseHeld = globalInput->IsMiddleMouseHeld();

		bool middleMousePressed = globalInput->IsMiddleMousePressed();

		bool middleMouseReleased = globalInput->IsMiddleMouseReleased();

		if (mouseInClickableArea || mouseInScaleUpBtnArea|| mouseInScaleDownBtnArea)
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

			SetWindowPos(pWindow->GetHWND(), nullptr, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}

		if (mouseInClickableArea && leftMousePressed)
		{
			clicked = true;
		}
		else
		{
			clicked = false;
		}

		if (mouseInClickableArea && middleMousePressed)
		{
			dragging = true;
		}
		else if(middleMouseReleased)
		{
			dragging = false;
		}

		if (mouseInScaleDownBtnArea ||mouseInScaleUpBtnArea)
		{
			scaleUpBtn.element.uiColor.a = 180;
			scaleDownBtn.element.uiColor.a = 180;
		}
		else
		{
			scaleUpBtn.element.uiColor.a = 50;
			scaleDownBtn.element.uiColor.a = 50;
		}

		if (mouseInScaleUpBtnArea && leftMousePressed)
		{
			hinaScale += 0.01;
			ResizeClRect(hinature->GetCurrentImage());
			SetBtnPos(scaleUpBtn, clRect, scaleUpBtn.button.radius * 2, scaleUpBtn.button.radius * 2);
			SetBtnPos(scaleDownBtn, clRect, scaleDownBtn.button.radius * 2, (scaleDownBtn.button.radius * 4) + 10);
		}

		if (mouseInScaleDownBtnArea && leftMousePressed)
		{
			hinaScale -= 0.01;
			ResizeClRect(hinature->GetCurrentImage());
			SetBtnPos(scaleUpBtn, clRect, scaleUpBtn.button.radius * 2, scaleUpBtn.button.radius * 2);
			SetBtnPos(scaleDownBtn, clRect, scaleDownBtn.button.radius * 2, (scaleDownBtn.button.radius * 4) + 10);
		}

		if (dragging)
		{
			clRect.x = (mousePos.x - clRect.width * 0.5f);
			clRect.y = (mousePos.y - clRect.height * 0.25f);

			SetBtnPos(scaleUpBtn, clRect, scaleUpBtn.button.radius * 2, scaleUpBtn.button.radius * 2);
			SetBtnPos(scaleDownBtn, clRect, scaleDownBtn.button.radius * 2, (scaleDownBtn.button.radius * 4) + 10);
		}

		
		Update();

		Sleep(20);
	}

	delete hinature;
	delete globalInput;
	delete pWindow;

	return 0;
}