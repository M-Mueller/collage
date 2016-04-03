#include "RenderBuffer.h"
#include "easylogging++.h"

#include <glue/GlFrameBuffer.h>
#include <glue/GlRenderBuffer.h>

#include <cassert>

RenderBuffer::RenderBuffer(QObject* parent):
    Texture(parent),
    _buffer(nullptr)
{

}

RenderBuffer::~RenderBuffer()
{
    delete _buffer;
}

int RenderBuffer::width() const
{
    return _width;
}

void RenderBuffer::setWidth(int width)
{
    _width = width;
}

int RenderBuffer::height() const
{
    return _height;
}

void RenderBuffer::attachTo(Framebuffer& fbo, Framebuffer::Attachment pos)
{
    GlFrameBuffer* gl = fbo.gl();
    if(gl && _buffer)
    {
        gl->attach(*_buffer, static_cast<GlFrameBuffer::Attachment>(pos));
    }
    else
    {
        LOG(ERROR) << "Texture could not be attached";
    }
}

void RenderBuffer::bind(int /*unit*/)
{
    LOG(ERROR) << "Renderbuffers cannot be used in samplers";
}

void RenderBuffer::synchronize()
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

    if(!_buffer)
    {
        _buffer = new GlRenderBuffer();
    }
    assert(_buffer);

    assert(mapToFormat.count(std::make_pair(_channels, _type)));
    auto format = mapToFormat[std::make_pair(_channels, _type)];
    if(_buffer->width() != _width || _buffer->height() != _height || _buffer->format() != format)
    {
        LOG(INFO) << "Resizing RenderBuffer";
        _buffer->bind();
        _buffer->resize(format, _width, _height);
        _buffer->release();
    }
}

void RenderBuffer::setHeight(int height)
{
    _height = height;
}

