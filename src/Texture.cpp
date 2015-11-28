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
    if(_type == Depth16 || _type == Depth24 || _type == Depth32 || _type == Depth32F)
        return;
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
    case Depth16:
        return 2;
    case Depth24:
        return 3;
    case Depth32:
    case Depth32F:
        return 4;
    }
    return 0;
}

void Texture::setType(const Type& type)
{
    _type = type;
    if(_type == Depth16 || _type == Depth24 || _type == Depth32 || _type == Depth32F)
        _channels = 1;
}
