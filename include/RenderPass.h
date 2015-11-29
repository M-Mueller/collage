#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "RendererElement.h"
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtQml/QQmlListProperty>

class Entity;
class Camera;
class Framebuffer;
class Uniform;
class GlProgram;
class RenderPass: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(QString vertexShaderPath READ vertexShaderPath WRITE setVertexShaderPath)
    Q_PROPERTY(QString geometryShaderPath READ geometryShaderPath WRITE setGeometryShaderPath)
    Q_PROPERTY(QString fragmentShaderPath READ fragmentShaderPath WRITE setFragmentShaderPath)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(Framebuffer* renderToTexture READ renderToTexture WRITE setRenderToTexture)
    Q_PROPERTY(QRect viewport READ viewport WRITE setViewport)
    Q_PROPERTY(QQmlListProperty<Uniform> uniforms READ uniforms)
    Q_PROPERTY(QQmlListProperty<Entity> entities READ entities)
    Q_CLASSINFO("DefaultProperty", "entities")
public:
    RenderPass(QObject* parent=0);
    virtual ~RenderPass();

    QString vertexShaderPath() const;
    QString geometryShaderPath() const;
    QString fragmentShaderPath() const;

    virtual void synchronize() override;
    virtual void render();

    QQmlListProperty<Uniform> uniforms();
    QQmlListProperty<Entity> entities();
    Camera* camera() const;
    Framebuffer* renderToTexture() const;
    QRect viewport() const;

public slots:
    void setVertexShaderPath(const QString& vertexShaderPath);
    void setGeometryShaderPath(const QString& geometryShaderPath);
    void setFragmentShaderPath(const QString& fragmentShaderPath);
    void setCamera(Camera* camera);
    void setRenderToTexture(Framebuffer* renderToTexture);
    void setViewport(const QRect& viewport);

    Q_INVOKABLE void reloadShaders();

private:
    static void appendUniform(QQmlListProperty<Uniform>* list, Uniform* value);
    static Uniform* uniformAt(QQmlListProperty<Uniform>* list, int index);
    static void clearUniform(QQmlListProperty<Uniform> *list);
    static int uniformCount(QQmlListProperty<Uniform> *list);

    static void appendEntity(QQmlListProperty<Entity>* list, Entity* value);
    static Entity* entityAt(QQmlListProperty<Entity>* list, int index);
    static void clearEntities(QQmlListProperty<Entity> *list);
    static int entityCount(QQmlListProperty<Entity> *list);

private:
    QString _vertexShaderPath;
    QString _geometryShaderPath;
    QString _fragmentShaderPath;
    bool _forceShaderReload;

    GlProgram* _program;
    Camera* _camera;
    QList<Uniform*> _uniforms;
    QList<Entity*> _entities;
    Framebuffer* _renderToTexture;
    QRect _viewport;

    // members that can be used during render
    QRect _r_viewport;
};



#endif // SHADERPROGRAM_H
