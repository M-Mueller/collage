#ifndef CUBE_H
#define CUBE_H

#include "Entity.h"
#include <QtGui/QMatrix4x4>

class GlVertexArray;
class GlVertexBuffer;
class GlIndexBuffer;
class Cube : public Entity
{
    Q_OBJECT
    Q_PROPERTY(CullMode cullMode READ cullMode WRITE setCullMode)
    Q_PROPERTY(QMatrix4x4 modelMatrix READ modelMatrix WRITE setModelMatrix)

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
    QMatrix4x4 modelMatrix() const;

    QVector3D worldToVoxel(const QVector3D& world) const;

public slots:
    void setCullMode(const CullMode& cullMode);
    void setModelMatrix(const QMatrix4x4& modelMatrix);

private:
    GlVertexArray* _vao;
    GlVertexBuffer* _vbo;
    GlIndexBuffer* _ibo;

    CullMode _cullMode;
    CullMode _r_cullMode;

    QMatrix4x4 _modelMatrix;
    glm::mat4 _r_modelMatrix;
};

#endif // CUBE_H
