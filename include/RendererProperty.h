#ifndef RENDERPROPERTY_H
#define RENDERPROPERTY_H

#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>
#include <QtGui/QColor>
#include <QtCore/QString>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

template<typename T, typename U>
inline U RendererProperty_Convert(const T& from)
{
    // TODO: direct conversion if U==T
    return static_cast<U>(from);
}

template<>
inline std::string RendererProperty_Convert(const QString& from)
{
    return from.toStdString();
}

template<>
inline glm::vec3 RendererProperty_Convert(const QVector3D& from)
{
    return glm::vec3(from.x(), from.y(), from.z());
}

template<>
inline glm::vec4 RendererProperty_Convert(const QColor& from)
{
    return glm::vec4(from.red(), from.green(), from.blue(), from.alpha());
}

template<>
inline glm::mat4 RendererProperty_Convert(const QMatrix4x4& from)
{
    return glm::make_mat4(from.constData());
}

/**
 * @brief The RendererProperty class wraps a property that is available both in the main Qt thread and in the OpenGL rendering thread.
 * A RendererProperty objects remains two copies of a variable (that can have different type): the first variable represents the main value of the property that is read/write.
 * This variable should be used by the main Qt thread.
 * The second variable is just a copy of the first one that is updated which a call to synchronize.
 * This variable is read-only and should only be accessed in the rendering thread.
 * It has an undefined value before synchronized is called the first time.
 */
template<typename T, typename U=T>
class RendererProperty
{
public:
    RendererProperty()
    {

    }

    RendererProperty(const T& value):
        value(value)
    {

    }

    /**
     * @brief Returns the value of the property.
     * This is a convenient method to get the value directly without accessing the actual member.
     * \code
     * RendererProperty<int> p(5);
     * int v = p;
     * \endcode
     */
    operator T() const
    {
        return value;
    }

    /**
     * @brief Assigns a new value to the property.
     * This is a convenient method to assign a value directly to the property without accessing the actual member.
     * \code
     * RendererProperty<int> p;
     * p = 5;
     * \endcode
     */
    RendererProperty& operator=(const T& v)
    {
        value = v;
        return *this;
    }

    bool operator==(const T& v)
    {
        return value == v;
    }

    bool operator!=(const T& v)
    {
        return value != v;
    }

    /**
     * @brief Returns the property value for the rendering thread.
     */
    const U& gl() const
    {
        return _r_value;
    }

    /**
     * @brief Should only be called during VisualizationRenderer::synchronize
     */
    void synchronize()
    {
        _r_value = RendererProperty_Convert<T,U>(value);
    }

    /**
     * @brief The actual value of the property.
     */
    T value;

private:
    U _r_value;
};

#endif // RENDERPROPERTY_H
