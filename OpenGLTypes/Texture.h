#pragma once

#include <glad/glad.h>
#include "TextureParameters.h"
#include <iostream>

struct FTexImage
{
    GLint InternalFormat = GL_RGBA8;
    int Width{512};
	int Height{512};
    GLint Format = GL_RGB;
    GLenum DataType = GL_UNSIGNED_BYTE;
    unsigned char* Data = nullptr;
    GLint TextureType = GL_TEXTURE_2D;

    static void ResetToDefaults(FTexImage& ImageData)
    {
        ImageData.InternalFormat = GL_RGBA8;
        ImageData.Height = ImageData.Width = 512;
        ImageData.Format = GL_RGB;
        ImageData.Data = nullptr;
        ImageData.TextureType = GL_TEXTURE_2D;
        
    }
};

struct FImageLoader
{
    static FTexImage LoadImage(const std::string& ImagePath, unsigned char** PtrToData_OPTIONAL = nullptr);
    static void FreeImage(unsigned char* Data);
};

class Texture
{
private:
	unsigned int TextureID{};
    GLint TextureType = GL_TEXTURE_2D;
	int Width{};
	int Height{};
    
    static bool GetParameterAsKeyValuePair(int SettingID, const FTextureParameters& Parameters, std::pair<GLenum, GLint>& OUTKeyValuePair);

public:
	explicit Texture(GLint TextureType);
    explicit Texture(const FTexImage& ImageData, const FTextureParameters& Parameters, bool bShouldGenerateMipMaps = false);

	void Bind(unsigned int TextureUnit = 0) const;
	void UnBind() const;
	~Texture();

	inline const unsigned int& GetWidth()  const {return  Width; }
	inline const unsigned int& GetHeight() const { return Height; }
    inline const unsigned int& GetTextureID() const { return TextureID; }


    //Texture parameters with a value of 'EnumPropertyName::None' won't be set
    void SetParameters(const FTextureParameters& Parameters) const;

    void SetTexImage2D(const FTexImage& ImageData, bool bShouldGenerateMipMaps = false);
};