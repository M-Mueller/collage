#ifndef VOLUME_H
#define VOLUME_H

#include "ImageData.h"

#include <QtGui/QVector3D>
#include <QtGui/QColor>
#include <QtCore/QObject>

#include <memory>

namespace collage
{
    class Image: public QObject
    {
        Q_OBJECT
        Q_PROPERTY(ImageData::Type type READ type WRITE setType NOTIFY typeChanged)
        Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
        Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
        Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
        Q_PROPERTY(int channels READ channels WRITE setChannels NOTIFY channelsChanged)
        Q_PROPERTY(QVector3D spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
        Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
        //Q_ENUM(ImageData::Type)

    public:
        Image(ImageData::Type type=ImageData::Char, int width=0, int height=0, int depth=0, int channels=0, QObject* parent=0);
        virtual ~Image();

        ImageData::Type type() const;
        int width() const;
        int height() const;
        int depth() const;
        int channels() const;
        QVector3D spacing() const;
        QVector3D size() const;

        QString source() const;

        float normalizedValue(int x, int y, int z, int c) const;

        void* data();
        const void* data() const;

    public slots:
        void setType(ImageData::Type type);
        void setWidth(int width);
        void setHeight(int height);
        void setDepth(int depth);
        void setChannels(int channels);
        void setSpacing(QVector3D spacing);

        void setSource(QString source);

    signals:
        void spacingChanged(QVector3D spacing);
        void typeChanged(ImageData::Type type);
        void widthChanged(int width);
        void heightChanged(int height);
        void depthChanged(int depth);
        void channelsChanged(int channels);

        void sourceChanged(QString source);

    private:
        void loadMHD(const QString& path);

    private:
        QVector3D _spacing;
        std::unique_ptr<ImageData> _data;
        QString m_source;
    };
}

#endif // VOLUME_H
