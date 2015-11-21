import QtQuick 2.5 as QtQuick
import QtQuick.Controls 1.4 as QtControls
import QtQuick.Dialogs 1.2 as QtDialog

import visualizationframebuffer 1.0
import RenderPass 1.0
import Texture2D 1.0
import Camera 1.0
import TurnTableCamera 1.0
import Rectangle 1.0
import Cube 1.0

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

    VisualizationFramebuffer {
        id: vis
        anchors.fill: parent
        transform: QtQuick.Scale { origin.x: width/2; origin.y: height/2; yScale: -1}

        QtQuick.MouseArea {
            anchors.fill: parent;
            acceptedButtons: Qt.LeftButton

            property int originX: 0
            property int originY: 0
            onPressed: {
                originX = mouse.x
                originY = mouse.y
            }

            onPositionChanged: {
                camera.phi += (originX - mouse.x)*0.01
                camera.theta -= (originY - mouse.y)*0.01
                originX = mouse.x
                originY = mouse.y
                vis.update()
            }

            onWheel: {
                camera.radius -= wheel.angleDelta.y/100
                wheel.accepted = true;
                vis.update()
            }
        }

        RenderPass {
            vertexShaderPath: "/home/markus/Projects/vis/glsl/Rectangle.vs"
            fragmentShaderPath: "/home/markus/Projects/vis/glsl/Rectangle.fs"

            camera: TurnTableCamera {
                id: camera
            }

            Rectangle {
                texture: Texture2D {
                    id: tex
                    source: ":/Test.png"
                }
            }
        }
    }
}

