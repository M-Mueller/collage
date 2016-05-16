#ifndef VISUALIZATIONRENDERER_H
#define VISUALIZATIONRENDERER_H

#include <QtQuick/QQuickFramebufferObject>

namespace collage
{
    class RenderPass;
    class Renderer: public QQuickFramebufferObject::Renderer
    {
    public:
        Renderer();

        QOpenGLFramebufferObject* createFramebufferObject(const QSize &size);

        void synchronize(QQuickFramebufferObject* item);
        void render();

    private:
        QList<RenderPass*> m_passes;
        QQuickFramebufferObject* m_item;
    };
}

#endif // VISUALIZATIONRENDERER_H
