#include "Volume.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include <cassert>

#include "easylogging++.h"

template<>
VolumeTyped<char>::VolumeTyped(int width, int height, int depth, int channels, char* data):
    VolumeTyped(Char, width, height, depth, channels, data)
{

}

template<>
VolumeTyped<unsigned char>::VolumeTyped(int width, int height, int depth, int channels, unsigned char* data):
    VolumeTyped(UnsignedChar, width, height, depth, channels, data)
{

}

template<>
VolumeTyped<short>::VolumeTyped(int width, int height, int depth, int channels, short* data):
    VolumeTyped(Short, width, height, depth, channels, data)
{

}

template<>
VolumeTyped<unsigned short>::VolumeTyped(int width, int height, int depth, int channels, unsigned short* data):
    VolumeTyped(UnsignedShort, width, height, depth, channels, data)
{

}

template<>
VolumeTyped<float>::VolumeTyped(int width, int height, int depth, int channels, float* data):
    VolumeTyped(Float, width, height, depth, channels, data)
{

}

template<typename T>
VolumeTyped<T>::VolumeTyped(Type type, int width, int height, int depth, int channels, T* data):
    Volume(type, width, height, depth, channels),
    _data(data)
{

}

template<typename T>
VolumeTyped<T>::~VolumeTyped()
{
    delete _data;
}

template<typename T>
void* VolumeTyped<T>::pointer()
{
    return static_cast<void*>(_data);
}

template<typename T>
const void* VolumeTyped<T>::constPointer() const
{
    return static_cast<void*>(_data);
}

template<typename T>
T* VolumeTyped<T>::data()
{
    return _data;
}

template<typename T>
const T* VolumeTyped<T>::constData() const
{
    return _data;
}

Volume::Volume(Volume::Type type, int width, int height, int depth, int channels, QObject* parent):
    QObject(parent),
    _type(type),
    _width(width),
    _height(height),
    _depth(depth),
    _channels(channels),
    _spacing(1.0, 1.0, 1.0)
{

}

Volume::~Volume()
{

}

