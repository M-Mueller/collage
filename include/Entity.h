#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include "grogl/GlProgram.h"

/**
 * @brief The Entity class is the base class of all drawable objects.
 * Because this object is used by the \ref VisualizationRenderer, it must split its properties from the renderer properties.
 * That also means any calls to OpenGL should only be performed in synchronize() and render().
 */
class Entity: public QObject
{
    Q_OBJECT
public:
    Entity(QObject* parent=0);
    virtual ~Entity();

    /**
     * @brief Called during QQuickFramebufferObject::Renderer::synchronize.
     * Use this method to update the properties of the render method with the properties of the QObject.
     */
    virtual void synchronize();

    /**
     * @brief Renders the entity with the given program.
     * The program will be already activated before calling this method.
     * The shader will contain two preset uniforms for the camera:
     * \code
     * uniform mat4 viewMatrix;
     * uniform mat4 projectionMatrix;
     * \endcode
     */
    virtual void render(GlProgram& program) =0;
};

#endif // ENTITY_H
