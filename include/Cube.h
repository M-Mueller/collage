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
    Q_PROPERTY(CullMode cullMode READ cullMode WRITE setCullMode)
    Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)

public:
    enum CullMode
    {
        None,
        Front,
        Back
    };
    Q_ENUM(CullMode)

    Cube(QObject* parent=0);
    virtual ~Cube();

    void synchronize() override;
    void render(GlProgram& program) override;

    CullMode cullMode() const;
    QVector3D size() const;

public slots:
    void setCullMode(const CullMode& cullMode);
    void setSize(QVector3D size);

signals:
    void sizeChanged(QVector3D size);

private:
    std::unique_ptr<GlVertexArray> _vao;
    std::unique_ptr<GlVertexBuffer> _vbo;
    std::unique_ptr<GlIndexBuffer> _ibo;

    RendererProperty<CullMode> _cullMode;
    QVector3D _size;
};

#endif // CUBE_H
