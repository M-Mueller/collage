import QtQuick 2.0
import QtQuick.Controls 1.4

Item {
	id: colorPicker
	signal finished(var result)

	width: 4*50 + 2*10 + (4-1)*10 // show 4 items per row (including spacing)

	Button {
		id: colorPickerRemove

		anchors.margins: 10
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		text: qsTr("Remove")

		onClicked: {
			colorPicker.finished("remove")
		}
	}

	Flow {
		id: colorPickerFlow

		property int controlPointIndex: 0

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.top: colorPickerRemove.bottom
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
						colorPicker.finished(modelData)
					}
				}
			}
		}
	}
}
