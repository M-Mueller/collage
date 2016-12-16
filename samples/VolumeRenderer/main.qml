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
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.left: gridLayout1.right

		VolumeRenderer {
			id: volumeRenderer
			anchors.fill: parent
		}

		Rectangle
		{
			id: tfDialog
			anchors.margins: 4
			anchors.topMargin: -tfDialogHandle.height
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
						target: tfDialog
						anchors.top: undefined
						anchors.bottom: tfDialog.parent.bottom
					}
				},
				State {
					name: "collapsed"
					AnchorChanges {
						target: tfDialog
						anchors.top: tfDialog.parent.bottom
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

			TransferFunctionEditor {
				id: tfEditor
				anchors.fill: parent
				anchors.margins: 8

				opacity: 0.6

				onTransferFunctionChanged: {
					volumeRenderer.transferFunction.load(transferFunction)
					volumeRenderer.update()
				}
			}

			Rectangle {
				id: tfDialogHandle
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
						if (tfDialog.state == "expanded") 0; else 180;
					}
				}

				MouseArea {
					anchors.fill: parent
					onClicked: {
						if (tfDialog.state == "collapsed")
							tfDialog.state = "expanded"
						else
							tfDialog.state = "collapsed"
						parent.color = "#CCCCCC" // onExited is not called when MouseArea moves
					}
					hoverEnabled: true
					onEntered: parent.color = "#DDDDDD"
					onExited: parent.color = "#CCCCCC"
				}
			}
		}
	}

	GridLayout {
		id: gridLayout1
		width: 194
		columnSpacing: 5
		rowSpacing: 5
		rows: 3
		columns: 2
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 0
		anchors.top: parent.top
		anchors.topMargin: 0

		Button {
			id: reloadShaders
			text: qsTr("Reload Shaders")

			Layout.columnSpan: 2

			onClicked: {
				volumeRenderer.reloadShaders()
				volumeRenderer.update()
			}
		}

		Label {
			color: "#ffffff"
			text: qsTr("Mode:")
		}

		SpinBox {
			minimumValue: 0
			maximumValue: 5
			value: volumeRenderer.mode

			onValueChanged: {
				tfEditor.visible = value >= 2
				volumeRenderer.mode = value
				volumeRenderer.update()
			}
		}

		Label {
			color: "#ffffff"
			text: qsTr("Window Width:")
		}

		SpinBox {
			id: windowWidthSpinBox
			stepSize: 0.1
			decimals: 2
			minimumValue: 0.0
			maximumValue: 1.0
			value: volumeRenderer.windowWidth
			onValueChanged: volumeRenderer.windowWidth = value
		}

		Label {
			color: "#ffffff"
			text: qsTr("Window Center:")
		}

		SpinBox {
			id: windowCenterSpinBox
			stepSize: 0.1
			decimals: 2
			minimumValue: -1.0
			maximumValue: 1.0
			value: volumeRenderer.windowCenter
			onValueChanged: volumeRenderer.windowCenter = value
		}
	}
}

