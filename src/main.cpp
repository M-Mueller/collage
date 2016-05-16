#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "easylogging++.h"

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
#include "PopOver.h"
#include "BoundingBox.h"
#include "Octree.h"
#include "Image.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    using namespace collage;
    QGuiApplication app(argc, argv);

    qmlRegisterType<collage::Canvas>("Canvas", 1, 0, "Canvas");
    qmlRegisterType<collage::RenderPass>("RenderPass", 1, 0, "RenderPass");
    qmlRegisterUncreatableType<collage::DepthFunc>("DepthFunc", 1, 0, "DepthFunc", "DepthFunc is an enum");
    qmlRegisterUncreatableType<collage::CullMode>("CullMode", 1, 0, "CullMode", "CullMode is an enum");

    qmlRegisterUncreatableType<collage::Texture>("Texture", 1, 0, "Texture", "Texture is abstract");
    qmlRegisterType<collage::Texture1D>("Texture1D", 1, 0, "Texture1D");
    qmlRegisterType<collage::Texture2D>("Texture2D", 1, 0, "Texture2D");
    qmlRegisterType<collage::Texture3D>("Texture3D", 1, 0, "Texture3D");

    qmlRegisterType<collage::Image>("Image", 1, 0, "Image");

    qmlRegisterType<collage::Framebuffer>("Framebuffer", 1, 0, "Framebuffer");
    qmlRegisterType<collage::RenderBuffer>("RenderBuffer", 1, 0, "RenderBuffer");

    qmlRegisterType<collage::Camera>("Camera", 1, 0, "Camera");
    qmlRegisterType<collage::TurnTableCamera>("TurnTableCamera", 1, 0, "TurnTableCamera");

    qmlRegisterUncreatableType<collage::Entity>("Entity", 1, 0, "Entity", "Entity is abstract");
    qmlRegisterType<collage::Rectangle>("Rectangle", 1, 0, "Rectangle");
    qmlRegisterType<collage::Cube>("Cube", 1, 0, "Cube");
    qmlRegisterType<collage::BoundingBox>("BoundingBox", 1, 0, "BoundingBox");
    qmlRegisterType<collage::Octree>("Octree", 1, 0, "Octree");
    qmlRegisterType<collage::ClearFramebuffer>("ClearFramebuffer", 1, 0, "ClearFramebuffer");
    qmlRegisterType<collage::Uniforms>("Uniforms", 1, 0, "Uniforms");
    qmlRegisterType<collage::UniformStruct>("UniformStruct", 1, 0, "UniformStruct");
    qmlRegisterType<collage::BindTexture>("BindTexture", 1, 0, "BindTexture");

    qmlRegisterType<PopOver>("PopOver", 1, 0, "PopOver");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

