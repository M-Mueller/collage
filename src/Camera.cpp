#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(QObject* parent):
    QObject(parent)
{

}

QMatrix4x4 Camera::viewMatrix() const
{
    return _viewMatrix;
}

QMatrix4x4 Camera::projectionMatrix() const
{
    return _projectionMatrix;
}

void Camera::synchronize()
{
    _viewMatrix.synchronize();
    _projectionMatrix.synchronize();
    RendererElement::synchronize();
}

void Camera::applyMatrices(GlProgram& program)
{
    program.setUniform("viewMatrix", _viewMatrix.gl());
    program.setUniform("projectionMatrix", _projectionMatrix.gl());
}

void Camera::setViewMatrix(const QMatrix4x4& viewMatrix)
{
    _viewMatrix = viewMatrix;
    emit viewMatrixChanged(_viewMatrix);
}

void Camera::setProjectionMatrix(const QMatrix4x4& projectionMatrix)
{
    _projectionMatrix = projectionMatrix;
    emit projectionMatrixChanged(_projectionMatrix);
}

