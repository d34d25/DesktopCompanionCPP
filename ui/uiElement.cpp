#include "uiElement.h"

Shapes::Rectangle CalculateBtnRect(Shapes::Circle& circle)
{
    Shapes::Rectangle aabb = {};

    aabb.x = circle.x - circle.radius;
    aabb.y = circle.y - circle.radius;
    aabb.width = circle.radius * 2.0f;
    aabb.height = circle.radius * 2.0f;

    return aabb;
}

Shapes::Rectangle SetButtonTextArea(const Button& button, float offsetX, float offsetY)
{
    Shapes::Rectangle btnTextArea;
    btnTextArea.x = button.element.hitbox.x + offsetX;
    btnTextArea.y = button.element.hitbox.y + offsetY;
    btnTextArea.width = button.element.hitbox.width;
    btnTextArea.height = button.element.hitbox.height;

    return btnTextArea;
}
