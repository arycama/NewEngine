#pragma once

#include <winerror.h>
#include <string>

class Utility
{
public:
	inline static void ThrowIfFailed(HRESULT hresult);
};

