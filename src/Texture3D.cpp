#include "Texture3D.h"
#include "Volume.h"
#include "easylogging++.h"

#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <glue/GlTexture3D.h>
#include <glue/GlFrameBuffer.h>

#include <cassert>

Texture3D::Texture3D(QObject* parent):
    Texture(parent),
    _width(0),
    _height(0),
    _depth(0),
    _volume(nullptr),
    _tex(nullptr),
    _update(false)
{

}

Texture3D::~Texture3D()
{
    delete _tex;
}

void Texture3D::synchronize()
{
    std::map<std::pair<int, Type>, GlTexture::InternalFormat> mapToFormat =
    {
        {{1, Type::Char}, GlTexture::InternalFormat::R8},
        {{2, Type::Char}, GlTexture::InternalFormat::RG8},
        {{3, Type::Char}, GlTexture::InternalFormat::RGB8},
        {{4, Type::Char}, GlTexture::InternalFormat::RGBA8},
        {{1, Type::UnsignedChar}, GlTexture::InternalFormat::R8},
        {{2, Type::UnsignedChar}, GlTexture::InternalFormat::RG8},
        {{3, Type::UnsignedChar}, GlTexture::InternalFormat::RGB8},
        {{4, Type::UnsignedChar}, GlTexture::InternalFormat::RGBA8},
        {{1, Type::Short}, GlTexture::InternalFormat::R16},
        {{2, Type::Short}, GlTexture::InternalFormat::RG16},
        {{3, Type::Short}, GlTexture::InternalFormat::RGB16},
        {{4, Type::Short}, GlTexture::InternalFormat::RGBA16},
        {{1, Type::UnsignedShort}, GlTexture::InternalFormat::R16},
        {{2, Type::UnsignedShort}, GlTexture::InternalFormat::RG16},
        {{3, Type::UnsignedShort}, GlTexture::InternalFormat::RGB16},
        {{4, Type::UnsignedShort}, GlTexture::InternalFormat::RGBA16},
        {{1, Type::HalfFloat}, GlTexture::InternalFormat::R16F},
        {{2, Type::HalfFloat}, GlTexture::InternalFormat::RG16F},
        {{3, Type::HalfFloat}, GlTexture::InternalFormat::RGB16F},
        {{4, Type::HalfFloat}, GlTexture::InternalFormat::RGBA16F},
        {{1, Type::Float}, GlTexture::InternalFormat::R32F},
        {{2, Type::Float}, GlTexture::InternalFormat::RG32F},
        {{3, Type::Float}, GlTexture::InternalFormat::RGB32F},
        {{4, Type::Float}, GlTexture::InternalFormat::RGBA32F},
        {{1, Type::Depth16}, GlTexture::InternalFormat::DepthComponent16},
        {{1, Type::Depth24}, GlTexture::InternalFormat::DepthComponent24},
        {{1, Type::Depth32}, GlTexture::InternalFormat::DepthComponent32},
        {{1, Type::Depth32F}, GlTexture::InternalFormat::DepthComponent32F}
    };

    std::map<Type, GlTexture::PixelType> mapToPixelType =
    {
        {Type::Char, GlTexture::PixelType::Byte},
        {Type::UnsignedChar, GlTexture::PixelType::UnsignedByte},
        {Type::Short, GlTexture::PixelType::Short},
        {Type::UnsignedShort, GlTexture::PixelType::UnsignedShort},
        {Type::HalfFloat, GlTexture::PixelType::HalfFloat},
        {Type::Float, GlTexture::PixelType::Float}
    };

    std::map<int, GlTexture::PixelFormat> mapToPixelFormat =
    {
        {1, GlTexture::PixelFormat::Red},
        {2, GlTexture::PixelFormat::RG},
        {3, GlTexture::PixelFormat::RGB},
        {4, GlTexture::PixelFormat::RGBA}
    };

    assert(mapToFormat.count(std::make_pair(_channels, _type)));
    GlTexture::InternalFormat format = mapToFormat[std::make_pair(_channels, _type)];
    if(!_tex)
    {
        _tex = new GlTexture3D();
    }
    assert(_tex);

    if(_update)
    {
        _update = false;
        if(_volume)
        {
            LOG(INFO) << "Uploading Texture3D";
            assert(width() == _volume->width());
            assert(height() == _volume->height());
            assert(depth() == _volume->depth());
            assert(channels() == _volume->channels());
            assert(mapToPixelType.count(type()));
            assert(mapToPixelFormat.count(channels()));

            _tex->bind();
            _tex->setImageData(format, width(), height(), depth(), mapToPixelFormat[channels()], mapToPixelType[type()], _volume->constPointer());
            _tex->release();
        }
        else
        {
            if(_tex->width() != _width || _tex->height() != _height || _tex->format() != format)
            {
                LOG(INFO) << "Resizing Texture3D";
                // resize the texture if the dimensions changed
                _tex->bind();
                _tex->resize(format, _width, _height, _depth);
                _tex->release();
            }
        }
    }

    RendererElement::synchronize();
}

