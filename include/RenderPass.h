#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <QObject>
#include <QQmlListProperty>

class Entity;
class Camera;
class GlProgram;
class RenderPass: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString vertexShaderPath READ vertexShaderPath WRITE setVertexShaderPath)
    Q_PROPERTY(QString geometryShaderPath READ geometryShaderPath WRITE setGeometryShaderPath)
    Q_PROPERTY(QString fragmentShaderPath READ fragmentShaderPath WRITE setFragmentShaderPath)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(QQmlListProperty<Entity> entities READ entities)
    Q_CLASSINFO("DefaultProperty", "entities")
public:
    RenderPass();

    QString vertexShaderPath() const;
    QString geometryShaderPath() const;
    QString fragmentShaderPath() const;

    virtual void synchronize();
    virtual void render();

    QQmlListProperty<Entity> entities();

    Camera* camera() const;

public slots:
    void setVertexShaderPath(const QString& vertexShaderPath);
    void setGeometryShaderPath(const QString& geometryShaderPath);
    void setFragmentShaderPath(const QString& fragmentShaderPath);
    void setCamera(Camera* camera);

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
};



#endif // SHADERPROGRAM_H
