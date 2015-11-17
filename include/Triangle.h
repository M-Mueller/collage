#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Entity.h"

class GlVertexBuffer;
class GlVertexArray;

class Triangle: public Entity
{
    Q_OBJECT
public:
    Triangle(QObject* parent=0);
    virtual ~Triangle();

    void synchronize() override;
    void render(GlProgram& program) override;

private:
    GlVertexArray* vao;
    GlVertexBuffer* vbo;
};

#endif // TRIANGLE_H
