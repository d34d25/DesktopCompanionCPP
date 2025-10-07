#include "collisions.h"

bool IsPointOnRect(const Vector2D& point, const Shapes::Rectangle& rect)
{
    return (point.x >= rect.x && point.x <= rect.x + rect.width &&
        point.y >= rect.y && point.y <= rect.y + rect.height);
}
