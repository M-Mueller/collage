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
        anchors.left: column1.right
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
        anchors.left: column1.right
        anchors.leftMargin: 0

        onTransferFunctionChanged: {
            var tf = tfEditor.transferFunction(volumeRenderer.transferFunction.width)
            volumeRenderer.transferFunction.load(tf)
            volumeRenderer.update()
        }
    }

    Column {
        id: column1
        width: 200
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        spacing: 9

        Button {
            id: reloadShaders
            text: qsTr("Reload Shaders")

            onClicked: {
                volumeRenderer.reloadShaders()
                volumeRenderer.update()
            }
        }

        SpinBox {
            minimumValue: 0
            maximumValue: 2
            value: volumeRenderer.mode

            onValueChanged: {
                tfEditor.visible = value == 2
                volumeRenderer.mode = value
                volumeRenderer.update()
            }
        }

    }

}

