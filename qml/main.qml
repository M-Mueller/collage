import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

import visualizationframebuffer 1.0
import RenderPass 1.0
import Camera 1.0
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

        RenderPass {
            vertexShaderPath: "/home/markus/Projects/vis/glsl/Cube.vs"
            fragmentShaderPath: "/home/markus/Projects/vis/glsl/Cube.fs"

            camera: Camera {
                viewMatrix: Qt.matrix4x4(1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1)
                projectionMatrix: Qt.matrix4x4(1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1)
            }

            Cube {

            }
        }
    }
}

