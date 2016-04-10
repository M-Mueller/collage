function create(x, y, parent, content) {
    var component = Qt.createComponent("PopupMenu.qml")
    var popup = component.createObject(null)

    var screen = parent.mapToItem(null, x-popup.width/2, y)

    var topLevel = parent
    while(topLevel.parent) {
        topLevel = topLevel.parent
    }

    popup.x = screen.x + topLevel.x
    popup.y = screen.y + topLevel.y
    popup.content.sourceComponent = content
    return popup
}
