#include "Triangle.h"
#include <grogl/GlVertexBuffer.h>
#include <grogl/GlVertexArray.h>

Triangle::Triangle(QObject* parent):
    Entity(parent),
    vao(0),
    vbo(0)
{

}

Triangle::~Triangle()
{
    delete vao;
    delete vbo;
}

void Triangle::synchronize()
{
    if(!vbo && !vao)
    {
        std::vector<glm::vec2> vertices = {
             glm::vec2(0.0f,  0.5f),
             glm::vec2(0.5f, -0.5f),
             glm::vec2(-0.5f, -0.5f)
        };
        std::vector<glm::vec3> colors = {
             glm::vec3(1.0f, 0.0f, 0.0f),
             glm::vec3(1.0f, 1.0f, 0.0f),
             glm::vec3(0.0f, 0.0f, 1.0f)
        };

        vbo = new GlVertexBuffer;
        vbo->bind();
        vbo->setData(GlBuffer::StaticDraw, vertices, colors);

        vao = new GlVertexArray;
        vao->bind();
        vao->setVertexData(*vbo);
        vao->release();
        vbo->release();
    }
}

void Triangle::render(GlProgram& /*program*/)
{
    vao->bind();
    vao->draw(GlVertexArray::Triangles);
    vao->release();
}

