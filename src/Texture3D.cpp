#include "Texture3D.h"
#include "easylogging++.h"

#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <grogl/GlTexture3D.h>
#include <grogl/GlFrameBuffer.h>

#include <cassert>

Texture3D::Texture3D(QObject* parent):
    Texture(parent),
    _width(0),
    _height(0),
    _depth(0),
    _spacing(1.0, 1.0, 1.0),
    _tex(nullptr)
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

    if(_data.isNull())
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
    else
    {
        LOG(INFO) << "Uploading Texture3D (source: " << _source << ")";
        assert(_data.size() == _width*_height*_depth*_channels*typeSize(_type));
        assert(mapToPixelType.count(_type));
        assert(mapToPixelFormat.count(_channels));

        _tex->bind();
        _tex->setImageData(format, _width, _height, _depth, mapToPixelFormat[_channels], mapToPixelType[_type], _data.constData());
        _tex->release();
        _data.clear();
    }

    RendererElement::synchronize();
}

GlTexture3D* Texture3D::gl()
{
    return _tex;
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

int Texture3D::width() const
{
    return _width;
}

void Texture3D::setWidth(int width)
{
    if(width < 0)
        width = 0;
    _width = width;
}

int Texture3D::height() const
{
    return _height;
}

void Texture3D::setHeight(int height)
{
    if(height < 0)
        height = 0;
    _height = height;
}

int Texture3D::depth() const
{
    return _depth;
}
void Texture3D::setDepth(int depth)
{
    if(depth < 0)
        depth = 0;
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
    if(source.endsWith(".mhd"))
    {
        loadMHD(source);
        _source = source;
    }
}

bool Texture3D::loadMHD(const QString& header)
{
    QFile file(header);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG(ERROR) << "Could not open file: " << header;
        return false;
    }

    int dim = -1;
    int width = -1;
    int height = -1;
    int depth = 1;
    int channels = 1;
    Type type = (Type)-1;
    QVector3D spacing(1, 1, 1);
    QString rawPath;

    auto readInt = [](const QString& str, int& out)
    {
        bool ok;
        out = str.toInt(&ok);
        if(!ok)
            LOG(ERROR) << "Invalid MHD tag (not a integer): " << str;
        return ok;
    };

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        auto line = stream.readLine().trimmed();
        auto split = line.split('=');
        if(split.length() != 2)
        {
            LOG(ERROR) << "Skipping invalid MHD tag (too many elements): " << line;
            continue;
        }
        auto token = split[0].trimmed();
        auto value = split[1].trimmed();

        if(token.startsWith("NDims"))
        {
            if(!readInt(value, dim))
                return false;
        }
        else if(token.startsWith("DimSize"))
        {
            auto dims = value.split(' ');
            if(!readInt(dims.value(0, "1"), width)
               || !readInt(dims.value(1, "1"), height)
               || !readInt(dims.value(2, "1"), depth))
                return false;
        }
        else if(token.startsWith("ElementType"))
        {
            if(value == "MET_CHAR")
                type = Char;
            else if(value == "MET_UCHAR")
                type = UnsignedChar;
            else if(value == "MET_SHORT")
                type = Short;
            else if(value == "MET_USHORT")
                type = UnsignedShort;
            else if(value == "MET_FLOAT")
                type = Float;
            else
            {
                LOG(ERROR) << "Unsupported type: " << value;
                return false;
            }
        }
        else if(token.startsWith("ElementDataFile"))
        {
            rawPath = value;
        }
    }

    if(width <= 0)
    {
        LOG(ERROR) << "Invalid or missing width: " << width;
        return false;
    }
    if(height <= 0)
    {
        LOG(ERROR) << "Invalid or missing height: " << height;
        return false;
    }
    if(depth <= 0)
    {
        LOG(ERROR) << "Invalid or missing depth: " << depth;
        return false;
    }
    if(type == -1)
    {
        LOG(ERROR) << "Missing type";
        return false;
    }

    rawPath = QFileInfo(header).absolutePath() + "/" + rawPath;
    QFile rawFile(rawPath);
    if(!rawFile.open(QIODevice::ReadOnly))
    {
        LOG(ERROR) << "Could not open raw file: " << rawPath;
        return false;
    }

    QByteArray data = rawFile.readAll();
    if(data.size() != width*height*depth*channels*typeSize(type))
    {
        LOG(ERROR) << "Raw file has invalid size: " << data.size() << " (expected: " << width*height*depth*channels*typeSize(type) << ")";
        return false;
    }

    _type = type;
    _width = width;
    _height = height;
    _depth = depth;
    _channels = channels;
    _spacing = spacing;
    _data = data;
    return true;
}

