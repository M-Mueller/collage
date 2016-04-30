#ifndef VOLUME_H
#define VOLUME_H

#include <QtGui/QVector3D>
#include <QtCore/QObject>

class Volume: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)
    Q_PROPERTY(int depth READ depth CONSTANT)
    Q_PROPERTY(int channels READ channels CONSTANT)
    Q_PROPERTY(QVector3D spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

public:    
    enum Type
    {
        Char,
        UnsignedChar,
        Short,
        UnsignedShort,
        Float
    };

    Volume(Type type, int width, int height, int depth, int channels, QObject* parent=0);
    virtual ~Volume();
    static Volume* fromMHD(const QString& path);

    Type type() const;
    int width() const;
    int height() const;
    int depth() const;
    int channels() const;
    QVector3D spacing() const;

    static int typeSize(Type type);

    virtual void* pointer() =0;
    virtual const void* constPointer() const =0;

public slots:
    void setSpacing(QVector3D spacing);

signals:
    void spacingChanged(QVector3D spacing);

private:
    const Type _type;
    const int _width;
    const int _height;
    const int _depth;
    const int _channels;
    QVector3D _spacing;
};

template<typename T>
class VolumeTyped: public Volume
{
public:
    VolumeTyped(int width, int height, int depth, int channels, T* data);
    virtual ~VolumeTyped();

    void* pointer() override;
    const void* constPointer() const override;

    T* data();
    const T* constData() const;

private:
    VolumeTyped(Type type, int width, int height, int depth, int channels, T* data);
    T* _data;
};

class VolumeLoader: public QObject
{
    Q_OBJECT
public:
    VolumeLoader(QObject* parent=0);

    Q_INVOKABLE Volume* fromMHD(const QString& path);
};

#endif // VOLUME_H
