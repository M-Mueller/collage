#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "Texture.h"
#include <QtGui/QVector3D>
#include <QtGui/QImage>
#include <QtCore/QObject>

class GlTexture3D;
class Texture3D: public Texture
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(int depth READ depth WRITE setDepth)
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(QVector3D spacing READ spacing WRITE setSpacing)

public:
    Texture3D(QObject* parent=0);
    ~Texture3D();

    int width() const;
    int height() const;
    int depth() const;
    int channels() const override;
    Type type() const override;

    /// Returns the optional source the texture is loaded from
    QString source() const;

    virtual void synchronize() override;

    GlTexture3D* gl();

    QVector3D spacing() const;

public slots:
    void setHeight(int height);
    void setWidth(int width);
    void setDepth(int depth);
    void setChannels(int channels) override;
    void setType(const Type& type) override;
    void setSpacing(const QVector3D& spacing);

    void setSource(const QString& source);

protected:
    void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos) override;
    void bind(int unit) override;

private:
    bool loadMHD(const QString& header);

private:
    int _width;
    int _height;
    int _depth;

    QVector3D _spacing;

    GlTexture3D* _tex;

    QString _source;
    QByteArray _data;
};

#endif // TEXTURE3D_H
