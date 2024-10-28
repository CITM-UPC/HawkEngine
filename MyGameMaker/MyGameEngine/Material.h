#pragma once
#include <string>

class Material
{
public:
	Material() = default;

	bool LoadTexture(const std::string& file_path, auto mesh);
	void Bind() const;

	unsigned int GetTextureID() const { return textureID; }

private:
	unsigned int textureID = 0;
	unsigned int _id = 0;
	void CreateDefaultTexture();
};

