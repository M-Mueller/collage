#ifndef CAMERA_H
#define CAMERA_H

#include "RendererElement.h"
#include "RendererProperty.h"
#include <QtCore/QObject>
#include <QtGui/QMatrix4x4>
#include <glue/GlProgram.h>
#include <glm/glm.hpp>

class Camera: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(QMatrix4x4 viewMatrix READ viewMatrix WRITE setViewMatrix NOTIFY viewMatrixChanged)
    Q_PROPERTY(QMatrix4x4 projectionMatrix READ projectionMatrix WRITE setProjectionMatrix NOTIFY projectionMatrixChanged)
public:
    Camera(QObject* parent=0);

    virtual QMatrix4x4 viewMatrix() const;
    virtual QMatrix4x4 projectionMatrix() const;

    virtual void synchronize() override;
    void applyMatrices(GlProgram& program);

public slots:
    virtual void setViewMatrix(const QMatrix4x4& viewMatrix);
    virtual void setProjectionMatrix(const QMatrix4x4& projectionMatrix);

signals:
    void viewMatrixChanged(QMatrix4x4);
    void projectionMatrixChanged(QMatrix4x4);

protected:
    RendererProperty<QMatrix4x4, glm::mat4> _viewMatrix;
    RendererProperty<QMatrix4x4, glm::mat4> _projectionMatrix;
};

#endif // CAMERA_H
