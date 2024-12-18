#pragma once

#include <memory>
#include "Image.h"
#include "Shaders.h"
#include "types.h"

class Material
{
public:
	Material();
	enum WrapModes { Repeat, MirroredRepeat, Clamp };
	WrapModes wrapMode = Repeat;

	enum Filters { Nearest, Linear };
	Filters filter = Nearest;

	Shaders shader;
	vec4 color;

	bool useShader = false;

	unsigned int GetId() const { return gid; }
	void LoadTexture(const std::string& filename);
	unsigned int id() const { return imagePtr ? imagePtr->id() : 0; }
	void bind() const;
	void setImage(const std::shared_ptr<Image>& img_ptr) { imagePtr = img_ptr; }
	const auto& image() const { return *imagePtr; }
	std::shared_ptr<Image> getImg() { return imagePtr; }
	bool loadShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	void bindShaders() const;
	void setShaderUniform(const std::string& name, int value);
	void setShaderUniform(const std::string& name, float value);
	void setShaderUniform(const std::string& name, const glm::vec3& value);
	void setShaderUniform(const std::string& name, const glm::vec4& value);
	void setShaderUniform(const std::string& name, const glm::mat4& value);
	vec4 GetColor();
	void SetColor(const vec4& color);
	void SetShader(Shaders& shader);
	Shaders GetShader();

	void SaveBinary(const std::string& filename) const;
	static std::shared_ptr<Material> LoadBinary(const std::string& filename);

private:


	std::shared_ptr<Image> imagePtr;
	std::string image_path;

	unsigned int gid;
	static unsigned int next_id;

};

