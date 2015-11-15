#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include "grogl/GlProgram.h"

class Entity: public QObject
{
    Q_OBJECT
public:
    Entity(QObject* parent=0);
    virtual ~Entity();

    virtual void synchronize();
    virtual void render(GlProgram& program) =0;
};

#endif // ENTITY_H
