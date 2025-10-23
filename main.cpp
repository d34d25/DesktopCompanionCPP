#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "window.h"
#include <wincodec.h>
#include "input.h"
#include "companion.h"
#include "uiElement.h"

void CreateConsole();

void DeleteConsole();

void ShowConsole();

void HideConsole();

Shapes::Rectangle clRect;

Color rectCl = { 255,255,0,50 };

Companion* hinature = nullptr;

float hinaScale = 1.0f;

Variant newVariant;

bool clickedInHead = false;
bool clickedInChest = false;
bool clickedInBelly = false;
bool clickedInThighs = false;
bool clickedInLegs = false;

Input* globalInput = nullptr;

bool clicked = false;

bool dragging = false;

float screenW, screenH;

float rectScaleX, rectScaleY;

float offsetNumX, offsetNumY;

Button scaleUpBtn;
Button scaleDownBtn;

Button baseBtn;
Button nightwearBtn;
Button dressBtn;
Button swimsuitBtn;

Button closeBtn;

ConfigFile config;

void CreateConsole()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

void DeleteConsole()
{
	FreeConsole();
}

void ShowConsole()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

void HideConsole()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}


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
		offsetNumX = config.base_clRect_offset_x;
		offsetNumY = config.base_clRect_offset_y;
		rectScaleX = config.base_clRect_scale_x;
		rectScaleY = config.base_clRect_scale_y;
		break;
	case Variant::NIGHTWEAR:
		offsetNumX = config.night_clRect_offset_x;
		offsetNumY = config.night_clRect_offset_y;
		rectScaleX = config.night_clRect_scale_x;
		rectScaleY = config.night_clRect_scale_y;
		break;
	case Variant::DRESS:
		offsetNumX = config.dress_clRect_offset_x;
		offsetNumY = config.dress_clRect_offset_y;
		rectScaleX = config.dress_clRect_scale_x;
		rectScaleY = config.dress_clRect_scale_y;
		break;
	case Variant::SWIMSUIT:
		offsetNumX = config.swim_clRect_offset_x;
		offsetNumY = config.swim_clRect_offset_y;
		rectScaleX = config.swim_clRect_scale_x;
		rectScaleY = config.swim_clRect_scale_y;
		break;
	}

	ResizeClRect(hinature->GetCurrentImage());
}

void Update()
{
	if (hinature)
	{
		Place currentPlace = Place::NONE;
		Mood newMood = Mood::NEUTRAL;

		if (clickedInHead)
		{
			currentPlace = Place::HEAD;
			newMood = hinature->PickMoodByProbabilty(hinature->headMoods);
			hinature->ChangeMood(newMood, currentPlace, 1.0f);
		}
		else if (clickedInChest)
		{
			currentPlace = Place::CHEST;
			newMood = hinature->PickMoodByProbabilty(hinature->chestMoods);
			hinature->ChangeMood(newMood, currentPlace, 1.0f);
		}
		else if (clickedInBelly)
		{
			currentPlace = Place::BELLY;
			newMood = hinature->PickMoodByProbabilty(hinature->bellyMoods);
			hinature->ChangeMood(newMood, currentPlace, 1.0f);
		}
		else if (clickedInThighs)
		{
			currentPlace = Place::THIGHS;
			newMood = hinature->PickMoodByProbabilty(hinature->thighsMoods);
			hinature->ChangeMood(newMood, currentPlace, 1.0f);
		}
		else if (clickedInLegs)
		{
			currentPlace = Place::LEGS;
			newMood = hinature->PickMoodByProbabilty(hinature->legMoods);
			hinature->ChangeMood(newMood, currentPlace, 1.0f);
		}
		else if (clicked)
		{
			currentPlace = Place::NONE;
			hinature->ChangeMood(newMood, currentPlace, 1.0f);
		}

		hinature->UpdateTimer(&currentPlace);
	}

}



