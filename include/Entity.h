#ifndef ENTITY_H
#define ENTITY_H

#include "RendererElement.h"
#include <QtCore/QObject>

class GlProgram;

/**
 * The Entity class is the base class of all drawable objects.
 * Because this object is used by the \ref VisualizationRenderer, it must split its properties from the renderer properties.
 * That also means any calls to OpenGL should only be performed in synchronize() and render().
 */
class Entity: public QObject, public RendererElement
{
    Q_OBJECT
public:
    Entity(QObject* parent=0);
    virtual ~Entity();

    /**
     * Renders the entity with the given program.
     * The program is always activated when this method is called.
     */
    virtual void render(GlProgram& program) =0;

    /**
     * Resets any global state that was change during render().
     * The default implementation does nothing.
     */
    virtual void resetStates(GlProgram& program);
};

#endif // ENTITY_H
