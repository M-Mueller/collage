#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Entity.h"
#include "RendererProperty.h"

#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>

#include <memory>

class GlVertexArray;
class GlVertexBuffer;
class GlIndexBuffer;

namespace collage
{
    class BoundingBox : public Entity
    {
        Q_OBJECT
        Q_PROPERTY(RenderMode renderMode READ renderMode WRITE setRenderMode NOTIFY renderModeChanged)
        Q_PROPERTY(QVector3D center READ center WRITE setCenter NOTIFY centerChanged)
        Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)

    public:
        enum RenderMode
        {
            Wireframe,
            Solid
        };
        Q_ENUM(RenderMode)

        BoundingBox(QObject* parent=0);
        virtual ~BoundingBox();

        void synchronize() override;
        void render(GlProgram& program) override;

        QVector3D center() const;
        QVector3D size() const;

        RenderMode renderMode() const;

    public slots:
        void setCenter(QVector3D center);
        void setSize(QVector3D size);

        void setRenderMode(RenderMode renderMode);

    signals:
        void centerChanged(QVector3D center);
        void sizeChanged(QVector3D size);

        void renderModeChanged(RenderMode renderMode);

    private:
        std::unique_ptr<GlVertexArray> _vao;
        std::unique_ptr<GlVertexBuffer> _vertices;
        std::unique_ptr<GlIndexBuffer> _indices;

        bool _update;
        RendererProperty<RenderMode> _renderMode;
        QVector3D _center;
        QVector3D _size;
    };
}

#endif // CUBE_H
