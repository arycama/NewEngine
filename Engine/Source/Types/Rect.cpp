#include "Rect.h"

Rect::Rect(int left, int top, int right, int bottom) : left(left), top(top), right(right), bottom(bottom)
{
}

int Rect::GetWidth() const
{
	return right - left;
}

int Rect::GetHeight() const
{
	return bottom - top;
}