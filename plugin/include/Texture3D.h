#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "Texture.h"
#include <QtGui/QVector3D>
#include <QtGui/QImage>
#include <QtCore/QObject>

class GlTexture3D;
namespace collage
{
    class Image;
    class Texture3D: public Texture
    {
        Q_OBJECT
        Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
        Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
        Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
        Q_PROPERTY(QVector3D size READ size NOTIFY sizeChanged)
        Q_PROPERTY(collage::Image* volume READ volume WRITE setVolume NOTIFY volumeChanged)

    public:
        Texture3D(QObject* parent=0);
        ~Texture3D();

        int width() const;
        int height() const;
        int depth() const;
        int channels() const override;
        Type type() const override;
        QVector3D size() const;

        Image* volume() const;

        virtual void synchronize() override;

        GlTexture3D* gl();

    public slots:
        void setChannels(int channels) override;
        void setType(const Type& type) override;

        void setWidth(int width);
        void setHeight(int height);
        void setDepth(int depth);

        void setVolume(Image* volume);

    signals:
        void widthChanged(int width);
        void heightChanged(int height);
        void depthChanged(int depth);

        void sizeChanged(QVector3D size);
        void spacingChanged(QVector3D spacing);

        void volumeChanged(Image* volume);

    protected:
        void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos) override;
        void bind(int unit) override;
        void unbind(int unit) override;

    private:
        int _width;
        int _height;
        int _depth;

        Image* _volume;
        GlTexture3D* _tex;

        bool _update;
    };
}

#endif // TEXTURE3D_H
