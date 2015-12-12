#ifndef UNIFORM_H
#define UNIFORM_H

#include "RendererElement.h"
#include <QtGui/QVector3D>
#include <QtCore/QObject>

#include <glm/glm.hpp>

class GlProgram;
class Texture2D;
class Texture3D;

/**
 * @brief The Uniform class represents a single Uniform in a GlProgram.
 */
class Uniform: public QObject, public RendererElement
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
public:
    Uniform(QObject* parent=0);
    virtual ~Uniform();

    QString name() const;

    virtual void synchronize() override;

    virtual void set(GlProgram& program) =0;
    virtual void releaseResources();

public slots:
    void setName(const QString& name);

protected:
    QString _name;
    std::string _r_name;
};

/**
 * @brief The UniformTyped class is a convenient class to create Uniforms of certain types.
 * The template type T is the type that is exposed and which lives in the main Qt thread.
 * The optional template type U is the type of the internal copy of T that lives in the render thread.
 */
template<typename T, typename U=T>
class UniformTyped: public Uniform
{
public:
    UniformTyped(QObject* parent=0);

    T value() const
    {
        return _value;
    }

    void setValue(T value)
    {
        _value = value;
    }

    virtual void synchronize() override;
    virtual void set(GlProgram &program) override;

protected:
    T _value;
    U _r_value;
};

class UniformInt: public UniformTyped<int>
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue)
public:
    UniformInt(QObject* parent=0);
};

class UniformFloat: public UniformTyped<float>
{
    Q_OBJECT
    Q_PROPERTY(float value READ value WRITE setValue)
public:
    UniformFloat(QObject* parent=0);
};

class UniformVec3: public UniformTyped<QVector3D, glm::vec3>
{
    Q_OBJECT
    Q_PROPERTY(QVector3D value READ value WRITE setValue)
public:
    UniformVec3(QObject* parent=0);
};

class UniformSampler2D: public Uniform
{
    Q_OBJECT
    Q_PROPERTY(Texture2D* value READ value WRITE setValue)
    Q_PROPERTY(int unit READ unit WRITE setUnit)
public:
    UniformSampler2D(QObject* parent=0);

    Texture2D* value() const;
    int unit() const;

    virtual void synchronize() override;
    virtual void set(GlProgram &program) override;
    virtual void releaseResources() override;

public slots:
    void setValue(Texture2D* value);
    void setUnit(int unit);

private:
    Texture2D* _value;
    int _unit;
    int _r_unit;
};

class UniformSampler3D: public Uniform
{
    Q_OBJECT
    Q_PROPERTY(Texture3D* value READ value WRITE setValue)
    Q_PROPERTY(int unit READ unit WRITE setUnit)
public:
    UniformSampler3D(QObject* parent=0);

    Texture3D* value() const;
    int unit() const;

    virtual void synchronize() override;
    virtual void set(GlProgram &program) override;
    virtual void releaseResources() override;

public slots:
    void setValue(Texture3D* value);
    void setUnit(int unit);

private:
    Texture3D* _value;
    int _unit;
    int _r_unit;
};

#endif // UNIFORM_H
