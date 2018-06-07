#include "texture.h"

#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* path, GLint format, GLenum target, GLenum channel)
{
	glGenTextures(1, &id);
	glActiveTexture(channel);
	glBindTexture(target, id);
	dataPath = path;
	prefferedTarget = target;
	prefferedChannel = channel;
	this->format = format;
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Load(path, format, target);
	glBindTexture(prefferedTarget, 0);
}

Texture::~Texture()
{
	stbi_image_free(data);
}

constexpr GLuint Texture::getID() const noexcept
{
	return id;
}

constexpr int Texture::getWidth() const noexcept
{
	return width;
}

constexpr int Texture::getHeight() const noexcept
{
	return height;
}

constexpr int Texture::getnrChannels() const noexcept
{
	return nrChannels;
}

std::string Texture::getPath() const noexcept //to read the information
{
	return dataPath;
}

const bool Texture::IsLoaded() const noexcept
{
	return data != nullptr;
}

void Texture::bind() const
{
	glBindTexture(prefferedTarget, id);
}

void Texture::Load(const char* path, GLint format, GLenum target)
{
	data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else
	{
		std::cout << "ERROR LOADING IMAGE\n";
	}
}