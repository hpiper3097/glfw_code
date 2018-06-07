#ifndef TEXTURE_H
#define TEXTURE

#include "glad\glad.h"
#include "stb_image.h"
#include <memory>
#include <iostream>

class Texture
{
	GLuint id;

	const char* dataPath;
	unsigned char* data;
	int width, height, nrChannels;
	GLenum prefferedTarget;
	GLenum prefferedChannel;
public:

	Texture() noexcept
	{
		prefferedTarget = GL_TEXTURE_2D;
		glGenTextures(1, &id);
		data = nullptr;
		width, height, nrChannels = -1;
		dataPath = "NO PATH";
	}

	Texture(const char* path)
	{
		prefferedTarget = GL_TEXTURE_2D;
		glGenTextures(1, &id);
		glBindTexture(prefferedTarget, id);
		dataPath = path;
		glTexParameteri(prefferedTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(prefferedTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(prefferedTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(prefferedTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Load(path);
		glBindTexture(prefferedTarget, 0);
	}

	Texture(const char* path, GLint format)
	{
		prefferedTarget = GL_TEXTURE_2D;
		glGenTextures(1, &id);
		glBindTexture(prefferedTarget, id);
		dataPath = path;
		glTexParameteri(prefferedTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(prefferedTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(prefferedTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(prefferedTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Load(path, format);
		glBindTexture(prefferedTarget, 0);
	}

	Texture(const char* path, GLenum target)
	{
		glGenTextures(1, &id);
		glBindTexture(target, id);
		dataPath = path;
		prefferedTarget = target;
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Load(path, target);
		glBindTexture(prefferedTarget, 0);
	}

	Texture(const char* path, GLint format, GLenum target)
	{
		glGenTextures(1, &id);
		glBindTexture(target, id);
		dataPath = path;
		prefferedTarget = target;
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Load(path, format, target);
		glBindTexture(prefferedTarget, 0);
	}

	Texture(const char* path, GLint format, GLenum target, GLenum channel)
	{
		glGenTextures(1, &id);
		glActiveTexture(channel);
		glBindTexture(target, id);
		dataPath = path;
		prefferedTarget = target;
		prefferedChannel = channel;
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Load(path, format, target);
		glBindTexture(prefferedTarget, 0);
	}

	~Texture()
	{
		stbi_image_free(data);
	}

	Texture(Texture& tex) = delete; //no copying because unique pointer and why would you need to copy a texture
	Texture& operator=(Texture& tex) = delete;

	constexpr GLuint getID() const noexcept
	{
		return id;
	}

	constexpr int getWidth() const noexcept
	{
		return width;
	}

	constexpr int getHeight() const noexcept
	{
		return height;
	}

	constexpr int getnrChannels() const noexcept
	{
		return nrChannels;
	}

	std::string getPath() const noexcept //to read the information
	{
		return dataPath;
	}

	const bool IsLoaded() const noexcept
	{
		return data != nullptr;
	}

	constexpr void bind() const noexcept
	{
		glBindTexture(prefferedTarget, id);
	}

	void Load(const char* path)
	{
		data =stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR LOADING IMAGE\n";
		}
		stbi_image_free(data);
	}

	void Load(const char* path, GLint format)
	{
		data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR LOADING IMAGE\n";
		}
	}

	void Load(const char* path, GLenum target)
	{
		data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(target);
		}
		else
		{
			std::cout << "ERROR LOADING IMAGE\n";
		}
	}

	void Load(const char* path, GLint format, GLenum target)
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
};

#endif
