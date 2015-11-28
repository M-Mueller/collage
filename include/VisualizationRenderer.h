#ifndef VISUALIZATIONRENDERER_H
#define VISUALIZATIONRENDERER_H

#include <QtQuick/QQuickFramebufferObject>

class RenderPass;
class VisualizationRenderer: public QQuickFramebufferObject::Renderer
{
public:
    VisualizationRenderer();

    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size);

    void synchronize(QQuickFramebufferObject* item);
    void render();

private:
    QList<RenderPass*> m_passes;
    QQuickFramebufferObject* m_item;
};

#endif // VISUALIZATIONRENDERER_H
