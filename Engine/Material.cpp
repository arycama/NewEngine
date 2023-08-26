#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material(const Shader& shader, const Texture& texture) : shader(shader), texture(texture) { }

void Material::Render() const
{
	shader.Render(texture.GetTexture());
}