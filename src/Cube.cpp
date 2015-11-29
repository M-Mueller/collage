#include "Cube.h"
#include <grogl/GlVertexArray.h>
#include <grogl/GlVertexBuffer.h>
#include <grogl/GlIndexBuffer.h>
#include <glad/glad.h>

Cube::Cube(QObject* parent):
    Entity(parent),
    _vao(nullptr),
    _vbo(nullptr),
    _ibo(nullptr),
    _cullMode(None)
{

}

Cube::~Cube()
{
    delete _vao;
    delete _vbo;
    delete _ibo;
}

void Cube::synchronize()
{
    if(!_vbo && !_vao && !_ibo)
    {
        //     3.+------+ 2
        //    .' |    .'|
        // 7 +---+--+'6 |
        //   |   |  |   |
        //   | 0,+--+---+ 1
        //   |.'    | .'
        // 4 +------+'5
        std::vector<glm::vec3> vertices = {
           glm::vec3(-1.0f, -1.0f, -1.0f),
           glm::vec3(1.0f, -1.0f, -1.0f),
           glm::vec3(1.0f, 1.0f, -1.0f),
           glm::vec3(-1.0f, 1.0f, -1.0f),
           glm::vec3(-1.0f, -1.0f, 1.0f),
           glm::vec3(1.0f, -1.0f, 1.0f),
           glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec3(-1.0f, 1.0f, 1.0f)
        };

        std::vector<glm::vec3> colors = {
           glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3(1.0f, 0.0f, 0.0f),
           glm::vec3(1.0f, 1.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3(0.0f, 0.0f, 1.0f),
           glm::vec3(1.0f, 0.0f, 1.0f),
           glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec3(0.0f, 1.0f, 1.0f)
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
            _vao = new GlVertexArray;
            _vbo = new GlVertexBuffer;
            _ibo = new GlIndexBuffer;

            _vao->bind();

            _vbo->bind();
            _vbo->setData(GlBuffer::Usage::StaticDraw, vertices, colors);

            _ibo->bind(); // vao unbinds element_array_buffer
            _ibo->setData(GlBuffer::Usage::StaticDraw, indices);

            _vao->setVertexData(*_vbo);
            _vao->setIndexBuffer(*_ibo);

            _vao->release();

            _vbo->release();
            _ibo->release();
        }
        catch(...)
        {
            if(_vao)
            {
                _vao->release();
                delete _vao;
                _vao = nullptr;
            }

            if(_vbo)
            {
                _vbo->release();
                delete _vbo;
                _vbo = nullptr;
            }

            if(_ibo)
            {
                _ibo->release();
                delete _ibo;
                _ibo = nullptr;
            }
        }
    }
    _r_cullMode = _cullMode;
}

void Cube::render(GlProgram& /*program*/)
{
    if(_r_cullMode != None)
    {
        glEnable(GL_CULL_FACE);
        if(_r_cullMode == Front)
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

    if(_r_cullMode != None)
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

