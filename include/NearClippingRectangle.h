#ifndef NEARCLIPPINGRECTANGLE_H
#define NEARCLIPPINGRECTANGLE_H

#include "Rectangle.h"
#include "Camera.h"
#include "Cube.h"

class NearClippingRectangle: public Rectangle
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(Cube* cube READ cube WRITE setCube)

public:
    NearClippingRectangle(QObject* parent=0);

    void synchronize() override;
    void render(GlProgram& program) override;

    Camera* camera() const;
    Cube* cube() const;

public slots:
    void setCamera(Camera* camera);
    void setCube(Cube* cube);

private:
    Camera* _camera;
    Cube* _cube;
};

#endif // NEARCLIPPINGRECTANGLE_H
