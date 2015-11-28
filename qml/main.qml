import QtQuick 2.5 as QtQuick
import QtQuick.Controls 1.4 as QtControls
import QtQuick.Dialogs 1.2 as QtDialog

QtControls.ApplicationWindow {
    id: applicationWindow1
    visible: true
    width: 640
    height: 480
    color: "#343434"
    title: qsTr("Hello World")

    menuBar: QtControls.MenuBar {
        QtControls.Menu {
            title: qsTr("File")
            QtControls.MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            QtControls.MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    VolumeRenderer {

    }
}

