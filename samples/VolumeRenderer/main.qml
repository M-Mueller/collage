import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import TransferFunctionEditor 1.0

ApplicationWindow {
	id: applicationWindow1
	visible: true
	width: 800
	height: 600
	color: "#343434"
	title: qsTr("Hello World")

	menuBar: MenuBar {
		Menu {
			title: qsTr("File")
			MenuItem {
				text: qsTr("&Open")
				onTriggered: console.log("Open action triggered");
			}
			MenuItem {
				text: qsTr("Exit")
				onTriggered: Qt.quit();
			}
		}
	}

	Item {
		anchors.margins: 0
		anchors.fill: parent

		VolumeRenderer {
			id: volumeRenderer
			anchors.fill: parent
		}

		Rectangle
		{
			id: options
			anchors.margins: 4
			anchors.topMargin: -optionsHandle.height
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			height: Math.min(200, parent.height)

			border.color: "black"
			border.width: 1

			color: "#AAAAAAAA"
			radius: 5

			state: "expanded"
			states: [
				State {
					name: "expanded"
					AnchorChanges {
						target: options
						anchors.top: undefined
						anchors.bottom: options.parent.bottom
					}
				},
				State {
					name: "collapsed"
					AnchorChanges {
						target: options
						anchors.top: options.parent.bottom
						anchors.bottom: undefined
					}
				}
			]

			transitions: [
				Transition {
					from: "expanded"; to: "collapsed"
					AnchorAnimation {
						duration: 250
						easing.type: Easing.InBack
						easing.overshoot: 4
					}
				},
				Transition {
					from: "collapsed"; to: "expanded"
					AnchorAnimation {
						duration: 250
						easing.type: Easing.OutBack
						easing.overshoot: 4
					}
				}
			]

			RowLayout {
				anchors.fill: parent
				anchors.margins: 8

				GridLayout {
					Layout.maximumWidth: 200
					Layout.fillHeight: true

					columnSpacing: 5
					rowSpacing: 5
					columns: 2

					Button {
						text: qsTr("Reload Shaders")
						Layout.fillWidth: true

						Layout.columnSpan: 2

						onClicked: {
							volumeRenderer.reloadShaders()
							volumeRenderer.update()
						}
					}

					Label {
						text: qsTr("Mode:")
					}

					ComboBox {
						Layout.fillWidth: true

						model: [ "ISO", "MIP", "DVR", "Dbg Entry", "Dbg Exit"]
						currentIndex: volumeRenderer.mode

						onCurrentIndexChanged: volumeRenderer.mode = currentIndex
					}

					Label {
						text: qsTr("Lighting")
					}

					CheckBox {
						Layout.fillWidth: true

						checked: volumeRenderer.lightEnabled

						onCheckedChanged: volumeRenderer.lightEnabled = checked
					}

					Label {
						text: qsTr("Window Width:")
					}

					SpinBox {
						Layout.fillWidth: true

						stepSize: 0.1
						decimals: 2
						minimumValue: 0.0
						maximumValue: 1.0
						value: volumeRenderer.windowWidth

						onValueChanged: volumeRenderer.windowWidth = value
					}

					Label {
						text: qsTr("Window Center:")
					}

					SpinBox {
						Layout.fillWidth: true

						stepSize: 0.1
						decimals: 2
						minimumValue: -1.0
						maximumValue: 1.0
						value: volumeRenderer.windowCenter

						onValueChanged: volumeRenderer.windowCenter = value
					}
				}

				TransferFunctionEditor {
					id: tfEditor

					Layout.fillWidth: true
					Layout.fillHeight: true

					anchors.bottom: parent.bottom
					anchors.top: parent.top

					opacity: 0.6

					onTransferFunctionChanged: {
						volumeRenderer.transferFunction.load(transferFunction)
						volumeRenderer.update()
					}

					Component {
						id: colorPicker
						TransferFunctionColorPicker {
							anchors.fill: parent
						}
					}

					onControlPointClicked: {
						var component = Qt.createComponent("PopupMenu.qml")
						var popup = component.createObject(null)
						popup.content.sourceComponent = colorPicker
						popup.parentItem = this
						popup.origin = origin
						popup.show()

						popup.finished.connect(function(result) {
							if (result === "remove")
								tfEditor.removeControlPoint(controlPoint)
							else
								tfEditor.setControlPointColor(controlPoint, result)
						});
					}
				}
			}

			Rectangle {
				id: optionsHandle
				anchors.verticalCenter: parent.top;
				anchors.horizontalCenter: parent.horizontalCenter
				height: 24
				width: 64

				radius: parent.radius
				border.width: parent.border.width
				border.color: parent.border.color

				color: "#CCCCCC"

				Image {
					anchors.fill: parent
					source: "qrc:/images/ExpanderIcon.png"
					rotation: {
						if (options.state == "expanded") 0; else 180;
					}
				}

				MouseArea {
					anchors.fill: parent
					onClicked: {
						if (options.state == "collapsed")
							options.state = "expanded"
						else
							options.state = "collapsed"
						parent.color = "#CCCCCC" // onExited is not called when MouseArea moves
					}
					hoverEnabled: true
					onEntered: parent.color = "#DDDDDD"
					onExited: parent.color = "#CCCCCC"
				}
			}
		}
	}

}

