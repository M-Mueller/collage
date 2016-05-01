#include "ImageData.h"

#include <cassert>

#include "easylogging++.h"

template<>
ImageDataTyped<char>::ImageDataTyped(int width, int height, int depth, int channels, char* data):
    ImageDataTyped<char>(Char, width, height, depth, channels, data)
{

}

template<>
ImageDataTyped<unsigned char>::ImageDataTyped(int width, int height, int depth, int channels, unsigned char* data):
    ImageDataTyped<unsigned char>(UnsignedChar, width, height, depth, channels, data)
{

}

template<>
ImageDataTyped<short>::ImageDataTyped(int width, int height, int depth, int channels, short* data):
    ImageDataTyped<short>(Short, width, height, depth, channels, data)
{

}

template<>
ImageDataTyped<unsigned short>::ImageDataTyped(int width, int height, int depth, int channels, unsigned short* data):
    ImageDataTyped<unsigned short>(UnsignedShort, width, height, depth, channels, data)
{

}

template<>
ImageDataTyped<float>::ImageDataTyped(int width, int height, int depth, int channels, float* data):
    ImageDataTyped<float>(Float, width, height, depth, channels, data)
{

}

template<typename T>
ImageDataTyped<T>::ImageDataTyped(Type type, int width, int height, int depth, int channels, T* data):
    ImageData(type, width, height, depth, channels),
    _data(data)
{

}

template<typename T>
ImageDataTyped<T>::~ImageDataTyped()
{
    delete _data;
}

template<typename T>
void* ImageDataTyped<T>::pointer()
{
    return static_cast<void*>(_data);
}

template<typename T>
const void* ImageDataTyped<T>::constPointer() const
{
    return static_cast<void*>(_data);
}

template<typename T>
T* ImageDataTyped<T>::data()
{
    return _data;
}

template<typename T>
const T* ImageDataTyped<T>::constData() const
{
    return _data;
}

template<typename T>
float ImageDataTyped<T>::floatValue(int x, int y, int z, int c) const
{
    assert(hasIndex(x, y, z, c));
    return static_cast<float>(_data[index(x, y, z, c)]);
}

template<typename T>
float ImageDataTyped<T>::normalizedValue(int x, int y, int z, int c) const
{
    const float shift = -std::numeric_limits<T>::min();
    const float scale = 1.0f/(std::numeric_limits<T>::max() + shift);
    return (floatValue(x, y, z, c) + shift)*scale;
}

ImageData::ImageData(ImageData::Type t, int w, int h, int d, int c):
    type(t),
    width(w),
    height(h),
    depth(d),
    channels(c)
{

}

ImageData::~ImageData()
{

}

ImageData* ImageData::create(ImageData::Type type, int width, int height, int depth, int channels)
{
    assert(width >= 0);
    assert(height >= 0);
    assert(depth >= 0);
    assert(channels >= 0);
    size_t size = static_cast<size_t>(width)*static_cast<size_t>(height)*static_cast<size_t>(depth)*static_cast<size_t>(channels);
    switch(type)
    {
    case Char:
        return new ImageDataTyped<char>(width, height, depth, channels, new char[size]);
    case UnsignedChar:
        return new ImageDataTyped<unsigned char>(width, height, depth, channels, new unsigned char[size]);
    case Short:
        return new ImageDataTyped<short>(width, height, depth, channels, new short[size]);
    case UnsignedShort:
        return new ImageDataTyped<unsigned short>(width, height, depth, channels, new unsigned short[size]);
    case Float:
        return new ImageDataTyped<float>(width, height, depth, channels, new float[size]);
    }
    assert(false);
    return nullptr;
}

int ImageData::typeSize(ImageData::Type type)
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
    return 0;
}

bool ImageData::hasIndex(int x, int y, int z, int c) const
{
    return x >= 0 && x < width &&
            y >= 0 && y < height &&
            z >= 0 && z < depth &&
            c >= 0 && c < channels;
}

size_t ImageData::index(int x, int y, int z, int c) const
{
    return  static_cast<size_t>(c)
            + static_cast<size_t>(x*channels)
            + static_cast<size_t>(y*channels*width)
            + static_cast<size_t>(z*channels*width*height);
}
