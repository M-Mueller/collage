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
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(QVector3D spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(QVector3D size READ size NOTIFY sizeChanged)

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

    QVector3D size() const;
    QVector3D spacing() const;

public slots:
    void setChannels(int channels) override;
    void setType(const Type& type) override;

    void setSource(const QString& source);

    void setWidth(int width);
    void setHeight(int height);
    void setDepth(int depth);

    void setSpacing(QVector3D spacing);

signals:
    void widthChanged(int width);
    void heightChanged(int height);
    void depthChanged(int depth);

    void sizeChanged(QVector3D size);
    void spacingChanged(QVector3D spacing);

protected:
    void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos) override;
    void bind(int unit) override;
    void unbind(int unit) override;

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
