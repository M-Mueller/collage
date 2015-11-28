#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"
#include <QtCore/QObject>
#include <QtGui/QImage>

class GlTexture2D;
class Texture2D: public Texture
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(QString source READ source WRITE setSource)

public:

    Texture2D(QObject* parent=0);
    ~Texture2D();

    int width() const;
    int height() const;
    int channels() const override;
    Type type() const override;

    /// Returns the optional source the texture is loaded from
    QString source() const;

    virtual void synchronize() override;

    GlTexture2D* gl();

    virtual void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos);

public slots:
    void setHeight(int height);
    void setWidth(int width);
    void setChannels(int channels) override;
    void setType(const Type& type) override;
    /**
     * @brief Sets an optional source filepath to load a image for the texture.
     * The path will be instantly loaded as a QImage but it will not be uploaded as a GlTexture2D until synchronize is called.
     * The image is expected to be in the format RGB32 or ARGB32, any other format will be implicitly converted.
     */
    void setSource(const QString& source);

private:
    int _width;
    int _height;

    GlTexture2D* _tex;

    QString _source;
    QImage _sourceImage;
};

#endif // TEXTURE2D_H
