#ifndef TEXTURE_H
#define TEXTURE

#include "glad\glad.h"
#include "stb_image.h"
#include <memory>

class Texture
{
	GLuint id;
	std::unique_ptr<unsigned char> data;
	int width, height, nrChannels;
public:
	Texture();
	const GLuint getID() const
	{
		return id;
	}
	const bool IsLoaded() const
	{
		return data.get() != nullptr;
	}
	void Load(const char* path)
	{
		data = stbi_load(path, &width, &height, &nrChannels, 0);
	}
};

#endif
