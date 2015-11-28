#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "easylogging++.h"

#include "VisualizationFramebuffer.h"
#include "RenderPass.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "TurnTableCamera.h"
#include "Entity.h"
#include "Rectangle.h"
#include "Cube.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<VisualizationFramebuffer>("visualizationframebuffer", 1, 0, "VisualizationFramebuffer");
    qmlRegisterType<RenderPass>("RenderPass", 1, 0, "RenderPass");
    qmlRegisterType<Texture2D>("Texture2D", 1, 0, "Texture2D");
    qmlRegisterType<Texture3D>("Texture3D", 1, 0, "Texture3D");
    qmlRegisterType<Camera>("Camera", 1, 0, "Camera");
    qmlRegisterType<Framebuffer>("Framebuffer", 1, 0, "Framebuffer");
    qmlRegisterType<TurnTableCamera>("TurnTableCamera", 1, 0, "TurnTableCamera");
    qmlRegisterUncreatableType<Entity>("Entity", 1, 0, "Entity", "Entity is abstract");
    qmlRegisterType<Rectangle>("Rectangle", 1, 0, "Rectangle");
    qmlRegisterType<Cube>("Cube", 1, 0, "Cube");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

