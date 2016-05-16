#include "Canvas.h"
#include "Renderer.h"
#include "RenderPass.h"

namespace collage
{
    Canvas::Canvas(QQuickItem* parent):
        QQuickFramebufferObject(parent)
    {

    }

    QQuickFramebufferObject::Renderer* Canvas::createRenderer() const
    {
        return new collage::Renderer;
    }

    QList<RenderPass*> Canvas::shaderPasses() const
    {
        QList<RenderPass*> passes;
        for(auto child: children())
        {
            if(auto shader = qobject_cast<RenderPass*>(child))
            {
                passes.append(shader);
            }
        }
        return passes;
    }
}
