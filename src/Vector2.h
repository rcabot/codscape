#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	int x{};
	int y{};


	Vector2 operator+(const Vector2& other) const;
	long hash() const;
};



