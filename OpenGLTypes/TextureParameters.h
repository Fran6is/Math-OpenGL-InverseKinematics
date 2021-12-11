#pragma once

#include <glad/glad.h>

//Some texture properties in opengl using 'glTexParameter'
//Checkout https://docs.gl/gl3/glTexParameter and https://learnopengl.com/Getting-started/Textures

enum class ETextureType: GLenum
{
    None,
    Texture2D = GL_TEXTURE_2D,
    CubeMap = GL_TEXTURE_CUBE_MAP
};
enum class ETextureWrapping: GLint
{
    None,
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE
};

enum class ETextureResamplingFunction: GLint
{
    None,
    Linear = GL_LINEAR,
    NearestNeighbor = GL_NEAREST
};

struct FTextureParameters
{
    ETextureType TextureType = ETextureType::Texture2D;
    ETextureWrapping WrapS = ETextureWrapping::Repeat;
    ETextureWrapping WrapT = ETextureWrapping::Repeat;
    
    ETextureResamplingFunction UpSamplingFunction   = ETextureResamplingFunction::Linear;
    ETextureResamplingFunction DownSamplingFunction = ETextureResamplingFunction::Linear;
};