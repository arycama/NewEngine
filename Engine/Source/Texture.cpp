#include "Texture.h"
#include "DirectXHelpers.h"
#include "TextureLoader.h"

#include <d3d11.h>
#include <memory>
#include <dxgiformat.h>
#include <wrl/client.h>
#include <comdef.h>
#include <string>

using namespace _com_util;
using namespace Microsoft::WRL;
using namespace std;

Texture::Texture(unsigned char* data, int width, int height, ID3D11Device& device, ID3D11DeviceContext& deviceContext)
{
	CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS);

	CheckError(device.CreateTexture2D(&desc, nullptr, texture.GetAddressOf()));

	auto rowPitch = (width * 4) * sizeof(char);
	deviceContext.UpdateSubresource(texture.Get(), 0, nullptr, data, rowPitch, 0);

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, desc.Format, 0, -1);
	CheckError(device.CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf()));

	deviceContext.GenerateMips(textureView.Get());
}

ID3D11ShaderResourceView& Texture::GetTexture() const
{
	return *textureView.Get();
}
