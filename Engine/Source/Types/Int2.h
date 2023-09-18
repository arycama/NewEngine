#pragma once

struct Int2
{
	Int2(int x, int y);
	int x, y;

	Int2 operator+(const Int2& p) const;

	Int2 operator-(const Int2& p) const;

	Int2 operator*(const Int2& p) const;

	Int2 operator/(const Int2& p) const;
};