GlTexture3D* Texture3D::gl()
{
    return _tex;
}

QVector3D Texture3D::size() const
{
    return QVector3D(_width, _height, _depth);
}

void Texture3D::attachTo(Framebuffer& /*fbo*/, Framebuffer::Attachment /*pos*/)
{
    assert(false);
//    GlFrameBuffer* gl = fbo.gl();
//    if(gl && _tex)
//    {
//        gl->attach(*_tex, static_cast<GlFrameBuffer::Attachment>(pos));
//    }
//    else
//    {
//        LOG(ERROR) << "Texture could not be attached";
    //    }
}

void Texture3D::bind(int unit)
{
    if(_tex)
        _tex->bind(unit);
}

void Texture3D::unbind(int unit)
{
    if(_tex)
        _tex->release(unit);
}

int Texture3D::width() const
{
    if(_volume)
        return _volume->width();
    else
        return _width;
}

int Texture3D::height() const
{
    if(_volume)
        return _volume->height();
    else
        return _height;
}

int Texture3D::depth() const
{
    if(_volume)
        return _volume->depth();
    else
        return _depth;
}

int Texture3D::channels() const
{
    if(_volume)
        return _volume->channels();
    else
        return Texture::channels();
}

void Texture3D::setChannels(int channels)
{
    Texture::setChannels(channels);
    setVolume(nullptr);
}

Texture::Type Texture3D::type() const
{
    if(_volume)
    {
        switch (_volume->type())
        {
        case Volume::Char:
            return Char;
        case Volume::UnsignedChar:
            return UnsignedChar;
        case Volume::Short:
            return Short;
        case Volume::UnsignedShort:
            return UnsignedShort;
        case Volume::Float:
            return Float;
        default:
            assert(false);
            return Char;
        }
    }
    else
    {
        return Texture::type();
    }
}

void Texture3D::setType(const Texture::Type& type)
{
    Texture::setType(type);
    setVolume(nullptr);
    _update = true;
}

void Texture3D::setWidth(int width)
{
    if (_width == width)
        return;

    _width = width;
    setVolume(nullptr);
    _update = true;
    emit widthChanged(width);
    emit sizeChanged(size());
}

void Texture3D::setHeight(int height)
{
    if (_height == height)
        return;

    _height = height;
    setVolume(nullptr);
    _update = true;
    emit heightChanged(height);
    emit sizeChanged(size());
}

void Texture3D::setDepth(int depth)
{
    if (_depth == depth)
        return;

    _depth = depth;
    setVolume(nullptr);
    _update = true;
    emit depthChanged(depth);
    emit sizeChanged(size());
}

void Texture3D::setVolume(Volume* volume)
{
    if (_volume == volume)
        return;

    _volume = volume;
    _update = true;
    emit volumeChanged(volume);
}


Volume* Texture3D::volume() const
{
    return _volume;
}

