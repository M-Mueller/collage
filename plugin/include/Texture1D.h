#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "Texture.h"
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtCore/QVariant>

class GlTexture1D;

namespace collage
{
    class Texture1D: public Texture
    {
        Q_OBJECT
        Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)

    public:

        Texture1D(QObject* parent=0);
        ~Texture1D();

        virtual void synchronize() override;

        GlTexture1D* gl();

        int width() const;

    public slots:
        Q_INVOKABLE void load(const QList<QVariant>& data);

        void setWidth(int width);

    signals:
        void widthChanged(int width);

    protected:
        void attachTo(Framebuffer& fbo, Framebuffer::Attachment pos) override;
        void bind(int unit) override;
        void unbind(int unit) override;

    private:
        int _width;

        GlTexture1D* _tex;
        float* _data;
    };
}

#endif // TEXTURE1D_H
