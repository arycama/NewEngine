#include "TgaLoader.h"

#include <assert.h>
#include <stdio.h>

using namespace std;

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

unsigned char* TgaLoader::LoadFile(const string& path, unsigned int& width, unsigned int& height)
{
	FILE* filePtr;
	auto error = fopen_s(&filePtr, path.c_str(), "rb");
	assert(!error);

	TargaHeader tgaHeader;
	auto headerSize = fread(&tgaHeader, sizeof(TargaHeader), 1, filePtr);

	width = tgaHeader.width;
	height = tgaHeader.height;

	auto imageSize = tgaHeader.width * tgaHeader.height * 4;
	auto imageData = new unsigned char[imageSize];

	auto pixelCount = fread(imageData, 1, imageSize, filePtr);
	assert(pixelCount == imageSize);

	auto closeError = fclose(filePtr);
	assert(!closeError);

	return imageData;
}