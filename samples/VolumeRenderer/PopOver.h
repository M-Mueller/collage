#ifndef POPOVER_H
#define POPOVER_H

#include <QtQuick/QQuickWindow>
#include <QtCore/QPoint>

class PopOver: public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* parentItem READ parentItem WRITE setParentItem NOTIFY parentItemChanged)
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin NOTIFY originChanged)

public:
    PopOver(QWindow* parent=0);

    QPointF origin() const;
    QQuickItem* parentItem() const;

public slots:
    void setOrigin(QPointF origin);
    void setParentItem(QQuickItem* parentItem);

signals:
    void originChanged(QPointF origin);
    void parentItemChanged(QQuickItem* parentItem);

private:
    void updatePosition();

private:
    QPointF _origin;
    QQuickItem* _parentItem;
};

#endif // POPOVER_H
