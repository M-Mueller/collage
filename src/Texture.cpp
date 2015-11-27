#include "Texture.h"
#include <glm/glm.hpp>

Texture::Texture(QObject* parent):
    QObject(parent),
    _type(Char),
    _channels(1)
{

}

Texture::~Texture()
{

}

int Texture::channels() const
{
    return _channels;
}

void Texture::setChannels(int channels)
{
    _channels = glm::clamp(channels, 1, 4);
}

Texture::Type Texture::type() const
{
    return _type;
}

void Texture::setType(const Type& type)
{
    _type = type;
}
