import QtQuick 2.5 as QtQuick

import visualizationframebuffer 1.0
import RenderPass 1.0
import Texture 1.0
import Texture2D 1.0
import Camera 1.0
import Framebuffer 1.0
import RenderBuffer 1.0
import TurnTableCamera 1.0
import Rectangle 1.0
import Cube 1.0

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

    TurnTableCamera {
        id: camera
    }

    RenderPass {
        vertexShaderPath: "/home/markus/Projects/vis/glsl/Cube.vs"
        fragmentShaderPath: "/home/markus/Projects/vis/glsl/Cube.fs"

        camera: camera
        viewport: Qt.rect(0, 0, vis.width, vis.height)

        renderToTexture: Framebuffer {
            colorAttachment0: Texture2D {
                id: proxyBackFaces
                type: Texture2D.Char
                width: vis.width
                height: vis.height
                channels: 4
            }
            depthAttachment: RenderBuffer {
                type: Texture.Depth24
                width: vis.width
                height: vis.height
            }
        }

        Cube {
        }
    }

    RenderPass {
        vertexShaderPath: "/home/markus/Projects/vis/glsl/Rectangle.vs"
        fragmentShaderPath: "/home/markus/Projects/vis/glsl/Rectangle.fs"

        viewport: Qt.rect(0, 0, vis.width, vis.height)

        Rectangle {
            texture: proxyBackFaces
        }
    }
}
