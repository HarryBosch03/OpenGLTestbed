#pragma once

#include "Graphics.h"
#include "Asset.h"

#include <string>
#include <map>

class ShaderProgram;

enum class TextureFiltering
{
	Linear = GL_LINEAR,
	LinearMipmap = GL_LINEAR_MIPMAP_LINEAR,
	Nearest = GL_NEAREST,
	NearestMipmap = GL_NEAREST_MIPMAP_NEAREST,
};

struct TextureImportSettings
{
	TextureFiltering filtering = TextureFiltering::LinearMipmap;
	bool useMipmaps = true;
};

class Texture : public Asset
{
	GLuint handle = 0;

	int width = 0, height = 0, channels = 0;
	bool initalized = false;

	std::string ref;

	TextureImportSettings settings;

	void PassDataToGL(void* data, GLenum type, GLint internalFormat, const std::string& fileLoc, const TextureImportSettings& settings);

public:

	inline const GLuint Handle() { return handle; }

	Texture() = default;
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	~Texture();

	inline AssetType GetType() const override { return AssetType::Texture; }
	inline const std::string& Ref() { return ref; }

	void Orphan(int width, int height, GLint internalFormat = GL_RGBA, GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	Asset& LoadFromFile(const std::string& fileLoc, void* args) override;
	Asset& Reload() override;
	static bool DoesFileMatch(const std::string& fileloc);

	void Bind(const std::string& ref);
	void Unbind();

	inline const int& Width() const { return width; }
	inline const int& Height() const { return height; }
	inline const int& Channels() const { return channels; }

	static void BindAll();
	static void UnbindAll();

	static const std::map<std::string, Texture*>& BoundTextures();
};