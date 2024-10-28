#include "Material.h"
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <iostream> 
#include "Mesh.h"

static auto formatFromChannels(int channels) {
	switch (channels) {
	case 1: return GL_LUMINANCE;
	case 2: return GL_LUMINANCE_ALPHA;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	default: return GL_RGB;
	}
}

static int rowAlignment(int width, int channels) {
	const size_t rowSizeInBytes = static_cast<size_t>(width) * channels;
	if ((rowSizeInBytes % 8) == 0) return 8;
	if ((rowSizeInBytes % 4) == 0) return 4;
	if ((rowSizeInBytes % 2) == 0) return 2;
	return 1;
}

bool Material::LoadTexture(const std::string& filePath)
{
  
	auto img = ilGenImage();
	ilBindImage(img);
	ilLoadImage((const wchar_t*)filePath.c_str());
	auto width = ilGetInteger(IL_IMAGE_WIDTH);

	auto height = ilGetInteger(IL_IMAGE_HEIGHT);

	auto channels = ilGetInteger(IL_IMAGE_CHANNELS);
	auto data = ilGetData();


	if (!_id) glGenTextures(1, &_id);

	glBindTexture(GL_TEXTURE_2D, _id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, rowAlignment(width, channels));
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, formatFromChannels(channels), GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//now we can delete image from RAM
	ilDeleteImage(img);

    return true;
}


void Material::CreateDefaultTexture() 
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned char white[] = { 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white);

    // Configurar los parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // Desactivar la textura
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Material::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}