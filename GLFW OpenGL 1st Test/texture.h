#ifndef TEXTURE_H
#define TEXTURE

#include "glad\glad.h"
#include <string>

class Texture
{
	GLuint id;

	const char* dataPath;
	unsigned char* data;
	int width, height, nrChannels;
	GLint format;
	GLenum prefferedTarget;
	GLenum prefferedChannel;
public:

	Texture(const char* path, GLint format, GLenum target, GLenum channel);

	~Texture();

	Texture(Texture& tex) = delete; //no copying because unique pointer and why would you need to copy a texture
	Texture& operator=(Texture& tex) = delete;

	constexpr GLuint getID() const noexcept;

	constexpr int getWidth() const noexcept;

	constexpr int getHeight() const noexcept;

	constexpr int getnrChannels() const noexcept;

	std::string getPath() const noexcept;

	const bool IsLoaded() const noexcept;

	void bind() const;

	void Load(const char* path, GLint format, GLenum target);
};

#endif
