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
    _r_viewMatrix = glm::make_mat4(_viewMatrix.constData());
    _r_projectionMatrix = glm::make_mat4(_projectionMatrix.constData());
    RendererElement::synchronize();
}

void Camera::applyMatrices(GlProgram& program)
{
    program.setUniform("viewMatrix", _r_viewMatrix);
    program.setUniform("projectionMatrix", _r_projectionMatrix);
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

