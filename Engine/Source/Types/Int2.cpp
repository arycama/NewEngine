#include "Int2.h"

Int2::Int2(int x, int y) : x(x), y(y) { }

Int2 Int2::operator+(const Int2& p) const
{
	return Int2(x + p.x, y + p.y);
}

Int2 Int2::operator-(const Int2& p) const
{
	return Int2(x - p.x, y - p.y);
}

Int2 Int2::operator*(const Int2& p) const
{
	return Int2(x * p.x, y * p.y);
}

Int2 Int2::operator/(const Int2& p) const
{
	return Int2(x / p.x, y / p.y);
}