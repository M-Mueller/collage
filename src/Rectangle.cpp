#include "Rectangle.h"
#include <glue/GlVertexBuffer.h>
#include <glue/GlIndexBuffer.h>
#include <glue/GlVertexArray.h>
#include <glue/GlTexture2D.h>
#include <glm/gtc/type_ptr.hpp>

Rectangle::Rectangle(QObject* parent):
    Entity(parent),
    _color0(1.0f, 1.0f, 1.0f),
    _color1(1.0f, 1.0f, 1.0f),
    _color2(1.0f, 1.0f, 1.0f),
    _color3(1.0f, 1.0f, 1.0f),
    _texCoord0(0.0f, 0.0f, 0.0f),
    _texCoord1(1.0f, 0.0f, 0.0f),
    _texCoord2(1.0f, 1.0f, 0.0f),
    _texCoord3(0.0f, 1.0f, 0.0f),
    _update(true),
    _vao(0),
    _vbo(0),
    _ibo(0)
{

}

Rectangle::~Rectangle()
{
    delete _vao;
    delete _vbo;
    delete _ibo;
}

void Rectangle::synchronize()
{
    if(!_vbo && !_vao && !_ibo)
    {
        _vbo = new GlVertexBuffer;
        _ibo = new GlIndexBuffer;
        _vao = new GlVertexArray;
    }

    if(_update)
    {
        std::vector<glm::vec2> vertices = {
            glm::vec2(-1.0f, -1.0f),
            glm::vec2(1.0f, -1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(-1.0f, 1.0f)
        };
        std::vector<glm::vec3> colors = {
            glm::vec3(_color0.x(), _color0.y(), _color0.z()),
            glm::vec3(_color1.x(), _color1.y(), _color1.z()),
            glm::vec3(_color2.x(), _color2.y(), _color2.z()),
            glm::vec3(_color3.x(), _color3.y(), _color3.z())
        };
        std::vector<glm::vec3> texCoords = {
            glm::vec3(_texCoord0.x(), _texCoord0.y(), _texCoord0.z()),
            glm::vec3(_texCoord1.x(), _texCoord1.y(), _texCoord1.z()),
            glm::vec3(_texCoord2.x(), _texCoord2.y(), _texCoord2.z()),
            glm::vec3(_texCoord3.x(), _texCoord3.y(), _texCoord3.z())
        };
        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0
        };
        _vbo->bind();
        _vbo->setData(GlBuffer::Usage::StaticDraw, vertices, colors, texCoords);

        _ibo->bind();
        _ibo->setData(GlBuffer::Usage::StaticDraw, indices);

        _vao->bind();
        _ibo->bind();
        _vao->setVertexData(*_vbo);
        _vao->setIndexBuffer(*_ibo);
        _vao->release();
        _vbo->release();
        _ibo->release();
        _update = false;
    }
    Entity::synchronize();
}

void Rectangle::render(GlProgram& /*program*/)
{
    _vao->bind();
    _vao->drawElements(GlVertexArray::Primitive::Triangles);
    _vao->release();
}

void Rectangle::updateBuffers()
{
    _update = true;
}

QVector3D Rectangle::texCoord3() const
{
    return _texCoord3;
}

void Rectangle::setTexCoord3(const QVector3D& texCoord3)
{
    _texCoord3 = texCoord3;
    updateBuffers();
}

QVector3D Rectangle::texCoord2() const
{
    return _texCoord2;
}

void Rectangle::setTexCoord2(const QVector3D& texCoord2)
{
    _texCoord2 = texCoord2;
    updateBuffers();
}

QVector3D Rectangle::texCoord1() const
{
    return _texCoord1;
}

void Rectangle::setTexCoord1(const QVector3D& texCoord1)
{
    _texCoord1 = texCoord1;
    updateBuffers();
}

QVector3D Rectangle::texCoord0() const
{
    return _texCoord0;
}

void Rectangle::setTexCoord0(const QVector3D& texCoord0)
{
    _texCoord0 = texCoord0;
    updateBuffers();
}

QVector3D Rectangle::color0() const
{
    return _color0;
}

void Rectangle::setColor0(const QVector3D& color0)
{
    _color0 = color0;
    updateBuffers();
}

QVector3D Rectangle::color1() const
{
    return _color1;
}

void Rectangle::setColor1(const QVector3D& color1)
{
    _color1 = color1;
    updateBuffers();
}

QVector3D Rectangle::color2() const
{
    return _color2;
}

void Rectangle::setColor2(const QVector3D& color2)
{
    _color2 = color2;
    updateBuffers();
}

QVector3D Rectangle::color3() const
{
    return _color3;
}

void Rectangle::setColor3(const QVector3D& color3)
{
    _color3 = color3;
    updateBuffers();
}

