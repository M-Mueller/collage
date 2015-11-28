#ifndef TEXTURE_H
#define TEXTURE_H

#include "RendererElement.h"
#include "Framebuffer.h"
#include <QtCore/QObject>

class Texture: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(int channels READ channels WRITE setChannels)
public:
    enum Type
    {
        Char,
        UnsignedChar,
        Short,
        UnsignedShort,
        HalfFloat,
        Float,
        Depth16,
        Depth24,
        Depth32,
        Depth32F
    };
    Q_ENUM(Type)

    Texture(QObject* parent=0);
    virtual ~Texture();

    virtual int channels() const;
    virtual Type type() const;

    /// Returns the size of the Type in byte
    static int typeSize(Type type);

    virtual void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos) =0;

public slots:
    virtual void setChannels(int channels);
    virtual void setType(const Type& type);

protected:
    Type _type;
    int _channels;
};

#endif // TEXTURE_H
