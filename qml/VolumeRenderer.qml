import QtQuick 2.5 as QtQuick

import visualizationframebuffer 1.0
import RenderPass 1.0
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
import ClearFramebuffer 1.0
import Uniforms 1.0
import UniformStruct 1.0
import BindTexture 1.0

VisualizationFramebuffer {
    id: vis

    transform: QtQuick.Scale { origin.x: width/2; origin.y: height/2; yScale: -1} // flip the whole image

    function reloadShaders() {
        rayEntryPointsPass.reloadShaders()
        raycastingPass.reloadShaders()
    }

    property alias mode: raycastingUniforms.mode

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
            camera.radius -= wheel.angleDelta.y/10
            wheel.accepted = true;
            vis.update()
        }
    }

    TurnTableCamera {
        id: camera
        aspectRatio: vis.width/vis.height

        function center() {
            var invModelView = camera.viewMatrix.inverted()
            return invModelView.times(Qt.vector3d(0, 0, 0))
        }
    }

    QtQuick.QtObject {
        id: light
        property vector3d position: Qt.vector3d(50, 50, 50)
        property color color: "#88EEEEFF"
    }

    Texture3D {
        id: volume
        source: "/home/markus/Data/Bucky.mhd"
    }

    // Determine the entry points of the rays by drawing a cube that has its coordinates as colors
    RenderPass {
        id: rayEntryPointsPass
        vertexShaderPath: "/home/markus/Projects/vis/glsl/Cube.vs"
        fragmentShaderPath: "/home/markus/Projects/vis/glsl/Cube.fs"

        viewport: Qt.rect(0, 0, vis.width, vis.height)

        depthTest: true

        renderToTexture: Framebuffer {
            colorAttachment0: Texture2D {
                id: rayEntryTex
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

        Uniforms {
            property matrix4x4 viewMatrix: camera.viewMatrix
            property matrix4x4 projectionMatrix: camera.projectionMatrix
        }

        ClearFramebuffer {
            clearColorBuffer: true
            clearDepthBuffer: true
        }

        // first render the nearclipping plane as fallback position if the camera is inside the cube
        NearClippingRectangle {
            camera: camera
            cube: volumeProxy
        }

        // render the frontfaces of the cube
        Cube {
            id: volumeProxy
            cullMode: Cube.Back
            modelMatrix: Qt.matrix4x4(volume.width*volume.spacing.x, 0, 0, 0,
                                      0, volume.height*volume.spacing.y, 0, 0,
                                      0, 0, volume.depth*volume.spacing.z, 0,
                                      0, 0, 0, 1)
        }
    }

    RenderPass {
        id: raycastingPass
        vertexShaderPath: "/home/markus/Projects/vis/glsl/RayCasting.vs"
        fragmentShaderPath: "/home/markus/Projects/vis/glsl/RayCasting.fs"

        viewport: Qt.rect(0, 0, vis.width, vis.height)

        depthTest: true

        BindTexture {
            texture: rayEntryTex
            unit: 1
        }

        BindTexture {
            texture: volume
            unit: 2
        }

        Uniforms {
            id: raycastingUniforms
            property int rayEntryTex: 1
            property int volume: 2
            property double step: 1.0/(2*32.0)
            property int mode: 0
            property double iso: 0.5
            property matrix4x4 viewMatrix: camera.viewMatrix
            property matrix4x4 projectionMatrix: camera.projectionMatrix
            property UniformStruct light: UniformStruct {
                property vector3d position: light.position
                property color color: light.color
            }
            property vector3d viewer: camera.center()
        }

        ClearFramebuffer {
            clearColorBuffer: false // already cleared by qt
            clearDepthBuffer: true
        }

        // render only the backfaces of the cube as exit positions for the rays
        Cube {
            cullMode: Cube.Front
            modelMatrix: volumeProxy.modelMatrix
        }
    }
}
