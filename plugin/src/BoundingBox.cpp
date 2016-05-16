#include "BoundingBox.h"

#include <QtCore/QtDebug>

#include <glue/GlVertexArray.h>
#include <glue/GlVertexBuffer.h>
#include <glue/GlIndexBuffer.h>
#include <glue/GlProgram.h>

#include <glad/glad.h>

namespace collage
{
    BoundingBox::BoundingBox(QObject* parent):
        Entity(parent),
        _renderMode(Wireframe),
        _center(0.0, 0.0, 0.0),
        _size(1.0, 1.0, 1.0)
    {

    }

    BoundingBox::~BoundingBox()
    {
    }

    void BoundingBox::synchronize()
    {
        if(_update || !_vertices || !_vao || !_indices)
        {
            _vao = nullptr;
            _vertices = nullptr;
            _indices = nullptr;

            //     3.+------+ 2
            //    .' |    .'|
            // 7 +---+--+'6 |
            //   |   |  |   |
            //   | 0,+--+---+ 1
            //   |.'    | .'
            // 4 +------+'5
            std::vector<QVector3D> vertices = {
               _center + 0.5*_size*QVector3D(-1.0f, -1.0f, -1.0f),
               _center + 0.5*_size*QVector3D(1.0f, -1.0f, -1.0f),
               _center + 0.5*_size*QVector3D(1.0f, 1.0f, -1.0f),
               _center + 0.5*_size*QVector3D(-1.0f, 1.0f, -1.0f),
               _center + 0.5*_size*QVector3D(-1.0f, -1.0f, 1.0f),
               _center + 0.5*_size*QVector3D(1.0f, -1.0f, 1.0f),
               _center + 0.5*_size*QVector3D(1.0f, 1.0f, 1.0f),
               _center + 0.5*_size*QVector3D(-1.0f, 1.0f, 1.0f)
            };

            std::vector<unsigned short> indices;
            if(_renderMode == Wireframe)
            {
                // lines
                indices = {
                    0, 1,
                    1, 5,
                    5, 4,
                    4, 0,
                    3, 2,
                    2, 6,
                    6, 7,
                    7, 3,
                    0, 3,
                    1, 2,
                    4, 7,
                    5, 6
                };
            }
            else
            {
                // triangles
                indices = {
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
            }

            try
            {
                _vao = std::make_unique<GlVertexArray>();
                _vertices = std::make_unique<GlVertexBuffer>();
                _indices = std::make_unique<GlIndexBuffer>();

                _vertices->bind();
                _vertices->setData(GlBuffer::Usage::StaticDraw, vertices);

                _indices->bind();
                _indices->setData(GlBuffer::Usage::StaticDraw, indices);

                _vao->bind();
                _vao->setVertexData(*_vertices);
                _vao->setIndexBuffer(*_indices);
                _vao->release();

                _vertices->release();
                _indices->release();
            }
            catch(...)
            {
                qCritical("Failed to upload BoundingBox data");
                _vao = nullptr;
                _vertices = nullptr;
                _indices = nullptr;
            }
        }

        _renderMode.synchronize();

        _update = false;
    }

    void BoundingBox::render(GlProgram&)
    {
        if(_vao)
        {
            _vao->bind();
            if(_renderMode.gl() == Wireframe)
                _vao->drawElements(GlVertexArray::Primitive::Lines);
            else
                _vao->drawElements(GlVertexArray::Primitive::Triangles);
            _vao->release();
        }
    }

    QVector3D BoundingBox::size() const
    {
        return _size;
    }

    BoundingBox::RenderMode BoundingBox::renderMode() const
    {
        return _renderMode;
    }

    QVector3D BoundingBox::center() const
    {
        return _center;
    }

    void BoundingBox::setSize(QVector3D size)
    {
        if (_size == size)
            return;

        _size = size;
        _update = true;
        emit sizeChanged(size);
    }

    void BoundingBox::setRenderMode(BoundingBox::RenderMode renderMode)
    {
        if (_renderMode == renderMode)
            return;

        _renderMode = renderMode;
        _update = true;
        emit renderModeChanged(renderMode);
    }

    void BoundingBox::setCenter(QVector3D center)
    {
        if (_center == center)
            return;

        _center = center;
        _update = true;
        emit centerChanged(center);
    }
}
