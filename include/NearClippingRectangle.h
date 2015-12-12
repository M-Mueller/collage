#ifndef NEARCLIPPINGRECTANGLE_H
#define NEARCLIPPINGRECTANGLE_H

#include "Rectangle.h"
#include "Camera.h"

class NearClippingRectangle: public Rectangle
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)

public:
    NearClippingRectangle(QObject* parent=0);

    void synchronize() override;
    void render(GlProgram& program) override;

    Camera* camera() const;

public slots:
    void setCamera(Camera* camera);

private:
    Camera* _camera;
};

#endif // NEARCLIPPINGRECTANGLE_H
