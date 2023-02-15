#include "Texture2D.h"

#include "Core/Base.h"

#include "Imported/stb_image.h"


namespace graphics
{
    Texture2D::Texture2D(const char* path)
        : Texture2D()
    {
        GLCall(glGenTextures(1, &_ID));
        GLCall(glBindTexture(GL_TEXTURE_2D, _ID));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        unsigned char* data = LoadData(path);

        if (data)
        {
            if (nrChannels == 3)
            {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
            }

            if (nrChannels == 4)
            {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
            }

            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        }

        DeleteData(data);
    }

    void Texture2D::Activate(unsigned int slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, _ID));
    }

    void Texture2D::Delete() {
        if (_ID != 0) {
            GLCall(glDeleteTextures(1, &_ID));
        }
        _ID = 0;
    }

    unsigned char* Texture2D::LoadData(const char* path)
    {
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

        if (!data)
            throw std::invalid_argument("Failed to load an image/texture");

        return data;
    }


    void Texture2D::DeleteData(unsigned char* data) {
        stbi_image_free(data);
    }
}