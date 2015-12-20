#include "NearClippingRectangle.h"
#include "easylogging++.h"
#include <glad/glad.h>

NearClippingRectangle::NearClippingRectangle(QObject* parent):
    Rectangle(parent)
{

}

void NearClippingRectangle::synchronize()
{
    if(_camera && _cube)
    {
        auto viewMatrixInv = _camera->viewMatrix().inverted();
        auto projectionMatrixInv = _camera->projectionMatrix().inverted();

        // in normalized device coordinates, the near clipping plane is from (-1, -1, -1) to (1, 1, -1)
        setColor0(_cube->worldToVoxel(viewMatrixInv*projectionMatrixInv*QVector3D(-1, -1, -1)));
        setColor1(_cube->worldToVoxel(viewMatrixInv*projectionMatrixInv*QVector3D(1, -1, -1)));
        setColor2(_cube->worldToVoxel(viewMatrixInv*projectionMatrixInv*QVector3D(1, 1, -1)));
        setColor3(_cube->worldToVoxel(viewMatrixInv*projectionMatrixInv*QVector3D(-1, 1, -1)));
    }
    else
    {
        LOG(DEBUG) << "Cannot create NearClippingRectangle";
    }

    Rectangle::synchronize();
}

void NearClippingRectangle::render(GlProgram& program)
{
    if(_camera && _cube)
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

Cube* NearClippingRectangle::cube() const
{
    return _cube;
}

void NearClippingRectangle::setCube(Cube* cube)
{
    _cube = cube;
}
