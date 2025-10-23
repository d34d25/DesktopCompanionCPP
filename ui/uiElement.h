#pragma once
#include "myTypes.h"

struct UI_Element
{
	Shapes::Rectangle hitbox;
	Color uiColor;
};

struct Button
{
	UI_Element element;
	Shapes::Circle button;
	std::string text;
};

struct TextArea
{
	UI_Element element;
	std::string text;
};

Shapes::Rectangle CalculateBtnRect(Shapes::Circle& circle);

Shapes::Rectangle SetButtonTextArea(const Button& button, float offsetX = 0.0f, float offsetY = 0.0f);