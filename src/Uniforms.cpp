#include "Uniforms.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "easylogging++.h"

#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>
#include <QtGui/QColor>
#include <QtCore/QEvent>
#include <QtCore/QDynamicPropertyChangeEvent>
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaType>

#include <glue/GlProgram.h>
#include <glue/GlTexture2D.h>
#include <glue/GlTexture3D.h>

Uniforms::Uniforms(QObject* parent):
    Entity(parent),
    _sampler_type(-1)
{
    _sampler_type = qRegisterMetaType<Sampler*>();
}

void Uniforms::synchronize()
{
    Entity::synchronize();

    _uniforms.clear();
    for(int i=0; i<metaObject()->propertyCount(); ++i)
    {
        auto property = metaObject()->property(i);
        QString name = property.name();
        QVariant value = property.read(this);

        if(name != "objectName")
        {
            _uniforms.push_back(QPair<QString, QVariant>(name, value));
        }
    }
}

void Uniforms::render(GlProgram& program)
{
    for(auto uniform: _uniforms)
    {
        QString name = uniform.first;
        QVariant value = uniform.second;

        switch(value.type())
        {
        case QVariant::Double:
            program.setUniform(name.toStdString(), value.toDouble());
            break;
        case QVariant::Int:
            program.setUniform(name.toStdString(), value.toInt());
            break;
        case QVariant::Bool:
            program.setUniform(name.toStdString(), value.toBool());
            break;
        case QVariant::Vector2D:
            program.setUniform(name.toStdString(), value.value<QVector2D>());
            break;
        case QVariant::Vector3D:
            program.setUniform(name.toStdString(), value.value<QVector3D>());
            break;
        case QVariant::Vector4D:
            program.setUniform(name.toStdString(), value.value<QVector4D>());
            break;
        case QVariant::Color:
            program.setUniform(name.toStdString(), value.value<QColor>());
            break;
//        case QVariant::Matrix3x3:
//            program.setUniform(name.toStdString(), value.value<QMatrix3x3>());
//            break;
        case QVariant::Matrix4x4:
            program.setUniform(name.toStdString(), value.value<QMatrix4x4>());
            break;
        case QVariant::UserType:
        {
            if(value.userType() == _sampler_type)
                value.value<Sampler*>()->set(name, program);
            else
                LOG(WARNING) << "Uniform '" << name << "': Unsupported type";
            break;
        }
        default:
            LOG(WARNING) << "Uniform '" << name << "': Unsupported type";
        }
    }
}

bool Uniforms::event(QEvent* event)
{
    if(event->type() == QEvent::DynamicPropertyChange)
    {
        auto changeEvent = dynamic_cast<QDynamicPropertyChangeEvent*>(event);
        if(changeEvent)
        {
            LOG(INFO) << "Uniform added: " << changeEvent->propertyName();
        }
    }

    return QObject::event(event);
}



Sampler::Sampler(QObject* parent):
    QObject(parent),
    _texture(nullptr),
    _unit(0)
{

}

Texture* Sampler::texture() const
{
    return _texture;
}

int Sampler::unit() const
{
    return _unit;
}

void Sampler::set(const QString& name, GlProgram& program)
{
    if(_texture)
    {
        _texture->bind(_unit);
        program.setUniform(name.toStdString(), _unit);
    }
}

void Sampler::setTexture(Texture* texture)
{
    if (_texture == texture)
        return;

    _texture = texture;
    emit textureChanged(texture);
}

void Sampler::setUnit(int unit)
{
    if (_unit == unit)
        return;

    _unit = unit;
    emit unitChanged(unit);
}
