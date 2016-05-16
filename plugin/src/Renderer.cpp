#include "Renderer.h"
#include "Canvas.h"
#include "RendererElement.h"
#include "RenderPass.h"
#include "Texture2D.h"

#include <glad/glad.h>
#include <glue/GlQuery.h>

#include <QtGui/QOpenGLFramebufferObject>
#include <QtQuick/QQuickWindow>

namespace collage
{
    Renderer::Renderer():
        m_item(nullptr)
    {
        static bool init = false;
        if(!init)
        {
            gladLoadGL();
            init = true;
        }
    }

    QOpenGLFramebufferObject* Renderer::createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        // optionally enable multisampling by doing format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void Renderer::synchronize(QQuickFramebufferObject* item)
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Renderer::synchronize");

        Canvas* vfb = static_cast<Canvas*>(item);
        m_passes = vfb->shaderPasses();
        for(auto child: vfb->children())
        {
            if(auto element = dynamic_cast<RendererElement*>(child))
            {
                element->synchronize();
            }
        }
        m_item = item;

        glPopDebugGroup();
    }

    void Renderer::render()
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Renderer::render");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        GlQuery query(GlQuery::Target::TimeElapsed);
        double total = 0;
        for(auto pass: m_passes)
        {
            query.begin();
            pass->render();
            query.end();
            double elapsed = static_cast<double>(query.retrieve())/1e6;
            total += elapsed;
            qDebug("%s took %f ms", pass->objectName().toLatin1().data(), elapsed);
        }

        qDebug("Rendering took %f ms", total);

        glPopDebugGroup();

        if(m_item)
            m_item->window()->resetOpenGLState();
    }
}
