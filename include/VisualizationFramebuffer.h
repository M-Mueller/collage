#ifndef VISUALIZATIONFRAMEBUFFER_H
#define VISUALIZATIONFRAMEBUFFER_H

#include <QtQuick/QQuickFramebufferObject>

#include "RenderPass.h"

class VisualizationFramebuffer: public QQuickFramebufferObject
{
public:
    VisualizationFramebuffer(QQuickItem * parent = 0);

    Renderer* createRenderer() const;

    QList<RenderPass*> shaderPasses() const;
};

#endif // VISUALIZATIONFRAMEBUFFER_H
