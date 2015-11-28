#include "VisualizationRenderer.h"
#include "VisualizationFramebuffer.h"
#include "RendererElement.h"
#include "RenderPass.h"
#include "Texture2D.h"

#include <glad/glad.h>

#include <QtGui/QOpenGLFramebufferObject>
#include <QtQuick/QQuickWindow>

VisualizationRenderer::VisualizationRenderer():
    m_item(nullptr)
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
    for(auto child: vfb->children())
    {
        if(auto element = dynamic_cast<RendererElement*>(child))
        {
            element->synchronize();
        }
    }
    m_item = item;
}

void VisualizationRenderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for(auto pass: m_passes)
    {
        pass->render();
    }
    if(m_item)
        m_item->window()->resetOpenGLState();
}

