#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "VisualizationFramebuffer.h"
#include "RenderPass.h"
#include "Entity.h"
#include "Camera.h"
#include "Triangle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<VisualizationFramebuffer>("visualizationframebuffer", 1, 0, "VisualizationFramebuffer");
    qmlRegisterType<RenderPass>("RenderPass", 1, 0, "RenderPass");
    qmlRegisterUncreatableType<Entity>("Entity", 1, 0, "Entity", "Entity is abstract");
    qmlRegisterType<Camera>("Camera", 1, 0, "Camera");
    qmlRegisterType<Triangle>("Triangle", 1, 0, "Triangle");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

