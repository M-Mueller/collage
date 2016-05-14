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

class DepthFunc: public QObject // must be a QObject for the Q_ENUM macro
{
    Q_OBJECT
public:
    enum Enum
    {
        Never,
        Always,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual
    };
    Q_ENUM(Enum)
};

class CullMode: public QObject
{
    Q_OBJECT
public:
    enum Enum
    {
        None,
        Front,
        Back
    };
    Q_ENUM(Enum)
};

class RenderPass: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString vertexShaderPath READ vertexShaderPath WRITE setVertexShaderPath)
    Q_PROPERTY(QString geometryShaderPath READ geometryShaderPath WRITE setGeometryShaderPath)
    Q_PROPERTY(QString fragmentShaderPath READ fragmentShaderPath WRITE setFragmentShaderPath)
    Q_PROPERTY(Framebuffer* renderToTexture READ renderToTexture WRITE setRenderToTexture)
    Q_PROPERTY(QRect viewport READ viewport WRITE setViewport NOTIFY viewportChanged)
    Q_PROPERTY(bool depthTest READ depthTest WRITE setDepthTest NOTIFY depthTestChanged)
    Q_PROPERTY(DepthFunc::Enum depthFunc READ depthFunc WRITE setDepthFunc NOTIFY depthFuncChanged)
    Q_PROPERTY(CullMode::Enum cullMode READ cullMode WRITE setCullMode NOTIFY cullModeChanged)
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
    DepthFunc::Enum depthFunc() const;

    CullMode::Enum cullMode() const;

    bool enabled() const;

public slots:
    void setVertexShaderPath(const QString& vertexShaderPath);
    void setGeometryShaderPath(const QString& geometryShaderPath);
    void setFragmentShaderPath(const QString& fragmentShaderPath);

    void setRenderToTexture(Framebuffer* renderToTexture);
    void setViewport(const QRect& viewport);

    void setDepthTest(bool depthTest);
    void setDepthFunc(DepthFunc::Enum depthFunc);

    void setCullMode(CullMode::Enum cullMode);

    void setEnabled(bool enabled);

    Q_INVOKABLE void reloadShaders();

signals:
    void viewportChanged(QRect viewport);    
    void enabledChanged(bool enabled);
    void depthTestChanged(bool depthTest);
    void depthFuncChanged(DepthFunc::Enum depthFunc);
    void cullModeChanged(CullMode::Enum cullMode);

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

    RendererProperty<bool> _enabled;
    RendererProperty<QRect> _viewport;
    RendererProperty<bool> _depthTest;
    RendererProperty<DepthFunc::Enum> _depthFunc;
    RendererProperty<CullMode::Enum> _cullMode;
};



#endif // SHADERPROGRAM_H
