#include "Cube.h"

#include <glue/GlVertexArray.h>
#include <glue/GlVertexBuffer.h>
#include <glue/GlIndexBuffer.h>
#include <glue/GlProgram.h>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

Cube::Cube(QObject* parent):
    Entity(parent),
    _cullMode(None),
    _size(1.0, 1.0, 1.0)
{

}

Cube::~Cube()
{
}

void Cube::synchronize()
{
    if(!_vbo && !_vao && !_ibo)
    {
        _vao = nullptr;
        _vbo = nullptr;
        _ibo = nullptr;

        //     3.+------+ 2
        //    .' |    .'|
        // 7 +---+--+'6 |
        //   |   |  |   |
        //   | 0,+--+---+ 1
        //   |.'    | .'
        // 4 +------+'5
        std::vector<QVector3D> vertices = {
           _size*QVector3D(-1.0f, -1.0f, -1.0f),
           _size*QVector3D(1.0f, -1.0f, -1.0f),
           _size*QVector3D(1.0f, 1.0f, -1.0f),
           _size*QVector3D(-1.0f, 1.0f, -1.0f),
           _size*QVector3D(-1.0f, -1.0f, 1.0f),
           _size*QVector3D(1.0f, -1.0f, 1.0f),
           _size*QVector3D(1.0f, 1.0f, 1.0f),
           _size*QVector3D(-1.0f, 1.0f, 1.0f)
        };

        std::vector<QVector3D> colors = {
           QVector3D(0.0f, 0.0f, 0.0f),
           QVector3D(1.0f, 0.0f, 0.0f),
           QVector3D(1.0f, 1.0f, 0.0f),
           QVector3D(0.0f, 1.0f, 0.0f),
           QVector3D(0.0f, 0.0f, 1.0f),
           QVector3D(1.0f, 0.0f, 1.0f),
           QVector3D(1.0f, 1.0f, 1.0f),
           QVector3D(0.0f, 1.0f, 1.0f)
        };

        std::vector<unsigned short> indices = {
           0, 3, 1,
           1, 3, 2,
           4, 5, 6,
           4, 6, 7,
           1, 2, 5,
           5, 2, 6,
           0, 4, 3,
           4, 7, 3,
           0, 1, 4,
           1, 5, 4,
           3, 7, 2,
           2, 7, 6
        };

        try
        {
            _vao = std::make_unique<GlVertexArray>();
            _vbo = std::make_unique<GlVertexBuffer>();
            _ibo = std::make_unique<GlIndexBuffer>();

            _vbo->bind();
            _vbo->setData(GlBuffer::Usage::StaticDraw, vertices, colors);

            _ibo->bind(); // vao unbinds element_array_buffer
            _ibo->setData(GlBuffer::Usage::StaticDraw, indices);

            _vao->bind();
            _vao->setVertexData(*_vbo);
            _vao->setIndexBuffer(*_ibo);
            _vao->release();

            _vbo->release();
            _ibo->release();
        }
        catch(...)
        {
            _vao = nullptr;
            _vbo = nullptr;
            _ibo = nullptr;
        }
    }
    _cullMode.synchronize();
}

void Cube::render(GlProgram&)
{
    if(_cullMode.gl() != None)
    {
        glEnable(GL_CULL_FACE);
        if(_cullMode.gl() == Front)
            glCullFace(GL_FRONT);
        else
            glCullFace(GL_BACK);
    }

    if(_vao)
    {
        _vao->bind();
        _vao->drawElements(GlVertexArray::Primitive::Triangles);
        _vao->release();
    }

    if(_cullMode.gl() != None)
    {
        glDisable(GL_CULL_FACE);
    }
}

Cube::CullMode Cube::cullMode() const
{
    return _cullMode;
}

void Cube::setCullMode(const CullMode& cullMode)
{
    _cullMode = cullMode;
}

QVector3D Cube::size() const
{
    return _size;
}

void Cube::setSize(QVector3D size)
{
    if (_size == size)
        return;

    _size = size;
    emit sizeChanged(size);
}

