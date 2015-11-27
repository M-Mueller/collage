#include "Texture3D.h"
#include <grogl/GlTexture3D.h>

Texture3D::Texture3D(QObject* parent):
    Texture(parent),
    _width(0),
    _height(0),
    _depth(0),
    _tex(nullptr)
{

}

Texture3D::~Texture3D()
{
    delete _tex;
}

void Texture3D::synchronize()
{

}

GlTexture3D* Texture3D::gl()
{
    return _tex;
}

int Texture3D::width() const
{
    return _width;
}

void Texture3D::setWidth(int width)
{
    _width = width;
}

int Texture3D::height() const
{
    return _height;
}

void Texture3D::setHeight(int height)
{
    _height = height;
}

int Texture3D::depth() const
{
    return _depth;
}
void Texture3D::setDepth(int depth)
{
    _depth = depth;
}

int Texture3D::channels() const
{
    return Texture::channels();
}

void Texture3D::setChannels(int channels)
{
    Texture::setChannels(channels);
}

Texture::Type Texture3D::type() const
{
    return Texture::type();
}

void Texture3D::setType(const Texture::Type& type)
{
    Texture::setType(type);
}

QString Texture3D::source() const
{
    return _source;
}

void Texture3D::setSource(const QString& source)
{

}

