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
    Q_PROPERTY(Texture2D* texture READ texture WRITE setTexture)
public:
    Rectangle(QObject* parent=0);
    virtual ~Rectangle();

    void synchronize() override;
    void render(GlProgram& program) override;

    Texture2D* texture() const;

public slots:
    void setTexture(Texture2D* tex);

private:
    Texture2D* _tex;

    GlVertexArray* _vao;
    GlVertexBuffer* _vbo;
    GlIndexBuffer* _ibo;
};

#endif // TRIANGLE_H
