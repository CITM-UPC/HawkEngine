#include "Material.h"
#include <GL/glew.h>

static auto GLWrapMode(Material::WrapModes mode) {
	switch (mode) {
	case Material::Repeat: return GL_REPEAT;
	case Material::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case Material::Clamp: return GL_CLAMP_TO_EDGE;
	default: return GL_REPEAT;
	}
}

static auto GLMagFilter(Material::Filters filter) {
	switch (filter) {
	case Material::Nearest: return GL_NEAREST;
	case Material::Linear: return GL_LINEAR;
	default: return GL_NEAREST;
	}
}

static auto GLMinFilter(Material::Filters filter) {
	switch (filter) {
	case Material::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
	case Material::Linear: return GL_LINEAR_MIPMAP_LINEAR;
	default: return GL_NEAREST_MIPMAP_LINEAR;
	}
}

void Material::bind() const {
	glBindTexture(GL_TEXTURE_2D, imagePtr->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLWrapMode(wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLWrapMode(wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLMinFilter(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLMagFilter(filter));
}

bool Material::loadShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
	return shader.LoadShaders(vertexShaderFile, fragmentShaderFile);
}

// Function to bind shaders
void Material::bindShaders() const {
	shader.Bind();
}

// Function to set shader uniforms
void Material::setShaderUniform(const std::string& name, int value) {
	shader.SetUniform(name, value);
}

void Material::setShaderUniform(const std::string& name, float value) {
	shader.SetUniform(name, value);
}

void Material::setShaderUniform(const std::string& name, const glm::vec3& value) {
	shader.SetUniform(name, value);
}

void Material::setShaderUniform(const std::string& name, const glm::vec4& value) {
	shader.SetUniform(name, value);
}

void Material::setShaderUniform(const std::string& name, const glm::mat4& value) {
	shader.SetUniform(name, value);
}