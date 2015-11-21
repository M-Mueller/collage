#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "RendererElement.h"
#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

class Entity;
class Camera;
class Framebuffer;
class GlProgram;
class RenderPass: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(QString vertexShaderPath READ vertexShaderPath WRITE setVertexShaderPath)
    Q_PROPERTY(QString geometryShaderPath READ geometryShaderPath WRITE setGeometryShaderPath)
    Q_PROPERTY(QString fragmentShaderPath READ fragmentShaderPath WRITE setFragmentShaderPath)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(QQmlListProperty<Entity> entities READ entities)
    Q_PROPERTY(Framebuffer* renderToTexture READ renderToTexture WRITE setRenderToTexture)
    Q_CLASSINFO("DefaultProperty", "entities")
public:
    RenderPass();
    virtual ~RenderPass();

    QString vertexShaderPath() const;
    QString geometryShaderPath() const;
    QString fragmentShaderPath() const;

    virtual void synchronize() override;
    virtual void render();

    QQmlListProperty<Entity> entities();
    Camera* camera() const;
    Framebuffer* renderToTexture() const;

public slots:
    void setVertexShaderPath(const QString& vertexShaderPath);
    void setGeometryShaderPath(const QString& geometryShaderPath);
    void setFragmentShaderPath(const QString& fragmentShaderPath);
    void setCamera(Camera* camera);
    void setRenderToTexture(Framebuffer* renderToTexture);

private:
    static void appendEntity(QQmlListProperty<Entity>* list, Entity* value);
    static Entity* entityAt(QQmlListProperty<Entity>* list, int index);
    static void clearEntities(QQmlListProperty<Entity> *list);
    static int entityCount(QQmlListProperty<Entity> *list);

private:
    QString _vertexShaderPath;
    QString _geometryShaderPath;
    QString _fragmentShaderPath;

    GlProgram* _program;
    Camera* _camera;
    QList<Entity*> _entities;
    Framebuffer* _renderToTexture;
};



#endif // SHADERPROGRAM_H
