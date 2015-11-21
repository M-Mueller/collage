#include "VisualizationFramebuffer.h"
#include "VisualizationRenderer.h"
#include "RenderPass.h"
#include "Texture2D.h"

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

QList<Texture2D*> VisualizationFramebuffer::textures() const
{
    QList<Texture2D*> passes;
    for(auto child: children())
    {
        if(auto shader = qobject_cast<Texture2D*>(child))
        {
            passes.append(shader);
        }
    }
    return passes;
}

