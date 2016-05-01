#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "easylogging++.h"

#include "VisualizationFramebuffer.h"
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
#include "Image.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VisualizationFramebuffer>("visualizationframebuffer", 1, 0, "VisualizationFramebuffer");
    qmlRegisterType<RenderPass>("RenderPass", 1, 0, "RenderPass");

    qmlRegisterUncreatableType<Texture>("Texture", 1, 0, "Texture", "Texture is abstract");
    qmlRegisterType<Texture1D>("Texture1D", 1, 0, "Texture1D");
    qmlRegisterType<Texture2D>("Texture2D", 1, 0, "Texture2D");
    qmlRegisterType<Texture3D>("Texture3D", 1, 0, "Texture3D");

    qmlRegisterType<Image>("Image", 1, 0, "Image");

    qmlRegisterType<Framebuffer>("Framebuffer", 1, 0, "Framebuffer");
    qmlRegisterType<RenderBuffer>("RenderBuffer", 1, 0, "RenderBuffer");

    qmlRegisterType<Camera>("Camera", 1, 0, "Camera");
    qmlRegisterType<TurnTableCamera>("TurnTableCamera", 1, 0, "TurnTableCamera");

    qmlRegisterUncreatableType<Entity>("Entity", 1, 0, "Entity", "Entity is abstract");
    qmlRegisterType<Rectangle>("Rectangle", 1, 0, "Rectangle");
    qmlRegisterType<Cube>("Cube", 1, 0, "Cube");
    qmlRegisterType<BoundingBox>("BoundingBox", 1, 0, "BoundingBox");
    qmlRegisterType<ClearFramebuffer>("ClearFramebuffer", 1, 0, "ClearFramebuffer");
    qmlRegisterType<Uniforms>("Uniforms", 1, 0, "Uniforms");
    qmlRegisterType<UniformStruct>("UniformStruct", 1, 0, "UniformStruct");
    qmlRegisterType<BindTexture>("BindTexture", 1, 0, "BindTexture");

    qmlRegisterType<PopOver>("PopOver", 1, 0, "PopOver");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

