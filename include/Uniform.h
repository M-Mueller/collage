#ifndef UNIFORM_H
#define UNIFORM_H

#include "RendererElement.h"
#include <QtCore/QObject>

class GlProgram;
class Texture2D;
class Texture3D;

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

class UniformInt: public Uniform
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue)
public:
    UniformInt(QObject* parent=0);

    int value() const;
    virtual void synchronize() override;
    virtual void set(GlProgram &program) override;

public slots:
    void setValue(int value);

private:
    int _value;
    int _r_value;
};

class UniformFloat: public Uniform
{
    Q_OBJECT
    Q_PROPERTY(float value READ value WRITE setValue)
public:
    UniformFloat(QObject* parent=0);

    float value() const;
    virtual void synchronize() override;
    virtual void set(GlProgram &program) override;

public slots:
    void setValue(float value);

private:
    float _value;
    float _r_value;
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
