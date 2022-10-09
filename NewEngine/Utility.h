#pragma once

#include <winerror.h>
#include <string>

class Utility
{
public:
	static void ThrowIfFailed(HRESULT hresult, const std::string& message);
};

