#pragma once

#include <string>

class TgaLoader
{
public:
	static unsigned char* LoadFile(const std::string& path, unsigned int& width, unsigned int& height);
};