#ifndef CLEARFRAMEBUFFER_H
#define CLEARFRAMEBUFFER_H

#include "Entity.h"
#include "RendererProperty.h"

#include <QtGui/QColor>

namespace collage
{
    /**
     * The ClearFramebuffer Entity clears the current Framebuffer.
     * In particular, the color-, depth-, stencil- and accumulate buffer can be cleared with custom values.
     * By default nothing is cleared.
     */
    class ClearFramebuffer: public Entity
    {
        Q_OBJECT
        /**
         * Enables/Disables clearing of the color buffer.
         * Disabled by default.
         * @accessors %clearColorBuffer, setClearColorBuffer
         * @notifiers clearColorBufferChanged
         */
        Q_PROPERTY(bool clearColorBuffer READ clearColorBuffer WRITE setClearColorBuffer NOTIFY clearColorBufferChanged)
        /**
         * Enables/Disables clearing of the depth buffer.
         * Disabled by default.
         * @accessors %clearDepthBuffer, setClearDepthBuffer
         * @notifiers clearDepthBufferChanged
         */
        Q_PROPERTY(bool clearDepthBuffer READ clearDepthBuffer WRITE setClearDepthBuffer NOTIFY clearDepthBufferChanged)
        /**
         * Enables/Disables clearing of the stencil buffer.
         * Disabled by default.
         * @accessors %clearStencilBuffer, setClearStencilBuffer
         * @notifiers clearStencilBufferChanged
         */
        Q_PROPERTY(bool clearStencilBuffer READ clearStencilBuffer WRITE setClearStencilBuffer NOTIFY clearStencilBufferChanged)
        /**
         * Sets the color the color buffer is clear with.
         * Defaults to Qt.black.
         * @accessors %clearColor, setClearColor
         * @notifiers clearColorChanged
         */
        Q_PROPERTY(QColor clearColor READ clearColor WRITE setClearColor NOTIFY clearColorChanged)
        /**
         * Sets the value the depth buffer is clear with.
         * Defaults to 1.0.
         * @accessors %clearColor, setClearColor
         * @notifiers clearColorChanged
         */
        Q_PROPERTY(double clearDepth READ clearDepth WRITE setClearDepth NOTIFY clearDepthChanged)
    public:
        ClearFramebuffer(QObject* parent=0);
        virtual ~ClearFramebuffer();

        void synchronize() override;
        void render(GlProgram& program) override;

        bool clearColorBuffer() const;
        bool clearDepthBuffer() const;
        bool clearStencilBuffer() const;

        double clearDepth() const;
        QColor clearColor() const;

    public slots:
        void setClearColorBuffer(bool clearColorBuffer);
        void setClearDepthBuffer(bool clearDepthBuffer);
        void setClearStencilBuffer(bool clearStencilBuffer);

        void setClearDepth(double clearDepth);
        void setClearColor(QColor clearColor);

    signals:
        void clearColorBufferChanged(bool clearColorBuffer);
        void clearDepthBufferChanged(bool clearDepthBuffer);
        void clearStencilBufferChanged(bool clearStencilBuffer);

        void clearDepthChanged(double clearDepth);
        void clearColorChanged(QColor clearColor);

    private:
        RendererProperty<bool> _clearColorBuffer;
        RendererProperty<bool> _clearDepthBuffer;
        RendererProperty<bool> _clearStencilBuffer;

        RendererProperty<QColor, glm::vec4> _clearColor;
        RendererProperty<double> _clearDepth;
    };
}

#endif // CLEARFRAMEBUFFER_H
