#include "Uniforms.h"

#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>
#include <QtGui/QColor>
#include <QtCore/QEvent>
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaType>

#include <glue/GlProgram.h>

namespace collage
{
    Uniforms::Uniforms(QObject* parent):
        Entity(parent),
        _struct_type(-1)
    {
        _struct_type = qRegisterMetaType<UniformStruct*>();
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
                if(value.type() == QVariant::UserType && value.userType() == _struct_type)
                {
                    // add the properties of UniformStructs as <structname>.<membername>
                    auto ustruct = value.value<UniformStruct*>();
                    if(!ustruct)
                    {
                        qCritical("Unexpected QVariant type");
                        continue;
                    }

                    for(int j=0; j<ustruct->metaObject()->propertyCount(); ++j)
                    {
                        auto memberProperty = ustruct->metaObject()->property(j);
                        QString memberName = memberProperty.name();
                        QVariant memberValue = memberProperty.read(ustruct);

                        if(memberName != "objectName")
                        {
                            _uniforms.push_back(QPair<QString, QVariant>(name + "." + memberName, memberValue));
                        }
                    }
                }
                else
                {
                    _uniforms.push_back(QPair<QString, QVariant>(name, value));
                }
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
                qWarning("Uniform '%s': Unsupported type", name.toLatin1().data());
            }
        }
    }


    UniformStruct::UniformStruct(QObject* parent):
        QObject(parent)
    {

    }
}
