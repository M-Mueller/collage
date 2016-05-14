#ifndef CUBE_H
#define CUBE_H

#include "Entity.h"
#include "RendererProperty.h"

#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>

#include <memory>

class GlVertexArray;
class GlVertexBuffer;
class GlIndexBuffer;
class Cube : public Entity
{
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)

public:
    Cube(QObject* parent=0);
    virtual ~Cube();

    void synchronize() override;
    void render(GlProgram& program) override;

    QVector3D size() const;

public slots:
    void setSize(QVector3D size);

signals:
    void sizeChanged(QVector3D size);

private:
    std::unique_ptr<GlVertexArray> _vao;
    std::unique_ptr<GlVertexBuffer> _vbo;
    std::unique_ptr<GlIndexBuffer> _ibo;

    QVector3D _size;
};

#endif // CUBE_H
