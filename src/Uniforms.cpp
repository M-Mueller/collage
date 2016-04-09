#include "Uniforms.h"
#include "easylogging++.h"

#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>
#include <QtGui/QColor>
#include <QtCore/QEvent>
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaType>

#include <glue/GlProgram.h>

Uniforms::Uniforms(QObject* parent):
    Entity(parent)
{

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
        default:
            LOG(WARNING) << "Uniform '" << name << "': Unsupported type";
        }
    }
}
