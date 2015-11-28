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

int Texture::typeSize(Texture::Type type)
{
    switch (type) {
    case Char:
    case UnsignedChar:
        return 1;
    case Short:
    case UnsignedShort:
        return 2;
    case HalfFloat:
        return 2;
    case Float:
        return 4;
    }
    return 0;
}

void Texture::setType(const Type& type)
{
    _type = type;
}
