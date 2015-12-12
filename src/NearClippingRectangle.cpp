#include "NearClippingRectangle.h"
#include <glad/glad.h>

NearClippingRectangle::NearClippingRectangle(QObject* parent):
    Rectangle(parent)
{

}

void NearClippingRectangle::synchronize()
{
    if(_camera)
    {
        auto viewMatrixInv = _camera->viewMatrix().inverted();
        auto projectionMatrixInv = _camera->projectionMatrix().inverted();

        setColor0(viewMatrixInv*projectionMatrixInv*QVector3D(-1, -1, -1));
        setColor1(viewMatrixInv*projectionMatrixInv*QVector3D(1, -1, -1));
        setColor2(viewMatrixInv*projectionMatrixInv*QVector3D(1, 1, -1));
        setColor3(viewMatrixInv*projectionMatrixInv*QVector3D(-1, 1, -1));

        // convert from world to voxel coordinates
        //TODO: for now the volume is always centered and has unit size
        setColor0(color0()/2 + QVector3D(1, 1, 1)/2);
        setColor1(color1()/2 + QVector3D(1, 1, 1)/2);
        setColor2(color2()/2 + QVector3D(1, 1, 1)/2);
        setColor3(color3()/2 + QVector3D(1, 1, 1)/2);
    }

    Rectangle::synchronize();
}

void NearClippingRectangle::render(GlProgram& program)
{
    if(_camera)
    {
        glDepthMask(GL_FALSE);

        program.setUniform("viewMatrix", glm::mat4());
        program.setUniform("projectionMatrix", glm::mat4());
        Rectangle::render(program);
        _camera->applyMatrices(program);

        glDepthMask(GL_TRUE);
    }
}

Camera* NearClippingRectangle::camera() const
{
    return _camera;
}

void NearClippingRectangle::setCamera(Camera* camera)
{
    _camera = camera;
}
