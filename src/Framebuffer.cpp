#include "Framebuffer.h"
#include <grogl/GlFrameBuffer.h>

Framebuffer::Framebuffer():
    _color0(nullptr),
    _color1(nullptr),
    _fbo(nullptr),
    _width(0),
    _height(0)
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
            _fbo->attach(*_color0->gl(), 0);
        if(_color1)
            _fbo->attach(*_color1->gl(), 0);
        _fbo->assignDrawBuffers();
        _fbo->release();
    }
}

Texture2D* Framebuffer::colorAttachment0() const
{
    return _color0;
}

void Framebuffer::setColorAttachment0(Texture2D* colorAttachment0)
{
    _color0 = colorAttachment0;
}

Texture2D* Framebuffer::colorAttachment1() const
{
    return _color1;
}

int Framebuffer::width()
{
    if(_width <= 0)
        return _color0->width();
    else
        return _width;
}

int Framebuffer::height()
{
    if(_height <= 0)
        return _color0->height();
    else
        return _height;
}

GlFrameBuffer* Framebuffer::gl()
{
    return _fbo;
}

void Framebuffer::setColorAttachment1(Texture2D* colorAttachment1)
{
    _color1 = colorAttachment1;
}

