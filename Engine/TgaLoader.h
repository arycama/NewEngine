#pragma once

#include <memory>
#include <string>

class TgaLoader
{
public:
	static std::unique_ptr<unsigned char[]> LoadFile(const std::string& path, unsigned int& width, unsigned int& height);
};