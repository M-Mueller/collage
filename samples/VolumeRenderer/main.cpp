#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>

#include "PopOver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<PopOver>("PopOver", 1, 0, "PopOver");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    for (auto obj: engine.rootObjects())
    {
        if (auto win = dynamic_cast<QWindow*>(obj))
        {
            QSurfaceFormat fmt = win->format();
            fmt.setVersion(3, 3);
            fmt.setProfile(QSurfaceFormat::CoreProfile);

            win->setFormat(fmt);
        }
    }

    return app.exec();
}

