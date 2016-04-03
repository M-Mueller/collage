#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "RendererElement.h"
#include "RendererProperty.h"

#include <QtQml/QQmlListProperty>
#include <QtGui/QColor>
#include <QtCore/QObject>
#include <QtCore/QRect>

#include <glm/glm.hpp>

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
    Q_PROPERTY(Framebuffer* renderToTexture READ renderToTexture WRITE setRenderToTexture)
    Q_PROPERTY(QRect viewport READ viewport WRITE setViewport NOTIFY viewportChanged)
    Q_PROPERTY(bool depthTest READ depthTest WRITE setDepthTest)
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

    QQmlListProperty<Entity> entities();
    Framebuffer* renderToTexture() const;
    QRect viewport() const;

    bool depthTest() const;

public slots:
    void setVertexShaderPath(const QString& vertexShaderPath);
    void setGeometryShaderPath(const QString& geometryShaderPath);
    void setFragmentShaderPath(const QString& fragmentShaderPath);

    void setRenderToTexture(Framebuffer* renderToTexture);
    void setViewport(const QRect& viewport);

    void setDepthTest(bool depthTest);

    Q_INVOKABLE void reloadShaders();

signals:
    void viewportChanged(QRect viewport);

private:
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

    QList<Entity*> _entities;

    Framebuffer* _renderToTexture;

    RendererProperty<QRect> _viewport;
    RendererProperty<bool> _depthTest;
};



#endif // SHADERPROGRAM_H
