#pragma once

#include <string>

class TgaLoader
{
public:
	static char* LoadFile(const std::string& path, int& width, int& height);
};