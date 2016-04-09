#include "RenderPass.h"
#include "Entity.h"
#include "Framebuffer.h"

#include <QtCore/QFile>

#include <glue/GlProgram.h>
#include <glue/GlFrameBuffer.h>

#include <glad/glad.h>

#include "easylogging++.h"

RenderPass::RenderPass(QObject* parent):
    QObject(parent),
    _forceShaderReload(false),
    _program(nullptr),
    _renderToTexture(nullptr),
    _viewport(QRect())
{

}

RenderPass::~RenderPass()
{
    delete _program;
}

QString RenderPass::vertexShaderPath() const
{
    return _vertexShaderPath;
}

void RenderPass::setVertexShaderPath(const QString& vertexShaderPath)
{
    _vertexShaderPath = vertexShaderPath;
}

QString RenderPass::geometryShaderPath() const
{
    return _geometryShaderPath;
}

void RenderPass::setGeometryShaderPath(const QString& geometryShaderPath)
{
    _geometryShaderPath = geometryShaderPath;
}

QString RenderPass::fragmentShaderPath() const
{
    return _fragmentShaderPath;
}

void RenderPass::setFragmentShaderPath(const QString& fragmentShaderPath)
{
    _fragmentShaderPath = fragmentShaderPath;
}

void RenderPass::synchronize()
{
    if(_forceShaderReload)
    {
        delete _program;
        _program = nullptr;
        _forceShaderReload = false;
    }

    if(!_program)
    {
        QString vsSrc, gsSrc, fsSrc;

        QFile vs(_vertexShaderPath);
        if(vs.open(QIODevice::ReadOnly | QIODevice::Text))
            vsSrc = vs.readAll();

        if(!_geometryShaderPath.isEmpty())
        {
            QFile gs(_geometryShaderPath);
            if(gs.open(QIODevice::ReadOnly | QIODevice::Text))
                gsSrc = gs.readAll();
        }

        QFile fs(_fragmentShaderPath);
        if(fs.open(QIODevice::ReadOnly | QIODevice::Text))
            fsSrc = fs.readAll();

        if(!vsSrc.isEmpty() && !fsSrc.isEmpty())
        {
            try
            {
                if(gsSrc.isEmpty())
                    _program = new GlProgram(vsSrc.toStdString(), fsSrc.toStdString());
                else
                    _program = new GlProgram(vsSrc.toStdString(), gsSrc.toStdString(), fsSrc.toStdString());
            }
            catch(std::exception e)
            {
                LOG(ERROR) << "Failed to create program: " << e.what();
            }
        }
        else
        {
            LOG(ERROR) << "Failed to load shader source";
        }
    }

    _viewport.synchronize();
    _depthTest.synchronize();

    RendererElement::synchronize();
}

void RenderPass::render()
{
    if(!_program)
        return;

    GLint prevFBO = 0;
    GLint prevViewport[4];
    if(_renderToTexture)
    {
        // backup current configuration
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prevFBO);
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        // bind framebuffer and set viewport to render to complete texture
        _renderToTexture->gl()->bind();
        glViewport(_viewport.gl().x(), _viewport.gl().y(), _viewport.gl().width(), _viewport.gl().height());
    }

    if(_depthTest.gl())
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    _program->activate();

    for(auto entity: _entities)
    {
        entity->render(*_program);
    }

    for(auto entity = _entities.rbegin(); entity != _entities.rend(); ++entity)
    {
        (*entity)->resetStates(*_program);
    }

    _program->deactivate();

    if(_renderToTexture)
    {
        // restore previous viewport
        glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
        glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    }
}

QQmlListProperty<Entity> RenderPass::entities()
{
    return QQmlListProperty<Entity>(this, 0, &RenderPass::appendEntity, &RenderPass::entityCount,
                                    &RenderPass::entityAt, &RenderPass::clearEntities);
}


void RenderPass::appendEntity(QQmlListProperty<Entity>* list, Entity* value)
{
    if(RenderPass* pass = qobject_cast<RenderPass*>(list->object))
    {
        if(!pass->_entities.contains(value))
        {
            pass->_entities.append(value);
        }
    }
}

Entity* RenderPass::entityAt(QQmlListProperty<Entity>* list, int index)
{
    if(RenderPass* pass = qobject_cast<RenderPass*>(list->object))
    {
        return pass->_entities.value(index, nullptr);
    }
    return nullptr;
}

void RenderPass::clearEntities(QQmlListProperty<Entity>* list)
{
    if(RenderPass* pass = qobject_cast<RenderPass*>(list->object))
    {
        pass->_entities.clear();
    }
}

int RenderPass::entityCount(QQmlListProperty<Entity>* list)
{
    if(RenderPass* pass = qobject_cast<RenderPass*>(list->object))
    {
        return pass->_entities.count();
    }
    return 0;
}

bool RenderPass::depthTest() const
{
    return _depthTest;
}

void RenderPass::setDepthTest(bool depthTest)
{
    _depthTest = depthTest;
}

QRect RenderPass::viewport() const
{
    return _viewport;
}

void RenderPass::setViewport(const QRect& viewport)
{
    if(_viewport == viewport)
        return;

    _viewport = viewport;
    emit viewportChanged(viewport);
}

void RenderPass::reloadShaders()
{
    _forceShaderReload = true;
}

Framebuffer* RenderPass::renderToTexture() const
{
    return _renderToTexture;
}

void RenderPass::setRenderToTexture(Framebuffer* renderToTexture)
{
    _renderToTexture = renderToTexture;
}
