#ifndef COLLAGEPLUGIN_H
#define COLLAGEPLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

namespace collage
{
    class CollagePlugin: public QQmlExtensionPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "collage")

    public:
        void registerTypes(const char* uri) override;
    };
}

#endif // COLLAGEPLUGIN_H
