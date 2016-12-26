import QtQuick 2.5 as QtQuick
import collage 1.0

Canvas {
	id: root

	transform: QtQuick.Scale { origin.x: width/2; origin.y: height/2; yScale: -1} // flip the whole image

	function reloadShaders() {
		rayEntryPointsPass.reloadShaders()
		raycastingPass.reloadShaders()
	}

	property int mode: 2
	property double windowWidth: 1.0
	property double windowCenter: 0.5
	property bool lightEnabled: false

	property alias transferFunction: transferFunction

	onModeChanged: update()
	onWindowWidthChanged: update()
	onWindowCenterChanged: update()
	onLightEnabledChanged: update()

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
			root.update()
		}

		onWheel: {
			camera.radius -= wheel.angleDelta.y/10
			wheel.accepted = true;
			root.update()
		}
	}

	TurnTableCamera {
		id: camera
		aspectRatio: root.width/root.height
		radius: 100

		property matrix4x4 invViewProjMatrix: viewMatrix.inverted().times(projectionMatrix.inverted())

		function center() {
			var invView = camera.viewMatrix.inverted()
			return invView.times(Qt.vector3d(0, 0, 0))
		}
	}

	QtQuick.QtObject {
		id: light
		property vector3d position: Qt.vector3d(50, 50, 50)
		property color color: "#88EEEEFF"
	}

	Texture3D {
		id: volumeTex
		volume: Image {
			id: volume
			source: "/home/markus/Data/Bucky.mhd"
		}
	}

	Texture1D {
		id: transferFunction
		width: 256

		QtQuick.Component.onCompleted: {
			// initialize with ramp
			var tf = []
			for(var i=0; i<transferFunction.width; ++i) {
				var a = i/transferFunction.width;
				tf[i] = Qt.rgba(a, a, a, a)
			}
			transferFunction.load(tf)
		}
	}

	// Determine the entry points of the rays by drawing a cube that has its coordinates as colors
	RenderPass {
		id: rayEntryPointsPass
		vertexShaderPath: "/home/markus/Projects/collage/samples/VolumeRenderer/glsl/RayEntry.vs"
		fragmentShaderPath: "/home/markus/Projects/collage/samples/VolumeRenderer/glsl/RayEntry.fs"

		viewport: Qt.rect(0, 0, root.width, root.height)

		depthTest: true
		cullMode: CullMode.Back

		renderToTexture: Framebuffer {
			colorAttachment0: Texture2D {
				id: rayEntryTex
				type: Texture2D.Float
				width: root.width
				height: root.height
				channels: 4
			}
			depthAttachment: RenderBuffer {
				type: Texture.Depth24
				width: root.width
				height: root.height
			}
		}

		ClearFramebuffer {
			clearColorBuffer: true
			clearDepthBuffer: true
		}

		// first render the near clipping plane as fallback position if the camera is inside the cube
		Uniforms {
			property matrix4x4 viewMatrix: Qt.matrix4x4()
			property matrix4x4 projectionMatrix: Qt.matrix4x4()
			// encode the position in the color attribute
			property bool usePositionAsColor: false
		}

		Rectangle {
			// convert from clip to world coordinates
			color0: camera.invViewProjMatrix.times(Qt.vector3d(-1, -1, -1))
			color1: camera.invViewProjMatrix.times(Qt.vector3d(1, -1, -1))
			color2: camera.invViewProjMatrix.times(Qt.vector3d(1, 1, -1))
			color3: camera.invViewProjMatrix.times(Qt.vector3d(-1, 1, -1))
		}

		ClearFramebuffer {
			clearColorBuffer: false
			clearDepthBuffer: true
		}

		// render the frontfaces of the cube
		Uniforms {
			property matrix4x4 viewMatrix: camera.viewMatrix
			property matrix4x4 projectionMatrix: camera.projectionMatrix
			property bool usePositionAsColor: true
		}

		Octree {
			volume: volume
		}
	}

	// Determine the entry points of the rays by drawing a cube that has its coordinates as colors
	RenderPass {
		id: rayExitPointsPass
		vertexShaderPath: "/home/markus/Projects/collage/samples/VolumeRenderer/glsl/RayEntry.vs"
		fragmentShaderPath: "/home/markus/Projects/collage/samples/VolumeRenderer/glsl/RayEntry.fs"

		viewport: Qt.rect(0, 0, root.width, root.height)

		depthTest: true
		depthFunc: DepthFunc.Greater
		cullMode: CullMode.Front

		renderToTexture: Framebuffer {
			colorAttachment0: Texture2D {
				id: rayExitTex
				type: Texture2D.Float
				width: root.width
				height: root.height
				channels: 4
			}
			depthAttachment: RenderBuffer {
				type: Texture.Depth24
				width: root.width
				height: root.height
			}
		}

		ClearFramebuffer {
			clearColorBuffer: true
			clearDepthBuffer: true
			clearDepth: 0.0
		}

		// render the frontfaces of the cube
		Uniforms {
			property matrix4x4 viewMatrix: camera.viewMatrix
			property matrix4x4 projectionMatrix: camera.projectionMatrix
			property bool usePositionAsColor: true
		}

		Octree {
			volume: volume
		}
	}

	RenderPass {
		id: raycastingPass
		vertexShaderPath: "/home/markus/Projects/collage/samples/VolumeRenderer/glsl/RayCasting.vs"
		fragmentShaderPath: "/home/markus/Projects/collage/samples/VolumeRenderer/glsl/RayCasting.fs"

		viewport: Qt.rect(0, 0, root.width, root.height)
		depthTest: false

		BindTexture {
			texture: rayEntryTex
			unit: 1
		}

		BindTexture {
			texture: rayExitTex
			unit: 2
		}

		BindTexture {
			texture: volumeTex
			unit: 3
		}

		BindTexture {
			texture: transferFunction
			unit: 4
		}

		Uniforms {
			id: raycastingUniforms

			property int rayEntryTex: 1
			property int rayExitTex: 2

			property int volume: 3
			property vector3d volumeSize: Qt.vector3d(volume.width, volume.height, volume.depth)
			property vector3d volumeSpacing: volume.spacing

			property double step: 0.5*Math.max(volume.spacing.x, Math.max(volume.spacing.y, volume.spacing.z))

			property double windowWidth: root.windowWidth
			property double windowCenter: root.windowCenter

			property int mode: root.mode
			property double iso: 0.5
			property int transferFunction: 4

			property UniformStruct light: UniformStruct {
				property vector3d position: light.position
				property color color: light.color
			}
			property vector3d viewer: camera.center()
			property int lightEnabled: root.lightEnabled
		}

		ClearFramebuffer {
			clearColorBuffer: false // already cleared by qt
			clearDepthBuffer: true
		}

		// render fullscreen rectangle
		Rectangle {
		}
	}
}
