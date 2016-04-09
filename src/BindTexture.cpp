#include "BindTexture.h"
#include "Texture.h"

BindTexture::BindTexture(QObject* parent):
    Entity(parent),
    _texture(nullptr),
    _unit(0)
{

}

void BindTexture::synchronize()
{
    Entity::synchronize();
    _unit.synchronize();
}

void BindTexture::render(GlProgram& /*program*/)
{
    if(_texture)
        _texture->bind(_unit.gl());
}

void BindTexture::resetStates(GlProgram& /*program*/)
{
    if(_texture)
        _texture->unbind(_unit.gl());
}

Texture* BindTexture::texture() const
{
    return _texture;
}

int BindTexture::unit() const
{
    return _unit;
}

void BindTexture::setTexture(Texture* texture)
{
    if (_texture == texture)
        return;

    _texture = texture;
    emit textureChanged(texture);
}

void BindTexture::setUnit(int unit)
{
    if (_unit == unit)
        return;

    _unit = unit;
    emit unitChanged(unit);
}
