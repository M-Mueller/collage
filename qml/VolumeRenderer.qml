import QtQuick 2.5 as QtQuick

import visualizationframebuffer 1.0
import RenderPass 1.0
import UniformInt 1.0
import UniformFloat 1.0
import UniformVec3 1.0
import UniformSampler2D 1.0
import UniformSampler3D 1.0
import Texture 1.0
import Texture2D 1.0
import Texture3D 1.0
import Camera 1.0
import Framebuffer 1.0
import RenderBuffer 1.0
import TurnTableCamera 1.0
import Rectangle 1.0
import NearClippingRectangle 1.0
import Cube 1.0

VisualizationFramebuffer {
    id: vis
    anchors.fill: parent
    transform: QtQuick.Scale { origin.x: width/2; origin.y: height/2; yScale: -1}

    function reloadShaders() {
        frontFacePass.reloadShaders()
        raycastingPass.reloadShaders()
    }

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
            camera.radius -= wheel.angleDelta.y/200
            wheel.accepted = true;
            vis.update()
        }
    }

    TurnTableCamera {
        id: camera
    }

    Texture3D {
        id: volume
        source: "/home/markus/Data/Bucky.mhd"
    }

    RenderPass {
        id: frontFacePass
        vertexShaderPath: "/home/markus/Projects/vis/glsl/Cube.vs"
        fragmentShaderPath: "/home/markus/Projects/vis/glsl/Cube.fs"

        camera: camera
        viewport: Qt.rect(0, 0, vis.width, vis.height)

        depthTest: true

        clearColorBuffer: false
        clearDepthBuffer: true

        renderToTexture: Framebuffer {
            colorAttachment0: Texture2D {
                id: proxyFrontFace
                type: Texture2D.Float
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

        NearClippingRectangle {
            camera: camera
        }

        Cube {
            cullMode: Cube.Back
        }
    }

    RenderPass {
        id: raycastingPass
        vertexShaderPath: "/home/markus/Projects/vis/glsl/RayCasting.vs"
        fragmentShaderPath: "/home/markus/Projects/vis/glsl/RayCasting.fs"

        camera: camera
        viewport: Qt.rect(0, 0, vis.width, vis.height)

        depthTest: true

        clearColorBuffer: false
        clearDepthBuffer: true

        uniforms: [
            UniformSampler2D {
                name: "frontface"
                value: proxyFrontFace
                unit: 1
            },
            UniformSampler3D {
                name: "volume"
                value: volume
                unit: 2
            },
            UniformInt {
                name: "width"
                value: vis.width
            },
            UniformInt {
                name: "height"
                value: vis.height
            },
            UniformFloat {
                name: "iso"
                value: 0.5
            },
            UniformFloat {
                name: "step"
                value: 0.001
            }
        ]

        Cube {
            cullMode: Cube.Front
        }
    }
}
