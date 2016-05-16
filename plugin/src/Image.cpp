#include "Image.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>

#include <cassert>

namespace collage
{
    Image::Image(ImageData::Type type, int width, int height, int depth, int channels, QObject* parent):
        QObject(parent),
        _spacing(1.0, 1.0, 1.0)
    {
        _data = std::unique_ptr<ImageData>(ImageData::create(type, width, height, depth, channels));
    }

    Image::~Image()
    {

    }

    QVector3D Image::spacing() const
    {
        return _spacing;
    }

    QVector3D Image::size() const
    {
        return QVector3D(width(), height(), depth());
    }

    QString Image::source() const
    {
        return m_source;
    }

    float Image::normalizedValue(int x, int y, int z, int c) const
    {
        return _data->normalizedValue(x, y, z, c);
    }

    void* Image::data()
    {
        return _data->pointer();
    }

    const void* Image::data() const
    {
        return _data->pointer();
    }

    ImageData::Type Image::type() const
    {
        return _data->type;
    }

    int Image::width() const
    {
        return _data->width;
    }

    int Image::height() const
    {
        return _data->height;
    }

    int Image::depth() const
    {
        return _data->depth;
    }

    int Image::channels() const
    {
        return _data->channels;
    }

    void Image::setSpacing(QVector3D spacing)
    {
        if (_spacing == spacing)
            return;

        _spacing = spacing;
        emit spacingChanged(spacing);
    }

    void Image::setSource(QString source)
    {
        if (m_source == source)
            return;

        m_source = source;
        if(source.endsWith(".mhd"))
        {
            loadMHD(source);
        }
        else
        {
            qCritical("Unknown extensions");
        }
        emit sourceChanged(source);
    }

    void Image::setType(ImageData::Type type)
    {
        if (_data->type == type)
            return;

        _data = std::unique_ptr<ImageData>(ImageData::create(type, width(), height(), depth(), channels()));
        emit typeChanged(type);
    }

    void Image::setWidth(int width)
    {
        if (_data->width == width)
            return;

        _data = std::unique_ptr<ImageData>(ImageData::create(type(), width, height(), depth(), channels()));
        emit widthChanged(width);
    }

    void Image::setHeight(int height)
    {
        if (_data->height == height)
            return;

        _data = std::unique_ptr<ImageData>(ImageData::create(type(), width(), height, depth(), channels()));
        emit heightChanged(height);
    }

    void Image::setDepth(int depth)
    {
        if (_data->depth == depth)
            return;

        _data = std::unique_ptr<ImageData>(ImageData::create(type(), width(), height(), depth, channels()));
        emit depthChanged(depth);
    }

    void Image::setChannels(int channels)
    {
        if (_data->channels == channels)
            return;

        _data = std::unique_ptr<ImageData>(ImageData::create(type(), width(), height(), depth(), channels));
        emit channelsChanged(channels);
    }

    void Image::loadMHD(const QString& path)
    {
        qInfo("Loading %s", path.toLatin1().data());
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qCritical("Could not open file: %s", path.toLatin1().data());
            return;
        }

        int dim = -1;
        int width = -1;
        int height = -1;
        int depth = 1;
        int channels = 1;
        ImageData::Type type = (ImageData::Type)-1;
        QVector3D spacing(1, 1, 1);
        QString rawPath;

        auto readInt = [](const QString& str, int& out)
        {
            bool ok;
            out = str.toInt(&ok);
            if(!ok)
                qCritical("Invalid MHD tag (not a integer): %s", str.toLatin1().data());
            return ok;
        };

        auto readFloat = [](const QString& str, float& out)
        {
            bool ok;
            out = str.toFloat(&ok);
            if(!ok)
                qCritical("Invalid MHD tag (not a double): %s", str.toLatin1().data());
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
                qCritical("Skipping invalid MHD tag (too many elements): %s", line.toLatin1().data());
                continue;
            }
            auto token = split[0].trimmed();
            auto value = split[1].trimmed();

            if(token.startsWith("NDims"))
            {
                if(!readInt(value, dim))
                    return;
            }
            else if(token.startsWith("DimSize"))
            {
                auto dims = value.split(' ');
                if(!readInt(dims.value(0, "1"), width)
                   || !readInt(dims.value(1, "1"), height)
                   || !readInt(dims.value(2, "1"), depth))
                    return;
            }
            else if(token.startsWith("ElementType"))
            {
                if(value == "MET_CHAR")
                    type = ImageData::Char;
                else if(value == "MET_UCHAR")
                    type = ImageData::UnsignedChar;
                else if(value == "MET_SHORT")
                    type = ImageData::Short;
                else if(value == "MET_USHORT")
                    type = ImageData::UnsignedShort;
                else if(value == "MET_FLOAT")
                    type = ImageData::Float;
                else
                {
                    qCritical("Unsupported type: %s", value.toLatin1().data());
                    return;
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
                    return;
            }
        }

        if(width <= 0)
        {
            qCritical("Invalid or missing width: %d", width);
            return;
        }
        if(height <= 0)
        {
            qCritical("Invalid or missing height: %d", height);
            return;
        }
        if(depth <= 0)
        {
            qCritical("Invalid or missing depth: %d", depth);
            return;
        }
        if(type == -1)
        {
            qCritical("Missing type");
            return;
        }

        rawPath = QFileInfo(path).absolutePath() + "/" + rawPath;
        QFile rawFile(rawPath);
        if(!rawFile.open(QIODevice::ReadOnly))
        {
            qCritical("Could not open raw file: %s", rawPath.toLatin1().data());
            return;
        }

        char* data = nullptr;
        qint64 byteSize = rawFile.bytesAvailable();
        try
        {
            data = new char[static_cast<quint64>(byteSize)];
            if(rawFile.read(data, byteSize) == -1)
            {
                qCritical("Failed to read data from file");
                return;
            }
            assert(rawFile.atEnd());

            if(byteSize != width*height*depth*channels*ImageData::typeSize(type))
            {
                qCritical("Raw file has invalid size: %lld (expected: %lld)", byteSize, qint64(width)*height*depth*channels*ImageData::typeSize(type));
                return;
            }
        }
        catch(std::bad_alloc)
        {
            qCritical("Failed to allocate volume memory (%lld)", byteSize);
            return;
        }

        ImageData* volumeData = nullptr;
        try
        {
            switch(type)
            {
            case ImageData::Char:
                volumeData = new ImageDataTyped<char>(width, height, depth, channels, reinterpret_cast<char*>(data));
                break;
            case ImageData::UnsignedChar:
                volumeData = new ImageDataTyped<unsigned char>(width, height, depth, channels, reinterpret_cast<unsigned char*>(data));
                break;
            case ImageData::Short:
                volumeData = new ImageDataTyped<short>(width, height, depth, channels, reinterpret_cast<short*>(data));
                break;
            case ImageData::UnsignedShort:
                volumeData = new ImageDataTyped<unsigned short>(width, height, depth, channels, reinterpret_cast<unsigned short*>(data));
                break;
            case ImageData::Float:
                volumeData = new ImageDataTyped<float>(width, height, depth, channels, reinterpret_cast<float*>(data));
                break;
            }
        }
        catch(std::runtime_error e)
        {
            qCritical("Failed to create Volume: %s", e.what());
            return;
        }

        assert(volumeData);
        _data = std::unique_ptr<ImageData>(volumeData);
        setSpacing(spacing);
        emit typeChanged(_data->type);
        emit widthChanged(_data->width);
        emit heightChanged(_data->height);
        emit depthChanged(_data->depth);
        emit channelsChanged(_data->channels);
    }
}
