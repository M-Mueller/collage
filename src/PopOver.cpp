#include "PopOver.h"
#include <QtQuick/QQuickItem>

PopOver::PopOver(QWindow* parent):
    QQuickWindow(parent),
    _origin(0, 0),
    _parentItem(nullptr)
{
    setFlags(Qt::FramelessWindowHint);
    setColor(Qt::transparent);

    connect(this, &QWindow::activeChanged, [this]()
    {
        if(!isActive())
            close();
    });
}

QPointF PopOver::origin() const
{
    return _origin;
}

QQuickItem* PopOver::parentItem() const
{
    return _parentItem;
}

void PopOver::setOrigin(QPointF origin)
{
    if (_origin == origin)
        return;

    _origin = origin;
    updatePosition();
    emit originChanged(origin);
}

void PopOver::setParentItem(QQuickItem* parentItem)
{
    if (_parentItem == parentItem)
        return;

    _parentItem = parentItem;
    updatePosition();
    emit parentItemChanged(parentItem);
}

void PopOver::updatePosition()
{
    QPoint globalOrigin = _origin.toPoint();
    if(_parentItem && _parentItem->window())
    {
        QPointF screen = _parentItem->mapToScene(_origin);
        globalOrigin = _parentItem->window()->mapToGlobal(screen.toPoint());
    }
    setX(globalOrigin.x() - width()/2);
    setY(globalOrigin.y() - height());
}
