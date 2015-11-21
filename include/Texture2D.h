#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "RendererElement.h"
#include <QtCore/QObject>
#include <QtGui/QImage>

class GlTexture2D;
class Texture2D: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(int channels READ channels WRITE setChannels)
    Q_PROPERTY(QString source READ source WRITE setSource)

public:
    enum class Type
    {
        Char,
        Short,
        HalfFloat,
        Float
    };
    Q_ENUM(Type)

    Texture2D(QObject* parent=0);
    ~Texture2D();

    int width() const;
    int height() const;
    int channels() const;
    Type type() const;

    virtual void synchronize() override;

    GlTexture2D* gl();

    /// Returns the optional source the texture is loaded from
    QString source() const;

public slots:
    void setHeight(int height);
    void setWidth(int width);
    void setChannels(int channels);
    void setType(const Type& type);
    /**
     * @brief Sets an optional source filepath to load a image for the texture.
     * The path will be instantly loaded as a QImage but it will not be uploaded as a GlTexture2D until synchronize is called.
     * The image is expected to be in the format RGB32 or ARGB32, any other format will be implicitly converted.
     */
    void setSource(const QString& source);

private:
    Type _type;
    int _width;
    int _height;
    int _channels;

    GlTexture2D* _tex;

    QString _source;
    QImage _sourceImage;
};

#endif // TEXTURE2D_H
