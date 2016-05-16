#ifndef VISUALIZATIONFRAMEBUFFER_H
#define VISUALIZATIONFRAMEBUFFER_H

#include <QtQuick/QQuickFramebufferObject>

namespace collage
{
    class RenderPass;
    class Texture;
    class Canvas: public QQuickFramebufferObject
    {
    public:
        Canvas(QQuickItem * parent = 0);

        Renderer* createRenderer() const;

        QList<RenderPass*> shaderPasses() const;
    };
}

#endif // VISUALIZATIONFRAMEBUFFER_H
