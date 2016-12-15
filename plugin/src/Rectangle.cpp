#include "Rectangle.h"
#include <glue/GlVertexBuffer.h>
#include <glue/GlIndexBuffer.h>
#include <glue/GlVertexArray.h>
#include <glue/GlTexture2D.h>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

namespace collage
{
    Rectangle::Rectangle(QObject* parent):
        Entity(parent),
        _position0(-1.0f, -1.0f, 0.0f),
        _position1(1.0f, -1.0f, 0.0f),
        _position2(1.0f, 1.0f, 0.0f),
        _position3(-1.0f, 1.0f, 0.0f),
        _color0(1.0f, 1.0f, 1.0f),
        _color1(1.0f, 1.0f, 1.0f),
        _color2(1.0f, 1.0f, 1.0f),
        _color3(1.0f, 1.0f, 1.0f),
        _update(true)
    {

    }

    Rectangle::~Rectangle()
    {

    }

    void Rectangle::synchronize()
    {
        if(!_vbo && !_vao && !_ibo)
        {
            _vbo = std::make_unique<GlVertexBuffer>();
            _ibo = std::make_unique<GlIndexBuffer>();
            _vao = std::make_unique<GlVertexArray>();
        }

        if(_update)
        {
            std::vector<QVector3D> vertices = {
                _position0,
                _position1,
                _position2,
                _position3
            };
            std::vector<QVector3D> colors = {
                _color0,
                _color1,
                _color2,
                _color3,
            };
            std::vector<unsigned int> indices = {
                0, 1, 2,
                2, 3, 0
            };
            _vbo->bind();
            _vbo->setData(GlBuffer::Usage::StaticDraw, vertices, colors);

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

    QVector3D Rectangle::position0() const
    {
        return _position0;
    }

    QVector3D Rectangle::position1() const
    {
        return _position1;
    }

    QVector3D Rectangle::position2() const
    {
        return _position2;
    }

    QVector3D Rectangle::position3() const
    {
        return _position3;
    }

    QVector3D Rectangle::color0() const
    {
        return _color0;
    }

    QVector3D Rectangle::color1() const
    {
        return _color1;
    }

    QVector3D Rectangle::color2() const
    {
        return _color2;
    }

    QVector3D Rectangle::color3() const
    {
        return _color3;
    }

    void Rectangle::setPosition0(QVector3D position0)
    {
        if (_position0 == position0)
            return;

        _position0 = position0;
        emit position0Changed(position0);
        updateBuffers();
    }

    void Rectangle::setPosition1(QVector3D position1)
    {
        if (_position1 == position1)
            return;

        _position1 = position1;
        emit position1Changed(position1);
        updateBuffers();
    }

    void Rectangle::setPosition2(QVector3D position2)
    {
        if (_position2 == position2)
            return;

        _position2 = position2;
        emit position2Changed(position2);
        updateBuffers();
    }

    void Rectangle::setPosition3(QVector3D position3)
    {
        if (_position3 == position3)
            return;

        _position3 = position3;
        emit position3Changed(position3);
        updateBuffers();
    }

    void Rectangle::setColor0(QVector3D color0)
    {
        if (_color0 == color0)
            return;

        _color0 = color0;
        emit color0Changed(color0);
        updateBuffers();
    }

    void Rectangle::setColor1(QVector3D color1)
    {
        if (_color1 == color1)
            return;

        _color1 = color1;
        emit color1Changed(color1);
        updateBuffers();
    }

    void Rectangle::setColor2(QVector3D color2)
    {
        if (_color2 == color2)
            return;

        _color2 = color2;
        emit color2Changed(color2);
        updateBuffers();
    }

    void Rectangle::setColor3(QVector3D color3)
    {
        if (_color3 == color3)
            return;

        _color3 = color3;
        emit color3Changed(color3);
        updateBuffers();
    }
}
