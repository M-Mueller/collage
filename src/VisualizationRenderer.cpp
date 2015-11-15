#include "VisualizationRenderer.h"
#include "VisualizationFramebuffer.h"

#include <glad/glad.h>

#include <QtGui/QOpenGLFramebufferObject>

VisualizationRenderer::VisualizationRenderer()
{
    static bool init = false;
    if(!init)
    {
       gladLoadGL();
       init = true;
    }
}

QOpenGLFramebufferObject* VisualizationRenderer::createFramebufferObject(const QSize &size)
 {
     QOpenGLFramebufferObjectFormat format;
     format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
     // optionally enable multisampling by doing format.setSamples(4);
     return new QOpenGLFramebufferObject(size, format);
}

void VisualizationRenderer::synchronize(QQuickFramebufferObject* item)
{
    VisualizationFramebuffer* vfb = static_cast<VisualizationFramebuffer*>(item);
    m_passes = vfb->shaderPasses();
    for(auto pass: m_passes)
    {
        pass->synchronize();
    }
}

 void VisualizationRenderer::render()
 {
    for(auto pass: m_passes)
    {
        pass->render();
    }
 }
