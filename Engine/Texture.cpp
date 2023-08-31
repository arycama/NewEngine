#include "Texture.h"
#include "DirectXHelpers.h"
#include "TgaLoader.h"

#include <d3d11.h>
#include <memory>
#include <dxgiformat.h>
#include <wrl/client.h>
#include <comdef.h>
#include <string>

using namespace _com_util;
using namespace Microsoft::WRL;
using namespace std;

Texture::Texture(const string& path, ID3D11Device& device, ID3D11DeviceContext& deviceContext)
{
	int width, height;
	auto imageData = unique_ptr<char[]>(TgaLoader::LoadFile(path, width, height));

	auto textureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, width, height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS);
	CheckError(device.CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()));

	auto rowPitch = (width * 4) * sizeof(char);
	deviceContext.UpdateSubresource(texture.Get(), 0, nullptr, imageData.get(), rowPitch, 0);

	auto srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format, 0, -1);
	CheckError(device.CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf()));

	deviceContext.GenerateMips(textureView.Get());
}

ID3D11ShaderResourceView& Texture::GetTexture() const
{
	return *textureView.Get();
}
