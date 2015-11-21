#include "VisualizationRenderer.h"
#include "VisualizationFramebuffer.h"
#include "RenderPass.h"
#include "Texture2D.h"

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
    for(auto texture: vfb->textures())
    {
        texture->synchronize();
    }
}

void VisualizationRenderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for(auto pass: m_passes)
    {
        pass->synchronize();
    }
    for(auto pass: m_passes)
    {
        pass->render();
    }
}

