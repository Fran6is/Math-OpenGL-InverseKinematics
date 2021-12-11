#include "Texture.h"
#include <stb_image\stb_image.h>

FTexImage FImageLoader::LoadImage(const std::string& ImagePath, unsigned char** PtrToData_OPTIONAL) //optinal parameter
{
    stbi_set_flip_vertically_on_load(true);
    FTexImage ReadImage; 
    int nrChannels{};
    ReadImage.Data = stbi_load(ImagePath.c_str(), &ReadImage.Width, &ReadImage.Height, &nrChannels, 0);

    if (nrChannels == 1)      ReadImage.Format = GL_RED;
    else if (nrChannels == 3) ReadImage.Format = GL_RGB;
    else if (nrChannels == 4) ReadImage.Format = GL_RGBA;

    if(!ReadImage.Data)
    {
        std::cout << "std_image failed to load texture from '" << ImagePath << std::endl;
    }

    if(PtrToData_OPTIONAL) *PtrToData_OPTIONAL = ReadImage.Data;

    return ReadImage;
}

void FImageLoader::FreeImage(unsigned char* Data) 
{
    stbi_image_free(Data);
}

