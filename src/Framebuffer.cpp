#include "Framebuffer.h"
#include "Texture.h"
#include <grogl/GlFrameBuffer.h>

Framebuffer::Framebuffer():
    _color0(nullptr),
    _color1(nullptr),
    _fbo(nullptr)
{

}

Framebuffer::~Framebuffer()
{
    delete _fbo;
}

void Framebuffer::synchronize()
{
    RendererElement::synchronize(); // synchronize texture before

    if(!_fbo)
    {
        _fbo = new GlFrameBuffer();
    }

    if(_fbo)
    {
        _fbo->bind();
        if(_color0)
            _color0->attachTo(*this, Color0);
        if(_color1)
            _color1->attachTo(*this, Color1);
        if(_depth)
            _depth->attachTo(*this, Depth);
        _fbo->assignDrawBuffers();
        _fbo->release();
    }
}

Texture* Framebuffer::depthAttachment() const
{
    return _depth;
}

Texture* Framebuffer::colorAttachment0() const
{
    return _color0;
}

void Framebuffer::setColorAttachment0(Texture* colorAttachment0)
{
    _color0 = colorAttachment0;
}

Texture* Framebuffer::colorAttachment1() const
{
    return _color1;
}

GlFrameBuffer* Framebuffer::gl()
{
    return _fbo;
}

void Framebuffer::setDepthAttachment(Texture* depth)
{
    _depth = depth;
}

void Framebuffer::setColorAttachment1(Texture* colorAttachment1)
{
    _color1 = colorAttachment1;
}

