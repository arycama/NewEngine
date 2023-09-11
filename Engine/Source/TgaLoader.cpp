#include "TgaLoader.h"

#include <assert.h>
#include <fstream>

using namespace std;

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

unique_ptr<unsigned char[]> TgaLoader::LoadFile(const string& path, unsigned int& width, unsigned int& height)
{
	ifstream file(path);
	assert(file.is_open());

	TargaHeader tgaHeader;
	file.read((char*)&tgaHeader, sizeof(TargaHeader));

	width = tgaHeader.width;
	height = tgaHeader.height;

	auto imageSize = width * height * 4;
	auto imageData = make_unique<unsigned char[]>(imageSize);

	file.read((char*)imageData.get(), imageSize);
	return imageData;
}