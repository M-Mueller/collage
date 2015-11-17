#include "TurnTableCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TurnTableCamera::TurnTableCamera(QObject* parent):
    Camera(parent),
    _phi(0.0),
    _theta(glm::radians(90.0)),
    _radius(4.0)
{
    //TODO
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(65.0f), 640/(float)480, 1.0f, 100.f);
    setProjectionMatrix(QMatrix4x4(glm::value_ptr(projectionMatrix)));
    updateViewMatrix();
}

double TurnTableCamera::phi() const
{
    return _phi;
}

void TurnTableCamera::setPhi(double phi)
{
    _phi = phi;
    if(_phi < 0)
        _phi = 2*glm::pi<double>() + _phi;
    while(_phi >= 2*glm::pi<double>())
        _phi -= 2*glm::pi<double>();
    assert(_phi >= 0 && _phi < 2*glm::pi<double>());
    updateViewMatrix();
    emit phiChanged(_phi);
}

double TurnTableCamera::theta() const
{
    return _theta;
}

void TurnTableCamera::setTheta(double theta)
{
    _theta = glm::clamp(theta, 0.1, glm::pi<double>()); // TODO: restrict to 0.1 because of wrong up vector (see below)
    updateViewMatrix();
    emit thetaChanged(_theta);
}

double TurnTableCamera::radius() const
{
    return _radius;
}

void TurnTableCamera::setRadius(double radius)
{
    _radius = glm::clamp(radius, 0.1, 100.0);
    updateViewMatrix();
    emit radiusChanged(_radius);
}

void TurnTableCamera::updateViewMatrix()
{
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system#Coordinate_system_conversions
    // x goes to left, y up and z to front
    double x = _radius * glm::sin(_theta) * glm::sin(_phi);
    double y = _radius * glm::cos(_theta);
    double z = _radius * glm::sin(_theta) * glm::cos(_phi);

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //TODO: compute up vector
    setViewMatrix(QMatrix4x4(glm::value_ptr(viewMatrix)).transposed());
}

