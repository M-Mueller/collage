#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "Texture.h"

class GlRenderBuffer;
class RenderBuffer: public Texture
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
public:
    RenderBuffer(QObject* parent=0);
    virtual ~RenderBuffer();

    int width() const;
    int height() const;

    void synchronize();

public slots:
    void setWidth(int width);
    void setHeight(int height);

protected:
    void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos) override;

private:
    int _width;
    int _height;

    GlRenderBuffer* _buffer;
};

#endif // RENDERBUFFER_H
