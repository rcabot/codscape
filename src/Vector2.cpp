#include "Vector2.h"
#include <math.h>

Vector2::Vector2() = default;

Vector2::Vector2(const int x, const int y) : x{ x }, y{ y }
{
}

Vector2 Vector2::operator+(const Vector2 & other) const
{
	return { x + other.x, y + other.y };
}

long Vector2::hash() const
{
	const auto a = static_cast<unsigned long>(x >= 0 ? 2 * static_cast<long>(x) : -2 * static_cast<long>(x) - 1);
	const auto b = static_cast<unsigned long>(y >= 0 ? 2 * static_cast<long>(y) : -2 * static_cast<long>(y) - 1);
	const auto c = static_cast<long>((a >= b ? a * a + a + b : a + b * b) / 2);
    return x < 0 && y < 0 || x >= 0 && y >= 0 ? c : -c - 1;
}

float Vector2::distance(const Vector2 other) const
{
	auto a{x-other.x};
	auto b{y-other.y};
	return sqrt(a*a + b*b); //todo
}