Volume* Volume::fromMHD(const QString& path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG(ERROR) << "Could not open file: " << path;
        return nullptr;
    }

    int dim = -1;
    int width = -1;
    int height = -1;
    int depth = 1;
    int channels = 1;
    Type type = (Type)-1;
    QVector3D spacing(1, 1, 1);
    QString rawPath;

    auto readInt = [](const QString& str, int& out)
    {
        bool ok;
        out = str.toInt(&ok);
        if(!ok)
            LOG(ERROR) << "Invalid MHD tag (not a integer): " << str;
        return ok;
    };

    auto readFloat = [](const QString& str, float& out)
    {
        bool ok;
        out = str.toFloat(&ok);
        if(!ok)
            LOG(ERROR) << "Invalid MHD tag (not a double): " << str;
        return ok;
    };

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        auto line = stream.readLine().trimmed();
        // ignore comments
        if(line.startsWith('#'))
            continue;

        auto split = line.split('=');
        if(split.length() != 2)
        {
            LOG(ERROR) << "Skipping invalid MHD tag (too many elements): " << line;
            continue;
        }
        auto token = split[0].trimmed();
        auto value = split[1].trimmed();

        if(token.startsWith("NDims"))
        {
            if(!readInt(value, dim))
                return nullptr;
        }
        else if(token.startsWith("DimSize"))
        {
            auto dims = value.split(' ');
            if(!readInt(dims.value(0, "1"), width)
               || !readInt(dims.value(1, "1"), height)
               || !readInt(dims.value(2, "1"), depth))
                return nullptr;
        }
        else if(token.startsWith("ElementType"))
        {
            if(value == "MET_CHAR")
                type = Char;
            else if(value == "MET_UCHAR")
                type = UnsignedChar;
            else if(value == "MET_SHORT")
                type = Short;
            else if(value == "MET_USHORT")
                type = UnsignedShort;
            else if(value == "MET_FLOAT")
                type = Float;
            else
            {
                LOG(ERROR) << "Unsupported type: " << value;
                return nullptr;
            }
        }
        else if(token.startsWith("ElementDataFile"))
        {
            rawPath = value;
        }
        else if(token.startsWith("ElementSpacing"))
        {
            auto dims = value.split(' ');
            if(!readFloat(dims.value(0, "1"), spacing[0])
               || !readFloat(dims.value(1, "1"), spacing[1])
               || !readFloat(dims.value(2, "1"), spacing[2]))
                return nullptr;
        }
    }

    if(width <= 0)
    {
        LOG(ERROR) << "Invalid or missing width: " << width;
        return nullptr;
    }
    if(height <= 0)
    {
        LOG(ERROR) << "Invalid or missing height: " << height;
        return nullptr;
    }
    if(depth <= 0)
    {
        LOG(ERROR) << "Invalid or missing depth: " << depth;
        return nullptr;
    }
    if(type == -1)
    {
        LOG(ERROR) << "Missing type";
        return nullptr;
    }

    rawPath = QFileInfo(path).absolutePath() + "/" + rawPath;
    QFile rawFile(rawPath);
    if(!rawFile.open(QIODevice::ReadOnly))
    {
        LOG(ERROR) << "Could not open raw file: " << rawPath;
        return nullptr;
    }

    char* data = nullptr;
    qint64 byteSize = rawFile.bytesAvailable();
    try
    {
        data = new char[static_cast<quint64>(byteSize)];
        if(rawFile.read(data, byteSize) == -1)
        {
            LOG(ERROR) << "Failed to read data from file";
            return nullptr;
        }
        assert(rawFile.atEnd());

        if(byteSize != width*height*depth*channels*typeSize(type))
        {
            LOG(ERROR) << "Raw file has invalid size: " << byteSize << " (expected: " << width*height*depth*channels*typeSize(type) << ")";
            return nullptr;
        }
    }
    catch(std::bad_alloc)
    {
        LOG(ERROR) << "Failed to allocate volume memory (" << byteSize << ")";
        return nullptr;
    }

    Volume* volume = nullptr;
    try
    {
        switch(type)
        {
        case Char:
            volume = new VolumeTyped<char>(width, height, depth, channels, reinterpret_cast<char*>(data));
            break;
        case UnsignedChar:
            volume = new VolumeTyped<unsigned char>(width, height, depth, channels, reinterpret_cast<unsigned char*>(data));
            break;
        case Short:
            volume = new VolumeTyped<short>(width, height, depth, channels, reinterpret_cast<short*>(data));
            break;
        case UnsignedShort:
            volume = new VolumeTyped<unsigned short>(width, height, depth, channels, reinterpret_cast<unsigned short*>(data));
            break;
        case Float:
            volume = new VolumeTyped<float>(width, height, depth, channels, reinterpret_cast<float*>(data));
            break;
        }
    }
    catch(std::runtime_error e)
    {
        LOG(ERROR) << "Failed to create Volume: " << e.what();
        return nullptr;
    }

    assert(volume);
    volume->setSpacing(spacing);
    return volume;
}

Volume::Type Volume::type() const
{
    return _type;
}

int Volume::width() const
{
    return _width;
}

int Volume::height() const
{
    return _height;
}

int Volume::depth() const
{
    return _depth;
}

int Volume::channels() const
{
    return _channels;
}

QVector3D Volume::spacing() const
{
    return _spacing;
}

int Volume::typeSize(Volume::Type type)
{
    switch(type)
    {
    case Char:
    case UnsignedChar:
        return 1;
    case Short:
    case UnsignedShort:
        return 2;
    case Float:
        return 4;
    }
    assert(false);
    return 1;
}

void Volume::setSpacing(QVector3D spacing)
{
    if (_spacing == spacing)
        return;

    _spacing = spacing;
    emit spacingChanged(spacing);
}

VolumeLoader::VolumeLoader(QObject* parent):
    QObject(parent)
{

}

Volume* VolumeLoader::fromMHD(const QString& path)
{
    return Volume::fromMHD(path);
}
