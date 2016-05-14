#include "ClearFramebuffer.h"
#include <glad/glad.h>

ClearFramebuffer::ClearFramebuffer(QObject* parent):
    Entity(parent),
    _clearColorBuffer(false),
    _clearDepthBuffer(false),
    _clearStencilBuffer(false),
    _clearColor(QColor(0, 0, 0, 0)),
    _clearDepth(1.0)
{

}

ClearFramebuffer::~ClearFramebuffer()
{

}

void ClearFramebuffer::synchronize()
{
    _clearColorBuffer.synchronize();
    _clearDepthBuffer.synchronize();
    _clearStencilBuffer.synchronize();

    _clearColor.synchronize();
    _clearDepth.synchronize();
}

void ClearFramebuffer::render(GlProgram& /*program*/)
{
    glClearDepth(_clearDepth.gl());
    auto cc = _clearColor.gl();
    glClearColor(cc.r, cc.g, cc.b, cc.a);

    GLbitfield bits = 0;
    if(_clearColorBuffer.gl())
        bits |= GL_COLOR_BUFFER_BIT;
    if(_clearDepthBuffer.gl())
        bits |= GL_DEPTH_BUFFER_BIT;
    if(_clearStencilBuffer.gl())
        bits |= GL_STENCIL_BUFFER_BIT;

    if(bits != 0)
        glClear(bits);
}

bool ClearFramebuffer::clearColorBuffer() const
{
    return _clearColorBuffer;
}

bool ClearFramebuffer::clearDepthBuffer() const
{
    return _clearDepthBuffer;
}

bool ClearFramebuffer::clearStencilBuffer() const
{
    return _clearStencilBuffer;
}

double ClearFramebuffer::clearDepth() const
{
    return _clearDepth;
}

QColor ClearFramebuffer::clearColor() const
{
    return _clearColor;
}

void ClearFramebuffer::setClearColorBuffer(bool clearColorBuffer)
{
    if (_clearColorBuffer == clearColorBuffer)
        return;

    _clearColorBuffer = clearColorBuffer;
    emit clearColorBufferChanged(clearColorBuffer);
}

void ClearFramebuffer::setClearDepthBuffer(bool clearDepthBuffer)
{
    if (_clearDepthBuffer == clearDepthBuffer)
        return;

    _clearDepthBuffer = clearDepthBuffer;
    emit clearDepthBufferChanged(clearDepthBuffer);
}

void ClearFramebuffer::setClearStencilBuffer(bool clearStencilBuffer)
{
    if (_clearStencilBuffer == clearStencilBuffer)
        return;

    _clearStencilBuffer = clearStencilBuffer;
    emit clearStencilBufferChanged(clearStencilBuffer);
}

void ClearFramebuffer::setClearDepth(double clearDepth)
{
    if (std::abs(_clearDepth - clearDepth) < 1e-10)
        return;

    _clearDepth = clearDepth;
    emit clearDepthChanged(clearDepth);
}

void ClearFramebuffer::setClearColor(QColor clearColor)
{
    if (_clearColor == clearColor)
        return;

    _clearColor = clearColor;
    emit clearColorChanged(clearColor);
}
