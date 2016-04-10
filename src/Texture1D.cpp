#include "Texture1D.h"
#include "easylogging++.h"
#include <glue/GlTexture1D.h>
#include <glue/GlFrameBuffer.h>
#include <glm/glm.hpp>
#include <QtGui/QColor>

Texture1D::Texture1D(QObject* parent):
    Texture(parent),
    _width(0),
    _tex(nullptr),
    _data(nullptr)
{

}

Texture1D::~Texture1D()
{
    delete _tex;
    delete _data;
}

int Texture1D::width() const
{
    return _width;
}

void Texture1D::setWidth(int width)
{
    if (_width == width)
        return;

    _width = width;
    emit widthChanged(width);
}


void Texture1D::load(const QList<QVariant>& data)
{
    if(_data)
    {
        delete _data;
        _data = nullptr;
    }
    setWidth(0);

    if(data.empty())
        return;

    if(data[0].type() == QVariant::Double)
    {
        _data = new float[data.size()];
        setWidth(data.size());
        setChannels(1);
        setType(Texture::Float);
        for(int i=0; i<data.size(); ++i)
            _data[i] = data[i].toFloat();
    }
    else if(data[0].type() == QVariant::Color)
    {
        _data = new float[data.size()*4];
        setWidth(data.size());
        setChannels(4);
        setType(Texture::Float);
        for(int i=0; i<data.size(); ++i)
        {
            QColor c = data[i].value<QColor>();
            _data[4*i] = static_cast<float>(c.redF());
            _data[4*i+1] = static_cast<float>(c.greenF());
            _data[4*i+2] = static_cast<float>(c.blueF());
            _data[4*i+3] = static_cast<float>(c.alphaF());
        }
    }
    else
    {
        LOG(ERROR) << "Unsupported tpye";
    }
}

void Texture1D::synchronize()
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
        _tex = new GlTexture1D();
    }
    assert(_tex);

    if(!_data)
    {
        if(_tex->width() != _width || _tex->format() != format)
        {
            LOG(INFO) << "Resizing Texture1D";
            // resize the texture if the dimensions changed
            _tex->bind();
            _tex->resize(format, _width);
            _tex->release();
        }
    }
    else
    {
        GlTexture::PixelFormat pixelFormat;
        if(_channels == 1)
            pixelFormat = GlTexture::PixelFormat::Red;
        else if(_channels == 2)
            pixelFormat = GlTexture::PixelFormat::RG;
        else if(_channels == 3)
            pixelFormat = GlTexture::PixelFormat::RGB;
        else if(_channels == 4)
            pixelFormat = GlTexture::PixelFormat::RGBA;
        else
        {
            LOG(ERROR) << "Unsupported number of channels";
            RendererElement::synchronize();
            return;
        }

        GlTexture::PixelType pixelType;
        switch(_type)
        {
        case Char:
            pixelType = GlTexture::PixelType::Byte; break;
        case UnsignedChar:
            pixelType = GlTexture::PixelType::UnsignedByte; break;
        case Short:
			pixelType = GlTexture::PixelType::Short; break;
        case UnsignedShort:
			pixelType = GlTexture::PixelType::UnsignedShort; break;
        case HalfFloat:
			pixelType = GlTexture::PixelType::HalfFloat; break;
        case Float:
            pixelType = GlTexture::PixelType::Float; break;
        case Depth16:
        case Depth24:
        case Depth32:
        case Depth32F:
            LOG(ERROR) << "Unsupported Type";
            RendererElement::synchronize();
            return;
        }

        _tex->bind();
        _tex->setImageData(format, _width, pixelFormat, pixelType, _data);
        _tex->release();

        delete _data;
        _data = nullptr;
    }

    RendererElement::synchronize();
}

GlTexture1D* Texture1D::gl()
{
    return _tex;
}

void Texture1D::attachTo(Framebuffer& fbo, Framebuffer::Attachment pos)
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

void Texture1D::bind(int unit)
{
    if(_tex)
        _tex->bind(unit);
}

void Texture1D::unbind(int unit)
{
    if(_tex)
        _tex->release(unit);
}
