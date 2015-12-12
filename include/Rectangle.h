#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Entity.h"
#include "Texture2D.h"
#include <QtGui/QVector3D>

class GlVertexBuffer;
class GlIndexBuffer;
class GlVertexArray;

class Rectangle: public Entity
{
    Q_OBJECT
    Q_PROPERTY(QVector3D color0 READ color0 WRITE setColor0)
    Q_PROPERTY(QVector3D color1 READ color1 WRITE setColor1)
    Q_PROPERTY(QVector3D color2 READ color2 WRITE setColor2)
    Q_PROPERTY(QVector3D color3 READ color3 WRITE setColor3)
    Q_PROPERTY(QVector3D texCoord0 READ texCoord0 WRITE setTexCoord0)
    Q_PROPERTY(QVector3D texCoord1 READ texCoord1 WRITE setTexCoord1)
    Q_PROPERTY(QVector3D texCoord2 READ texCoord2 WRITE setTexCoord2)
    Q_PROPERTY(QVector3D texCoord3 READ texCoord3 WRITE setTexCoord3)

public:
    Rectangle(QObject* parent=0);
    virtual ~Rectangle();

    void synchronize() override;
    void render(GlProgram& program) override;

    QVector3D color0() const;
    QVector3D color1() const;
    QVector3D color2() const;
    QVector3D color3() const;

    QVector3D texCoord0() const;
    QVector3D texCoord1() const;
    QVector3D texCoord2() const;
    QVector3D texCoord3() const;

public slots:
    void setColor0(const QVector3D& color0);
    void setColor1(const QVector3D& color1);
    void setColor2(const QVector3D& color2);
    void setColor3(const QVector3D& color3);

    void setTexCoord0(const QVector3D& texCoord0);
    void setTexCoord1(const QVector3D& texCoord1);
    void setTexCoord2(const QVector3D& texCoord2);
    void setTexCoord3(const QVector3D& texCoord3);

private:
    void updateBuffers();

private:
    QVector3D _color0;
    QVector3D _color1;
    QVector3D _color2;
    QVector3D _color3;
    QVector3D _texCoord0;
    QVector3D _texCoord1;
    QVector3D _texCoord2;
    QVector3D _texCoord3;

    bool _update;
    GlVertexArray* _vao;
    GlVertexBuffer* _vbo;
    GlIndexBuffer* _ibo;
};

#endif // TRIANGLE_H
