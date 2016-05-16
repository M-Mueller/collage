#include "CollagePlugin.h"
#include "Canvas.h"
#include "RenderPass.h"
#include "Texture.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "RenderBuffer.h"
#include "TurnTableCamera.h"
#include "Entity.h"
#include "Rectangle.h"
#include "Cube.h"
#include "ClearFramebuffer.h"
#include "Uniforms.h"
#include "BindTexture.h"
#include "BoundingBox.h"
#include "Octree.h"
#include "Image.h"

namespace collage
{
    void CollagePlugin::registerTypes(const char* uri)
    {
        qmlRegisterType<collage::Canvas>(uri, 1, 0, "Canvas");
        qmlRegisterType<collage::RenderPass>(uri, 1, 0, "RenderPass");
        qmlRegisterUncreatableType<collage::DepthFunc>(uri, 1, 0, "DepthFunc", "DepthFunc is an enum");
        qmlRegisterUncreatableType<collage::CullMode>(uri, 1, 0, "CullMode", "CullMode is an enum");

        qmlRegisterUncreatableType<collage::Texture>(uri, 1, 0, "Texture", "Texture is abstract");
        qmlRegisterType<collage::Texture1D>(uri, 1, 0, "Texture1D");
        qmlRegisterType<collage::Texture2D>(uri, 1, 0, "Texture2D");
        qmlRegisterType<collage::Texture3D>(uri, 1, 0, "Texture3D");

        qmlRegisterType<collage::Image>(uri, 1, 0, "Image");

        qmlRegisterType<collage::Framebuffer>(uri, 1, 0, "Framebuffer");
        qmlRegisterType<collage::RenderBuffer>(uri, 1, 0, "RenderBuffer");

        qmlRegisterType<collage::Camera>(uri, 1, 0, "Camera");
        qmlRegisterType<collage::TurnTableCamera>(uri, 1, 0, "TurnTableCamera");

        qmlRegisterUncreatableType<collage::Entity>(uri, 1, 0, "Entity", "Entity is abstract");
        qmlRegisterType<collage::Rectangle>(uri, 1, 0, "Rectangle");
        qmlRegisterType<collage::Cube>(uri, 1, 0, "Cube");
        qmlRegisterType<collage::BoundingBox>(uri, 1, 0, "BoundingBox");
        qmlRegisterType<collage::Octree>(uri, 1, 0, "Octree");
        qmlRegisterType<collage::ClearFramebuffer>(uri, 1, 0, "ClearFramebuffer");
        qmlRegisterType<collage::Uniforms>(uri, 1, 0, "Uniforms");
        qmlRegisterType<collage::UniformStruct>(uri, 1, 0, "UniformStruct");
        qmlRegisterType<collage::BindTexture>(uri, 1, 0, "BindTexture");
    }
}
