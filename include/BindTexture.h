#ifndef BINDTEXTURE_H
#define BINDTEXTURE_H

#include "Entity.h"
#include "RendererProperty.h"

class Texture;

class BindTexture: public Entity
{
    Q_OBJECT
    Q_PROPERTY(Texture* texture READ texture WRITE setTexture NOTIFY textureChanged)
    Q_PROPERTY(int unit READ unit WRITE setUnit NOTIFY unitChanged)
public:
    BindTexture(QObject* parent=0);

    void synchronize() override;
    void render(GlProgram& program) override;
    void resetStates(GlProgram& program) override;

    Texture* texture() const;
    int unit() const;

public slots:
    void setTexture(Texture* texture);
    void setUnit(int unit);

signals:
    void textureChanged(Texture* texture);
    void unitChanged(int unit);

private:
    Texture* _texture;
    RendererProperty<int> _unit;
};

#endif // BINDTEXTURE_H
