#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "RendererElement.h"
#include <QtCore/QObject>

class GlFrameBuffer;
class Texture;
class Framebuffer: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(Texture* colorAttachment0 READ colorAttachment0 WRITE setColorAttachment0)
    Q_PROPERTY(Texture* colorAttachment1 READ colorAttachment1 WRITE setColorAttachment1)
    Q_PROPERTY(Texture* depthAttachment READ depthAttachment WRITE setDepthAttachment)

public:
    enum Attachment
    {
        Color0 = 0,
        Color1 = 1,
        Depth = 32
    };

    Framebuffer();
    virtual ~Framebuffer();

    virtual void synchronize() override;

    Texture* colorAttachment0() const;
    Texture* colorAttachment1() const;
    Texture* depthAttachment() const;

    GlFrameBuffer* gl();

public slots:
    void setColorAttachment0(Texture* colorAttachment0);
    void setColorAttachment1(Texture* colorAttachment1);
    void setDepthAttachment(Texture* depth);

private:
    Texture* _color0;
    Texture* _color1;
    Texture* _depth;

    GlFrameBuffer* _fbo;
};

#endif // FRAMEBUFFER_H
