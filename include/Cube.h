#ifndef CUBE_H
#define CUBE_H

#include "Entity.h"

class GlVertexArray;
class GlVertexBuffer;
class GlIndexBuffer;
class Cube : public Entity
{
    Q_OBJECT
    Q_PROPERTY(CullMode cullMode READ cullMode WRITE setCullMode)

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

public slots:
    void setCullMode(const CullMode& cullMode);

private:
    GlVertexArray* _vao;
    GlVertexBuffer* _vbo;
    GlIndexBuffer* _ibo;

    CullMode _cullMode;
    CullMode _r_cullMode;
};

#endif // CUBE_H
