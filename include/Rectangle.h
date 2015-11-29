#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Entity.h"
#include "Texture2D.h"

class GlVertexBuffer;
class GlIndexBuffer;
class GlVertexArray;

class Rectangle: public Entity
{
    Q_OBJECT
public:
    Rectangle(QObject* parent=0);
    virtual ~Rectangle();

    void synchronize() override;
    void render(GlProgram& program) override;

private:
    GlVertexArray* _vao;
    GlVertexBuffer* _vbo;
    GlIndexBuffer* _ibo;
};

#endif // TRIANGLE_H