void mDraw(NewCanvasX* canvas)
{

	canvas->BeginDraw();

	canvas->Clear({ 0.0f,0.0f,0.0f,0.0f });

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

		if (currImg)
		{
			float drawX = clRect.x + (clRect.width * 0.5f) - (currImg->width * hinaScale * 0.5f) + offsetX;
			float drawY = clRect.y + (clRect.height * 0.5f) - (currImg->height * hinaScale * 0.5f) + offsetY;

			if (currImg->isValid())
			{
				canvas->DrawImg(*currImg, drawX, drawY, nullptr, 0.0f, hinaScale, hinaScale);
			}

			hinature->DrawDialog(&clRect, hinaScale);
		}

		canvas->DrawCir(scaleUpBtn.button, scaleUpBtn.element.uiColor);

		Shapes::Rectangle scaleUpTextArea = SetButtonTextArea(scaleUpBtn,
			scaleUpBtn.button.radius * 0.5f - 1.0f,
			-scaleUpBtn.button.radius * 0.25);

		canvas->DrawTxt(scaleUpBtn.text, scaleUpTextArea, { 255,255,255, scaleUpBtn.element.uiColor.a });

		canvas->DrawCir(scaleDownBtn.button, scaleUpBtn.element.uiColor);

		Shapes::Rectangle scaleDownTextArea = SetButtonTextArea(scaleDownBtn,
			scaleDownBtn.button.radius * 0.5f + 2.5f,
			-scaleDownBtn.button.radius * 0.25f);

		canvas->DrawTxt(scaleDownBtn.text, scaleDownTextArea, { 255,255,255, scaleDownBtn.element.uiColor.a });

		canvas->DrawCir(baseBtn.button, baseBtn.element.uiColor);

		Shapes::Rectangle baseBtnTextArea = SetButtonTextArea(baseBtn,
			baseBtn.button.radius * 0.5f,
			-baseBtn.button.radius * 0.25f);

		canvas->DrawTxt(baseBtn.text, baseBtnTextArea, { 255,255,255,baseBtn.element.uiColor.a });

		canvas->DrawCir(nightwearBtn.button, nightwearBtn.element.uiColor);

		Shapes::Rectangle nightwearBtnTextArea = SetButtonTextArea(nightwearBtn,
			nightwearBtn.button.radius * 0.5f,
			-nightwearBtn.button.radius * 0.25f);
		canvas->DrawTxt(nightwearBtn.text, nightwearBtnTextArea, { 255,255,255, nightwearBtn.element.uiColor.a });

		canvas->DrawCir(closeBtn.button, closeBtn.element.uiColor);

		Shapes::Rectangle closeBtnTextArea = SetButtonTextArea(closeBtn,
			closeBtn.button.radius * 0.5f,
			-closeBtn.button.radius * 0.25f);
		canvas->DrawTxt(closeBtn.text, closeBtnTextArea, { 255,255,255,closeBtn.element.uiColor.a });

		canvas->DrawCir(dressBtn.button, dressBtn.element.uiColor);

		Shapes::Rectangle dressBtnTextArea = SetButtonTextArea(dressBtn,
			dressBtn.button.radius * 0.5f,
			-dressBtn.button.radius * 0.25f);
		canvas->DrawTxt(dressBtn.text, dressBtnTextArea, { 255,255,255,dressBtn.element.uiColor.a });

		canvas->DrawCir(swimsuitBtn.button, swimsuitBtn.element.uiColor);

		Shapes::Rectangle swimsuitBtnTextArea = SetButtonTextArea(swimsuitBtn,
			swimsuitBtn.button.radius * 0.5f,
			-swimsuitBtn.button.radius * 0.25f);
		canvas->DrawTxt(swimsuitBtn.text, swimsuitBtnTextArea, { 255,255,255,swimsuitBtn.element.uiColor.a });


		

	}

	//debug drawing

	if (config.debug_draw)
	{
		canvas->DrawRectO(hinature->regions.head, { 255,0,255,255 });
		canvas->DrawRectO(hinature->regions.chest, { 255,255,0,255 });
		canvas->DrawRectO(hinature->regions.belly, { 255,0,0,255 });
		canvas->DrawRectO(hinature->regions.thighs, { 0,0,255,255 });
		canvas->DrawRectO(hinature->regions.legs, { 0,255,0,255 });

		canvas->DrawRect(clRect, rectCl);

		ShowConsole();
	}
	


	//--------------------------------------------------------------------

	canvas->EndDraw();

	canvas->Present();

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CreateConsole();

	HideConsole();

	config = LoadConfigFile("../config.txt");

	dragging = false;

	Window* pWindow = new Window(1200, 680);

	screenW = pWindow->GetScreenWidth();
	screenH = pWindow->GetScreenHeight();

	hinature = new Companion(pWindow->GetNewCanvas(), Variant::BASE, &config);

	UpdateScaleForVariant();

	clRect.x = screenW - clRect.width;
	clRect.y = screenH - clRect.height;

	hinature->UpdateRegions(clRect);

	pWindow->SetNewDrawCallback(mDraw);

	globalInput = new Input(pWindow->GetHWND());

	int btnRadius = 15;

	scaleUpBtn = Button();
	scaleUpBtn.button.radius = btnRadius;

	const int buttonSpacing = 10;
	const int buttonDiameter = btnRadius * 2;

	int baseY = buttonDiameter + buttonSpacing;

	scaleUpBtn.element.hitbox = CalculateBtnRect(scaleUpBtn.button);
	scaleUpBtn.element.uiColor = { 100,100,255,50 };
	scaleUpBtn.text = "+";

	scaleDownBtn = Button();
	scaleDownBtn.button.radius = btnRadius;
	scaleDownBtn.element.hitbox = CalculateBtnRect(scaleDownBtn.button);
	scaleDownBtn.element.uiColor = { 100,100,255,50 };
	scaleDownBtn.text = "-";

	baseBtn = Button();
	baseBtn.button.radius = btnRadius;
	baseBtn.element.hitbox = CalculateBtnRect(baseBtn.button);
	baseBtn.element.uiColor = { 255,100,255,50 };
	baseBtn.text = "0";

	nightwearBtn = Button();
	nightwearBtn.button.radius = btnRadius;
	nightwearBtn.element.hitbox = CalculateBtnRect(nightwearBtn.button);
	nightwearBtn.element.uiColor = { 255,255,100,50 };
	nightwearBtn.text = "1";

	dressBtn = Button();
	dressBtn.button.radius = btnRadius;
	dressBtn.element.hitbox = CalculateBtnRect(dressBtn.button);
	dressBtn.element.uiColor = { 94,0,148,50 };
	dressBtn.text = "2";

	swimsuitBtn = Button();
	swimsuitBtn.button.radius = btnRadius;
	swimsuitBtn.element.hitbox = CalculateBtnRect(swimsuitBtn.button);
	swimsuitBtn.element.uiColor = { 0,0,100,50 };
	swimsuitBtn.text = "3";

	closeBtn = Button();
	closeBtn.button.radius = btnRadius;
	closeBtn.element.hitbox = CalculateBtnRect(closeBtn.button);
	closeBtn.element.uiColor = { 255,0,0,50 };
	closeBtn.text = "X";

	// First button at y = 30 + 10 = 40
	SetBtnPos(scaleUpBtn, clRect, buttonDiameter, baseY);
	SetBtnPos(scaleDownBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 1);
	SetBtnPos(baseBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 2);
	SetBtnPos(nightwearBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 3);
	SetBtnPos(dressBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 4);
	SetBtnPos(swimsuitBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 5);
	SetBtnPos(closeBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 6);

	auto UpdateButtonsAndRect = [&]() {
		ResizeClRect(hinature->GetCurrentImage());
		SetBtnPos(scaleUpBtn, clRect, buttonDiameter, baseY);
		SetBtnPos(scaleDownBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 1);
		SetBtnPos(baseBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 2);
		SetBtnPos(nightwearBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 3);
		SetBtnPos(dressBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 4);
		SetBtnPos(swimsuitBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 5);
		SetBtnPos(closeBtn, clRect, buttonDiameter, baseY + (buttonDiameter + buttonSpacing) * 6);
		hinature->UpdateRegions(clRect);
	};

	bool running = true;

	while (running)
	{
		running = pWindow->ProcessMessages();

		globalInput->Update();

		Vector2D mousePos = globalInput->GetMousePosition();

		bool mouseInClickableArea = IsPointOnRect(mousePos, clRect);

		bool mouseInScaleUpBtnArea = IsPointOnRect(mousePos, scaleUpBtn.element.hitbox);
		bool mouseInScaleDownBtnArea = IsPointOnRect(mousePos, scaleDownBtn.element.hitbox);

		bool mouseInBaseBtnArea = IsPointOnRect(mousePos, baseBtn.element.hitbox);
		bool mouseInNightwearBtnArea = IsPointOnRect(mousePos, nightwearBtn.element.hitbox);
		bool mouseInDressBtnArea = IsPointOnRect(mousePos, dressBtn.element.hitbox);
		bool mouseInSwimsuitBtnArea = IsPointOnRect(mousePos, swimsuitBtn.element.hitbox);

		bool mouseInCloseBtnArea = IsPointOnRect(mousePos, closeBtn.element.hitbox);

		bool mouseInButton = mouseInScaleUpBtnArea || mouseInScaleDownBtnArea ||
			mouseInBaseBtnArea || mouseInNightwearBtnArea || mouseInDressBtnArea ||
			mouseInSwimsuitBtnArea || mouseInCloseBtnArea;

		bool leftMousePressed = globalInput->IsLeftMousePressed();

		bool leftMouseHeld = globalInput->IsMiddleMouseHeld();

		bool middleMousePressed = globalInput->IsMiddleMousePressed();

		bool middleMouseReleased = globalInput->IsMiddleMouseReleased();

		clickedInHead = IsPointOnRect(mousePos, hinature->regions.head) && leftMousePressed;
		clickedInChest = IsPointOnRect(mousePos, hinature->regions.chest) && leftMousePressed;
		clickedInBelly = IsPointOnRect(mousePos, hinature->regions.belly) && leftMousePressed;
		clickedInThighs = IsPointOnRect(mousePos, hinature->regions.thighs) && leftMousePressed;
		clickedInLegs = IsPointOnRect(mousePos, hinature->regions.legs) && leftMousePressed;


		if (mouseInClickableArea || mouseInButton)
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
		else if (middleMouseReleased)
		{
			dragging = false;
		}

		if (mouseInButton)
		{
			scaleUpBtn.element.uiColor.a = 180;
			scaleDownBtn.element.uiColor.a = 180;

			baseBtn.element.uiColor.a = 180;
			nightwearBtn.element.uiColor.a = 180;
			dressBtn.element.uiColor.a = 180;
			swimsuitBtn.element.uiColor.a = 180;

			closeBtn.element.uiColor.a = 180;
		}
		else
		{
			scaleUpBtn.element.uiColor.a = 50;
			scaleDownBtn.element.uiColor.a = 50;

			baseBtn.element.uiColor.a = 50;
			nightwearBtn.element.uiColor.a = 50;
			dressBtn.element.uiColor.a = 50;
			swimsuitBtn.element.uiColor.a = 50;

			closeBtn.element.uiColor.a = 50;
		}

		if (mouseInScaleUpBtnArea && leftMousePressed)
		{
			hinaScale += 0.01;
			UpdateButtonsAndRect();

		}

		if (mouseInScaleDownBtnArea && leftMousePressed)
		{
			hinaScale -= 0.01;
			UpdateButtonsAndRect();
		}

		if (mouseInBaseBtnArea && leftMousePressed)
		{
			hinature->ChangeVariant(Variant::BASE);

			UpdateScaleForVariant();

			UpdateButtonsAndRect();
		}

		if (mouseInNightwearBtnArea && leftMousePressed)
		{
			hinature->ChangeVariant(Variant::NIGHTWEAR);

			UpdateScaleForVariant();

			UpdateButtonsAndRect();
		}

		if (mouseInDressBtnArea && leftMousePressed)
		{
			hinature->ChangeVariant(Variant::DRESS);

			UpdateScaleForVariant();

			UpdateButtonsAndRect();
		}

		if (mouseInSwimsuitBtnArea && leftMousePressed)
		{
			hinature->ChangeVariant(Variant::SWIMSUIT);

			UpdateScaleForVariant();

			UpdateButtonsAndRect();
		}

		if (dragging)
		{
			clRect.x = (mousePos.x - clRect.width * 0.5f);
			clRect.y = (mousePos.y - clRect.height * 0.25f);

			UpdateButtonsAndRect();
		}

		if (mouseInCloseBtnArea && leftMousePressed)
		{
			PostMessage(pWindow->GetHWND(), WM_CLOSE, 0, 0);
		}

		Update();

		Sleep(20);
	}

	
	DeleteConsole();
	delete hinature;
	delete globalInput;
	delete pWindow;

	return 0;
}
