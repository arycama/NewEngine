#include "Texture.h"

#include <comdef.h>
#include <cstdio>

using namespace _com_util;
using namespace Microsoft::WRL;
using namespace std;

Texture::Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const string& filename)
{
	int height, width;

	// Load the targa image data into memory.
	LoadTarga(filename, height, width);

	// Setup the description of the texture.
	const auto textureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS);

	// Create the empty texture.
	CheckError(device.CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()));

	// Set the row pitch of the targa image data.
	const unsigned int rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext.UpdateSubresource(texture.Get(), 0, nullptr, targaData, rowPitch, 0);

	// Setup the shader resource view description.
	const auto srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format, 0, -1);

	// Create the shader resource view for the texture.
	CheckError(device.CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf()));

	// Generate mipmaps for this texture.
	deviceContext.GenerateMips(textureView.Get());

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] targaData;
	targaData = nullptr;
}

ID3D11ShaderResourceView& Texture::GetTexture() const
{
	return *textureView.Get();
}

void Texture::LoadTarga(const std::string& filename, int& height, int& width)
{
	FILE* filePtr;
	TargaHeader targaFileHeader;

	// Open the targa file for reading in binary.
	int error = fopen_s(&filePtr, filename.c_str(), "rb");
	if(error != 0)
	{
		return;
	}

	// Read in the file header.
	auto count = static_cast<unsigned int>(fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr));
	if(count != 1)
	{
		return;
	}

	// Get the important information from the header.
	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);
	const int bpp = targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if(bpp != 32)
	{
		return;
	}

	// Calculate the size of the 32 bit image data.
	const int imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	auto* targaImage = new unsigned char[imageSize];

	// Read in the targa image data.
	count = static_cast<unsigned int>(fread(targaImage, 1, imageSize, filePtr));
	if(count != imageSize)
	{
		return;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return;
	}

	// Allocate memory for the targa destination data.
	targaData = new unsigned char[imageSize];

	// Initialize the index into the targa destination data array.
	int index = 0;

	// Initialize the index into the targa image data.
	int k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for(int j = 0; j<height; j++)
	{
		for(int i = 0; i<width; i++)
		{
			targaData[index + 0] = targaImage[k + 2];  // Red.
			targaData[index + 1] = targaImage[k + 1];  // Green.
			targaData[index + 2] = targaImage[k + 0];  // Blue
			targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete [] targaImage;
	targaImage = nullptr;
}
