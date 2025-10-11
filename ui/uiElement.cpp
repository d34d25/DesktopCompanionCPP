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
