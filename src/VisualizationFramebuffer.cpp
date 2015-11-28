#include "VisualizationFramebuffer.h"
#include "VisualizationRenderer.h"
#include "RenderPass.h"

VisualizationFramebuffer::VisualizationFramebuffer(QQuickItem* parent):
    QQuickFramebufferObject(parent)
{

}

QQuickFramebufferObject::Renderer*VisualizationFramebuffer::createRenderer() const
{
    return new VisualizationRenderer;
}

QList<RenderPass*> VisualizationFramebuffer::shaderPasses() const
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
