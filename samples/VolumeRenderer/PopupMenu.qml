import QtQuick 2.0
import QtQuick.Window 2.1
import PopOver 1.0

PopOver {
	id: popup

	width: 300
	height: 300

	// emitted with the selected result
	signal finished(var result)

	onFinished: {
		close()
	}

	property alias margin: canvas.margin
	property alias content: content

	Canvas {
		id: canvas
		anchors.fill: parent
		antialiasing: true

		property int margin: 10
		onPaint: {
			// draw border with small arrow on the button pointing to originX
			var ctx = canvas.getContext('2d')
			ctx.fillStyle = "white"
			ctx.strokeStyle = "black"

			ctx.beginPath()
			ctx.moveTo(margin, height-2*margin)
			ctx.lineTo(width/2-5, height-2*margin)
			ctx.lineTo(width/2, height)
			ctx.lineTo(width/2+5, height-2*margin)
			ctx.lineTo(width-2*margin, height-2*margin)
			ctx.lineTo(width-2*margin, margin)
			ctx.lineTo(margin, margin)
			ctx.closePath()

			ctx.fill()
			ctx.stroke()
		}

		Item {
			// actual content of the popup
			anchors.fill: parent
			anchors.margins: parent.margin
			Loader {
				id: content
				anchors.fill: parent
			}

			Connections {
				// forward finished signal from content
				target: content.item
				onFinished: popup.finished(result)
			}
		}
	}
}
