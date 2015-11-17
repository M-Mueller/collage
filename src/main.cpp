#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "easylogging++.h"

#include "VisualizationFramebuffer.h"
#include "RenderPass.h"
#include "Camera.h"
#include "TurnTableCamera.h"
#include "Entity.h"
#include "Triangle.h"
#include "Cube.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<VisualizationFramebuffer>("visualizationframebuffer", 1, 0, "VisualizationFramebuffer");
    qmlRegisterType<RenderPass>("RenderPass", 1, 0, "RenderPass");
    qmlRegisterType<Camera>("Camera", 1, 0, "Camera");
    qmlRegisterType<TurnTableCamera>("TurnTableCamera", 1, 0, "TurnTableCamera");
    qmlRegisterUncreatableType<Entity>("Entity", 1, 0, "Entity", "Entity is abstract");
    qmlRegisterType<Triangle>("Triangle", 1, 0, "Triangle");
    qmlRegisterType<Cube>("Cube", 1, 0, "Cube");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

