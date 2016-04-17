#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Entity.h"
#include "Texture2D.h"

#include <QtGui/QVector3D>

#include <memory>

class GlVertexBuffer;
class GlIndexBuffer;
class GlVertexArray;

class Rectangle: public Entity
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position0 READ position0 WRITE setPosition0 NOTIFY position0Changed)
    Q_PROPERTY(QVector3D position1 READ position1 WRITE setPosition1 NOTIFY position1Changed)
    Q_PROPERTY(QVector3D position2 READ position2 WRITE setPosition2 NOTIFY position2Changed)
    Q_PROPERTY(QVector3D position3 READ position3 WRITE setPosition3 NOTIFY position3Changed)
    Q_PROPERTY(QVector3D color0 READ color0 WRITE setColor0 NOTIFY color0Changed)
    Q_PROPERTY(QVector3D color1 READ color1 WRITE setColor1 NOTIFY color1Changed)
    Q_PROPERTY(QVector3D color2 READ color2 WRITE setColor2 NOTIFY color2Changed)
    Q_PROPERTY(QVector3D color3 READ color3 WRITE setColor3 NOTIFY color3Changed)

public:
    Rectangle(QObject* parent=0);
    virtual ~Rectangle();

    void synchronize() override;
    void render(GlProgram& program) override;

    QVector3D position0() const;
    QVector3D position1() const;
    QVector3D position2() const;
    QVector3D position3() const;

    QVector3D color0() const;
    QVector3D color1() const;
    QVector3D color2() const;
    QVector3D color3() const;

public slots:
    void setPosition0(QVector3D position0);
    void setPosition1(QVector3D position1);
    void setPosition2(QVector3D position2);
    void setPosition3(QVector3D position3);

    void setColor0(QVector3D color0);
    void setColor1(QVector3D color1);
    void setColor2(QVector3D color2);
    void setColor3(QVector3D color3);

signals:
    void position0Changed(QVector3D position0);
    void position1Changed(QVector3D position1);
    void position2Changed(QVector3D position2);
    void position3Changed(QVector3D position3);

    void color0Changed(QVector3D color0);
    void color1Changed(QVector3D color1);
    void color2Changed(QVector3D color2);
    void color3Changed(QVector3D color3);

private:
    void updateBuffers();

private:
    QVector3D _position0;
    QVector3D _position1;
    QVector3D _position2;
    QVector3D _position3;
    QVector3D _color0;
    QVector3D _color1;
    QVector3D _color2;
    QVector3D _color3;

    bool _update;
    std::unique_ptr<GlVertexArray> _vao;
    std::unique_ptr<GlVertexBuffer> _vbo;
    std::unique_ptr<GlIndexBuffer> _ibo;
};

#endif // TRIANGLE_H
