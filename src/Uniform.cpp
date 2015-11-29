#include "Uniform.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include <grogl/GlProgram.h>
#include <grogl/GlTexture2D.h>
#include <grogl/GlTexture3D.h>

Uniform::Uniform(QObject* parent):
    QObject(parent)
{

}

Uniform::~Uniform()
{

}

QString Uniform::name() const
{
    return _name;
}

void Uniform::synchronize()
{
    _r_name = _name.toStdString();
}

void Uniform::releaseResources()
{

}

void Uniform::setName(const QString& name)
{
    _name = name;
}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformInt::UniformInt(QObject* parent):
    Uniform(parent)
{

}

int UniformInt::value() const
{
    return _value;
}

void UniformInt::synchronize()
{
    Uniform::synchronize();
    _r_value = _value;
}

void UniformInt::set(GlProgram& program)
{
    program.setUniform(_r_name, _r_value);
}

void UniformInt::setValue(int value)
{
    _value = value;
}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformFloat::UniformFloat(QObject* parent):
    Uniform(parent)
{

}

float UniformFloat::value() const
{
    return _value;
}

void UniformFloat::synchronize()
{
    Uniform::synchronize();
    _r_value = _value;
}

void UniformFloat::set(GlProgram& program)
{
    program.setUniform(_r_name, _r_value);
}

void UniformFloat::setValue(float value)
{
    _value = value;
}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformSampler2D::UniformSampler2D(QObject* parent):
    Uniform(parent)
{

}

Texture2D* UniformSampler2D::value() const
{
    return _value;
}

void UniformSampler2D::synchronize()
{
    Uniform::synchronize();
    _r_unit = _unit;
}

void UniformSampler2D::set(GlProgram& program)
{
    _value->gl()->bind(_unit);
    program.setUniform(_r_name, _unit);
}

void UniformSampler2D::releaseResources()
{
    _value->gl()->release();
}

int UniformSampler2D::unit() const
{
    return _unit;
}

void UniformSampler2D::setUnit(int unit)
{
    _unit = unit;
}

void UniformSampler2D::setValue(Texture2D* value)
{
    _value = value;
}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformSampler3D::UniformSampler3D(QObject* parent):
    Uniform(parent)
{

}

Texture3D* UniformSampler3D::value() const
{
    return _value;
}

void UniformSampler3D::synchronize()
{
    Uniform::synchronize();
    _r_unit = _unit;
}

void UniformSampler3D::set(GlProgram& program)
{
    _value->gl()->bind(_unit);
    program.setUniform(_r_name, _unit);
}

void UniformSampler3D::releaseResources()
{
    _value->gl()->release();
}

int UniformSampler3D::unit() const
{
    return _unit;
}

void UniformSampler3D::setUnit(int unit)
{
    _unit = unit;
}

void UniformSampler3D::setValue(Texture3D* value)
{
    _value = value;
}
