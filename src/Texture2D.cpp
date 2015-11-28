#include "Texture2D.h"
#include <grogl/GlTexture2D.h>
#include <grogl/GlFrameBuffer.h>
#include <glm/glm.hpp>

#include "easylogging++.h"

Texture2D::Texture2D(QObject* parent):
    Texture(parent),
    _width(0),
    _height(0),
    _tex(nullptr)
{

}


Texture2D::~Texture2D()
{
    delete _tex;
}

int Texture2D::width() const
{
    return _width;
}

void Texture2D::setWidth(int width)
{
    if(width < 0)
        width = 0;
    _width = width;
}

int Texture2D::channels() const
{
    assert(_sourceImage.isNull() || _channels == 3 || _channels == 4);
    return _channels;
}

void Texture2D::setChannels(int channels)
{
    if(_sourceImage.isNull())
        Texture::setChannels(channels);
}

Texture2D::Type Texture2D::type() const
{
    assert(_sourceImage.isNull() || _type == Type::Char);
    return Texture::type();
}

void Texture2D::setType(const Type& type)
{
    if(_sourceImage.isNull())
        Texture::setType(type);
}

int Texture2D::height() const
{
    return _height;
}

void Texture2D::setHeight(int height)
{
    if(height < 0)
        height = 0;
    _height = height;
}


void Texture2D::synchronize()
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


    assert(mapToFormat.count(std::make_pair(_channels, _type)));
    GlTexture::InternalFormat format = mapToFormat[std::make_pair(_channels, _type)];
    if(!_tex)
    {
        _tex = new GlTexture2D();
    }
    assert(_tex);

    if(_sourceImage.isNull())
    {
        if(_tex->width() != _width || _tex->height() != _height || _tex->format() != format)
        {
            LOG(INFO) << "Resizing Texture2D";
            // resize the texture if the dimensions changed
            _tex->bind();
            _tex->resize(format, _width, _height);
            _tex->release();
        }
    }
    else
    {
        if(_tex->width() != _width || _tex->height() != _height || _tex->format() != format)
        {
            LOG(INFO) << "Uploading Texture2D (source: " << _source << ")";
            assert(_sourceImage.width() == _width && _sourceImage.height() == _height);//TODO: rescale image
            assert(_sourceImage.format() == QImage::Format_ARGB32 || _sourceImage.format() == QImage::Format_RGB32);

            // QImage stores images as ARGB but OpenGL can only load RGBA (RGA32 is stored as ARGB32 as well)
            auto data = new unsigned char[_width*_height*_channels];
            auto src = reinterpret_cast<const QRgb*>(_sourceImage.constBits());
            auto dst = reinterpret_cast<uint32_t*>(data);

            assert(QSysInfo::ByteOrder == QSysInfo::LittleEndian);
            for(int h=0; h<_height; ++h)
            {
                for(int w=0; w<_width; ++w)
                {
                    // because of little endian, this will be stored inverted
                    *dst = (qRed(*src)) |
                           (qGreen(*src) << 8) |
                           (qBlue(*src) << 16) |
                           (qAlpha(*src) << 24);
                    ++src;
                    ++dst;
                }
            }

            _tex->bind();
            _tex->setImageData(format, _width, _height, GlTexture::PixelFormat::RGBA, GlTexture::PixelType::UnsignedByte, data);
            _tex->release();
            delete[] data;
        }
    }

    RendererElement::synchronize();
}

GlTexture2D* Texture2D::gl()
{
    return _tex;
}

void Texture2D::attachTo(Framebuffer& fbo, Framebuffer::Attachment pos)
{
    GlFrameBuffer* gl = fbo.gl();
    if(gl && _tex)
    {
        gl->attach(*_tex, static_cast<GlFrameBuffer::Attachment>(pos));
    }
    else
    {
        LOG(ERROR) << "Texture could not be attached";
    }
}

QString Texture2D::source() const
{
    return _source;
}

void Texture2D::setSource(const QString& source)
{
    _source = source;

    QImage img(source);
    if(!source.isEmpty() && img.isNull())
        LOG(ERROR) << "Failed to load image: " << source;

    if(!img.isNull() && img.format() != QImage::Format_RGB32 && img.format() != QImage::Format_ARGB32)
    {
        // To make uploading easier, everything is converted to ARGB32
        LOG(INFO) << "Converting '" << source << "' to ARGB32";
        _sourceImage = img.convertToFormat(QImage::Format_ARGB32);
    }
    else
    {
        _sourceImage = img;
    }

    if(!img.isNull())
    {
        // if a valid image is set, the type and number of channels will be read only
        _type = Type::UnsignedChar;
        _width = img.width();
        _height = img.height();
        if(_sourceImage.hasAlphaChannel())
            _channels = 4;
        else
            _channels = 3;
    }
}
