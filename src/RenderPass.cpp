#include "RenderPass.h"
#include "Entity.h"
#include "Camera.h"
#include "Framebuffer.h"
#include <QFile>
#include <grogl/GlProgram.h>
#include <grogl/GlFrameBuffer.h>

RenderPass::RenderPass():
    _program(nullptr),
    _camera(nullptr),
    _renderToTexture(nullptr)
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
            if(gsSrc.isEmpty())
                _program = new GlProgram(vsSrc.toStdString(), fsSrc.toStdString());
            else
                _program = new GlProgram(vsSrc.toStdString(), gsSrc.toStdString(), fsSrc.toStdString());
        }
    }

    RendererElement::synchronize();
}

void RenderPass::render()
{
    GLint prevFBO;
    GLint prevViewport[4];
    if(_renderToTexture)
    {
        // backup current configuration
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prevFBO);
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        // bind framebuffer and set viewport to render to complete texture
        _renderToTexture->gl()->bind();
        glViewport(0, 0, _renderToTexture->width(), _renderToTexture->height());
    }

    _program->activate();
    if(_camera)
    {
        _camera->applyMatrices(*_program);
    }
    for(auto entity: _entities)
    {
        entity->render(*_program);
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

Framebuffer* RenderPass::renderToTexture() const
{
    return _renderToTexture;
}

void RenderPass::setRenderToTexture(Framebuffer* renderToTexture)
{
    _renderToTexture = renderToTexture;
}

Camera* RenderPass::camera() const
{
    return _camera;
}

void RenderPass::setCamera(Camera* camera)
{
    _camera = camera;
}
