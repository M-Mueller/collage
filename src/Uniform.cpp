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

template<typename T, typename U>
U _uniformConvert(const T& from)
{
    return static_cast<U>(from);
}

template<>
glm::vec3 _uniformConvert(const QVector3D& from)
{
    return glm::vec3(from.x(), from.y(), from.z());
}

template<typename T, typename U>
UniformTyped<T, U>::UniformTyped(QObject* parent):
    Uniform(parent)
{

}

template<typename T, typename U>
void UniformTyped<T, U>::synchronize()
{
    Uniform::synchronize();
    _r_value = _uniformConvert<T,U>(_value);
}

//TODO
//template<typename T>
//void UniformTyped<T, T>::synchronize()
//{
//    Uniform::synchronize();
//    _r_value = _value;
//}

template<typename T, typename U>
void UniformTyped<T, U>::set(GlProgram& program)
{
    program.setUniform(_r_name, _r_value);
}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformInt::UniformInt(QObject* parent):
    UniformTyped(parent)
{

}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformFloat::UniformFloat(QObject* parent):
    UniformTyped(parent)
{

}

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

UniformVec3::UniformVec3(QObject* parent):
    UniformTyped(parent)
{

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
