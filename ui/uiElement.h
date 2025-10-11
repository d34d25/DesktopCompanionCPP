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
};

struct TextArea
{
	UI_Element element;
	std::string text;
};

Shapes::Rectangle CalculateBtnRect(Shapes::Circle& circle);