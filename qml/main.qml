import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

import visualizationframebuffer 1.0
import RenderPass 1.0
import Camera 1.0
import TurnTableCamera 1.0
import Triangle 1.0
import Cube 1.0

ApplicationWindow {
    id: applicationWindow1
    visible: true
    width: 640
    height: 480
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

    VisualizationFramebuffer {
        id: vis
        anchors.fill: parent
        transform: Scale { origin.x: width/2; origin.y: height/2; yScale: -1}

        MouseArea {
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
            vertexShaderPath: "/home/markus/Projects/vis/glsl/Cube.vs"
            fragmentShaderPath: "/home/markus/Projects/vis/glsl/Cube.fs"

            camera: TurnTableCamera {
                id: camera
            }

            Cube {

            }
        }
    }
}

