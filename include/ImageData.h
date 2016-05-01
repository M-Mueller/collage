#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <cstddef>

class ImageData
{
public:
    enum Type
    {
        Char,
        UnsignedChar,
        Short,
        UnsignedShort,
        Float
    };

    ImageData(Type type, int width, int height, int depth, int channels);
    virtual ~ImageData();

    static ImageData* create(Type type, int width, int height, int depth, int channels);

    const Type type;
    const int width;
    const int height;
    const int depth;
    const int channels;

    static int typeSize(Type type);

    virtual void* pointer() =0;
    virtual const void* constPointer() const =0;

    virtual float normalizedValue(int x, int y, int z, int c) const =0;

    bool hasIndex(int x, int y, int z, int c) const;
    size_t index(int x, int y, int z, int c) const;
};

template<typename T>
class ImageDataTyped: public ImageData
{
public:
    ImageDataTyped(int width, int height, int depth, int channels, T* data);
    virtual ~ImageDataTyped();

    void* pointer() override;
    const void* constPointer() const override;

    T* data();
    const T* constData() const;

    float normalizedValue(int x, int y, int z, int c) const override;

private:
    ImageDataTyped(Type type, int width, int height, int depth, int channels, T* data);
    float floatValue(int x, int y, int z, int c) const;

    T* _data;
};
#endif // IMAGETYPED_H
