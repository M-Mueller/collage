import QtQuick 2.0

Item {

    signal transferFunctionChanged()

    ListModel {
        id: controlPoints
        ListElement {
            pos: 0
            alpha: 0
            color: "black"
        }
        ListElement {
            pos: 1
            alpha: 1
            color: "white"
        }

        onDataChanged: transferFunctionChanged()
    }

//    onTransferFunctionChanged: {
//        var tf = transferFunction(256)
//        for(var i=0; i<tf.length; ++i) {
//            console.log(i/256 + ": " + tf[i])
//        }
//        console.log()
//    }

    function addControlPoint(pos, alpha, color) {
        // find the current insertion index
        // control points must be order by pos
        for(var i=0; i<controlPoints.count; ++i) {
            var point = controlPoints.get(i)
            if(pos < point.pos)
                break
        }
        // don't add after the last element
        if (i == controlPoints.count)
            i--;

        controlPoints.insert(i, {"pos": pos, "alpha": alpha, "color": color})

        transferFunctionChanged()
    }

    function removeControlPoint(pos) {
        if(pos === 0.0 || pos === 1.0)
            return;

        // find the current insertion index
        // control points must be order by pos
        for(var i=0; i<controlPoints.count; ++i) {
            var point = controlPoints.get(i)
            if(pos === point.pos)
            {
                controlPoints.remove(i)
            }
        }

        transferFunctionChanged()
    }

    function transferFunction (samples) {
        var tf = []
        var prevcp = 0
        var nextcp = 1

        for(var i=0; i<samples; ++i) {
            var sample = i/(samples-1)
            while(controlPoints.get(nextcp).pos < sample) {
                prevcp++
                nextcp++
            }
            var p = controlPoints.get(prevcp);
            var n = controlPoints.get(nextcp);
            var ratio = (sample - n.pos)/(p.pos - n.pos)

            var pc = Qt.darker(p.color, 1.0) // converts from string to actual color type (without making color darker)
            var nc = Qt.darker(n.color, 1.0)
            var c = Qt.rgba(0, 0, 0, 0);
            // interpolate between nearest control points
            c.r = ratio*pc.r + (1.0 - ratio)*nc.r
            c.g = ratio*pc.g + (1.0 - ratio)*nc.g
            c.b = ratio*pc.b + (1.0 - ratio)*nc.b
            c.a = ratio*p.alpha + (1.0 - ratio)*n.alpha
            c.a = c.a*c.a
            tf[i] = c
        }
        return tf
    }

    Rectangle {
        color: "#AAAAAAAA"
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onDoubleClicked: {
                addControlPoint(mouse.x/parent.width, 1.0 - mouse.y/parent.height, "white")
                mouse.accepted = true
            }
        }
    }

    Canvas {
        // draws lines connecting the points
        id: canvas
        anchors.fill: parent
        antialiasing: true
        onPaint: {
            var ctx = canvas.getContext('2d')
            var model = controlPoints

            ctx.save()
            ctx.clearRect(0, 0, canvas.width, canvas.height)
            ctx.strokeStyle = "black"

            ctx.beginPath()
            for(var i=0; i<model.count; ++i) {
                var point = model.get(i)
                if(i==0)
                    ctx.moveTo(point.pos*canvas.width, canvas.height - point.alpha*canvas.height)
                else
                    ctx.lineTo(point.pos*canvas.width, canvas.height - point.alpha*canvas.height)
            }
            ctx.stroke();

            ctx.restore();
        }

        Connections {
            target: controlPoints

            onDataChanged: {
                canvas.requestPaint()
            }
            onRowsInserted: {
                canvas.requestPaint()
            }
            onRowsRemoved: {
                canvas.requestPaint()
            }
        }
    }

    Repeater {
        // creates a dragable point for each control point in the model
        model: controlPoints
        delegate:
            Component {
                id: controlPoint
                Rectangle {
                    color: model.color
                    x: model.pos*parent.width-width/2
                    y: parent.height - model.alpha*parent.height-height/2
                    width: 10
                    height: 10
                    radius: width*0.5

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.SizeAllCursor
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        property bool moved: false

                        onPressed: {
                            if(mouse.button === Qt.LeftButton) {
                                moved = false
                            }
                            else if (mouse.button === Qt.RightButton) {
                                removeControlPoint(model.pos)
                            }
                        }

                        onClicked: {
                            if(!moved && mouse.button === Qt.LeftButton) {
                                var component = Qt.createComponent("PopupMenu.qml")
                                var popup = component.createObject(null)
                                popup.content.sourceComponent = colorPicker
                                popup.parentItem = this
                                popup.origin = Qt.point(x+width/2, y+width/2)
                                popup.show()

                                popup.finished.connect(function(result) {
                                    model.color = result
                                });
                            }
                        }

                        onPositionChanged: {
                            // drag the point by changing the model
                            var pos = ((parent.x + mouse.x))/parent.parent.width
                            var alpha = (parent.parent.height - (parent.y + mouse.y))/parent.parent.height

                            // restrict the pos by the left and right neighbor
                            var posMin = 0.0
                            var posMax = 1.0
                            if(index > 0)
                                posMin = controlPoints.get(index-1).pos
                            if(index < controlPoints.count-1)
                                posMax = controlPoints.get(index+1).pos

                            pos = Math.min(Math.max(posMin, pos), posMax)
                            alpha = Math.min(Math.max(0.0, alpha), 1.0)

                            // first and last index must be fixed at the border
                            if(index > 0 && index < controlPoints.count-1)
                                controlPoints.setProperty(index, "pos", pos)
                            controlPoints.setProperty(index, "alpha", alpha)

                            moved = true // ignore click event after moving
                        }

                        Component {
                            id: colorPicker
							Flow {
                                id: colorPickerFlow
                                signal finished(var result)

								anchors.fill: parent
								anchors.margins: 10
								spacing: 10

								Repeater {
									model: ["red", "green", "blue", "black", "yellow", "pink", "purple", "gray"]
                                    delegate: Rectangle {
                                        color: modelData
                                        width: 50
                                        height: 50

                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                colorPickerFlow.finished(modelData)
                                            }
                                        }
									}
								}
							}
                        }
                    }
                }
            }
        anchors.fill: parent
    }
}

