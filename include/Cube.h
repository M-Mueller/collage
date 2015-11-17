#ifndef CUBE_H
#define CUBE_H

#include "Entity.h"

class GlVertexArray;
class GlVertexBuffer;
class GlIndexBuffer;
class Cube : public Entity
{
    Q_OBJECT
public:
    Cube(QObject* parent=0);
    virtual ~Cube();

    void synchronize() override;
    void render(GlProgram& program) override;

private:
    GlVertexArray* _vao;
    GlVertexBuffer* _vbo;
    GlIndexBuffer* _ibo;
};

#endif // CUBE_H
