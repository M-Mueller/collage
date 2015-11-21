#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "RendererElement.h"
#include "Texture2D.h"
#include <QtCore/QObject>

class GlFrameBuffer;
class Framebuffer: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(Texture2D* colorAttachment0 READ colorAttachment0 WRITE setColorAttachment0)
    Q_PROPERTY(Texture2D* colorAttachment1 READ colorAttachment1 WRITE setColorAttachment1)

public:
    Framebuffer();
    virtual ~Framebuffer();

    virtual void synchronize() override;

    Texture2D* colorAttachment0() const;
    Texture2D* colorAttachment1() const;

    int width();
    int height();

    GlFrameBuffer* gl();

public slots:
    void setColorAttachment0(Texture2D* colorAttachment0);
    void setColorAttachment1(Texture2D* colorAttachment1);

private:
    Texture2D* _color0;
    Texture2D* _color1;

    GlFrameBuffer* _fbo;
    int _width;
    int _height;
};

#endif // FRAMEBUFFER_H
