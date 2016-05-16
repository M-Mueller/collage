#ifndef OCTREE_H
#define OCTREE_H

#include "Entity.h"
#include "RendererProperty.h"

#include <memory>

namespace collage
{
    class Image;
    class BoundingBox;

    class Octree : public Entity
    {
        Q_OBJECT
        Q_PROPERTY(collage::Image* volume READ volume WRITE setVolume NOTIFY volumeChanged)

    public:
        Octree(QObject* parent=0);
        virtual ~Octree();

        void synchronize() override;
        void render(GlProgram& program) override;

        Image* volume() const;

    public slots:
        void setVolume(Image* volume);
        void compute();

    signals:
        void volumeChanged(Image* volume);
        void completed();

    private:
        Image* _volume;

        struct Node;
        typedef std::unique_ptr<Node> NodePtr;
        NodePtr _root;
    };
}

#endif // OCTREE_H
