#ifndef RENDERERELEMENT_H
#define RENDERERELEMENT_H

/**
 * @brief The RendererElement class defines an interface for all objects that are part of the Renderer.
 * Because the rendering thread is separate from the main thread, OpenGL objects cannot be created directly in every object but only when the context is current.
 * The RendererElement interface provides the synchronize() method that is called whenever OpenGL objects should be updated.
 * Subclasses should use this method to update their internal OpenGL objects.
 */
class RendererElement
{
public:
    /**
     * @brief Called during QQuickFramebufferObject::Renderer::synchronize with the OpenGL render context as current context.
     * Use this method to initialize/update all OpenGL related objects.
     * The default implementation will call synchronize() on all children of a QObject.
     * The rendering OpenGL context must be the current context when calling this method!
     */
    virtual void synchronize();
};

#endif // RENDERERELEMENT_H