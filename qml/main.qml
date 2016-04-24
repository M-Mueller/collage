import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

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

    VolumeRenderer {
        id: volumeRenderer
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: gridLayout1.right
        anchors.leftMargin: 0
    }

    TransferFunctionEditor {
        id: tfEditor
        y: 268
        height: 168
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: gridLayout1.right
        anchors.leftMargin: 0

        onTransferFunctionChanged: {
            var tf = tfEditor.transferFunction(volumeRenderer.transferFunction.width)
            volumeRenderer.transferFunction.load(tf)
            volumeRenderer.update()
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
            maximumValue: 3
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

