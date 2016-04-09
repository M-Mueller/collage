#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "Entity.h"

/**
 * The Uniforms class sets the uniforms of a GLSL program.
 * A Uniforms object will use each of its properties as a GLSL uniform.
 * The name and type of the property define the name and type of the uniform.
 *
 * Example:
 * \code
 * Uniforms {
 *      property double scale: 0.5
 *      property matrix4x4 viewMatrix: camera.viewMatrix
 * }
 * \endcode
 * Sets the two uniforms "scale" and "viewMatrix" with the GLSL values float and mat4.
 */
class Uniforms: public Entity
{
    Q_OBJECT
public:
    Uniforms(QObject* parent=0);

    void synchronize() override;
    void render(GlProgram& program) override;

private:
    QList<QPair<QString, QVariant>> _uniforms;
};

#endif // UNIFORMS_H
