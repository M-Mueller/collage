#include "Rectangle.h"
#include <grogl/GlVertexBuffer.h>
#include <grogl/GlIndexBuffer.h>
#include <grogl/GlVertexArray.h>
#include <grogl/GlTexture2D.h>

Rectangle::Rectangle(QObject* parent):
    Entity(parent),
    _tex(0),
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
        std::vector<glm::vec2> vertices = {
            glm::vec2(-1.0f, -1.0f),
            glm::vec2(1.0f, -1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(-1.0f, 1.0f)
        };
        std::vector<glm::vec2> texCoords = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f)
        };
        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0
        };

        _vbo = new GlVertexBuffer;
        _vbo->bind();
        _vbo->setData(GlBuffer::StaticDraw, vertices, texCoords);

        _ibo = new GlIndexBuffer;
        _ibo->bind();
        _ibo->setData(GlBuffer::StaticDraw, indices);

        _vao = new GlVertexArray;
        _vao->bind();
        _ibo->bind();
        _vao->setVertexData(*_vbo);
        _vao->setIndexBuffer(*_ibo);
        _vao->release();
        _vbo->release();
        _ibo->release();
    }

    if(_tex->parent() == this)
    {
        _tex->synchronize();
    }
}

void Rectangle::render(GlProgram& program)
{
    if(_tex)
    {
        _tex->gl()->bind(0);
        program.setUniform("tex", 0);
    }
    _vao->bind();
    _vao->drawElements(GlVertexArray::Triangles);
    _vao->release();
    if(_tex)
    {
        _tex->gl()->release();
    }
}

Texture2D* Rectangle::texture() const
{
    return _tex;
}

void Rectangle::setTexture(Texture2D* tex)
{
    _tex = tex;
}

