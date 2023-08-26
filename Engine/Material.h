#pragma once

class Material
{
public:
	Material(const class Shader& shader, const class Texture& texture);

	void Render() const;

private:
	const class Shader& shader;
	const class Texture& texture; // Todo this should probably be a vector
};