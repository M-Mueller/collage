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

    bool event(QEvent* event) override;

private:
    QList<QPair<QString, QVariant>> _uniforms;
    int _sampler_type;
};

class Texture;

/**
 * The Sampler class is a specialized property to set samplers.
 * \code
 * Texture2D {
 *     id: tex
 *     ...
 * }
 * Uniforms {
 *     property Sampler colors: Sampler {
 *          texture: tex
 *          unit: 1
 *     }
 * }
 * \encode
 * Sets the "colors" uniform with GLSL type sampler2D to texture unit 1 and binds the texture to unit 1.
 *
 * A Sampler works with any Texture object (e.g. Texture2D and Texture3D).
 *
 * This class should only be used as a property of a Uniforms object.
 */
class Sampler: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Texture* texture READ texture WRITE setTexture NOTIFY textureChanged)
    Q_PROPERTY(int unit READ unit WRITE setUnit NOTIFY unitChanged)
public:
    Sampler(QObject* parent=0);

    Texture* texture() const;
    int unit() const;

    void set(const QString& name, GlProgram& program);

public slots:
    void setTexture(Texture* texture);
    void setUnit(int unit);

signals:
    void textureChanged(Texture* texture);
    void unitChanged(int unit);

private:
    Texture* _texture;
    int _unit;
};
Q_DECLARE_METATYPE(Sampler*)

#endif // UNIFORMS_H
