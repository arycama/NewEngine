#pragma once

struct Rect
{
	Rect(int left, int top, int right, int bottom);
	
	int left, top, right, bottom;

	int GetWidth() const;
	int GetHeight() const;
};