#include "Octree.h"
#include "BoundingBox.h"
#include "Image.h"

#include <glue/GlProgram.h>

#include "easylogging++.h"

struct Octree::Node
{
    double min, max;
    BoundingBox bb;
    int level;
    std::vector<NodePtr> children;
};

Octree::Octree(QObject* parent):
    Entity(parent),
    _volume(nullptr)
{

}

Octree::~Octree()
{

}

void Octree::synchronize()
{
    if(_root)
    {
        std::function<void(NodePtr&)> sync = [&](NodePtr& node)
        {
            node->bb.synchronize();
            for(auto& c: node->children)
                sync(c);
        };

        sync(_root);
    }
}

void Octree::render(GlProgram& program)
{
    QList<QColor> colors = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::magenta,
        Qt::yellow,
        Qt::gray,
        Qt::cyan,
        Qt::darkBlue,
        Qt::darkRed
    };

    if(_root)
    {
        std::function<void(NodePtr&)> render = [&](NodePtr& node)
        {
            //if(node->max > 0)
            {
                for(auto& c: node->children)
                    render(c);
                program.setUniform("color", colors[node->level]);
                node->bb.render(program);
            }
        };

        render(_root);
    }
}

Image* Octree::volume() const
{
    return _volume;
}

void Octree::setVolume(Image* volume)
{
    if (_volume == volume)
        return;

    _volume = volume;
    emit volumeChanged(volume);

    compute();
}

void Octree::compute()
{
    if(!_volume)
        return;

    auto root = std::make_unique<Node>();
    root->level = 0;
    root->bb.setCenter(QVector3D(0, 0, 0));
    root->bb.setSize(_volume->size());

    QVector3D minBBSize = QVector3D(8, 8, 8);
    QVector3D centerOffset = _volume->size()/2;

    std::function<void(NodePtr&)> split = [&](NodePtr& parent)
    {
        QVector3D size = parent->bb.size();
        QVector3D childSize = size/2.0;
        QVector3D center = parent->bb.center();

        // minimum size reached
        if(childSize.x() < minBBSize.x() && childSize.y() < minBBSize.y() && childSize.z() < minBBSize.z())
            return;

        std::vector<int> sx, sy, sz;
        if(childSize.x() < minBBSize.x())
        {
            sx = {0};
            childSize.setX(childSize.x()*2);
        }
        else
        {
            sx = {-1, 1};
        }

        if(childSize.y() < minBBSize.y())
        {
            sy = {0};
            childSize.setY(childSize.y()*2);
        }
        else
        {
            sy = {-1, 1};
        }

        if(childSize.z() < minBBSize.z())
        {
            sz = {0};
            childSize.setZ(childSize.z()*2);
        }
        else
        {
            sz = {-1, 1};
        }

        for(int x: sx)
        {
            for(int y: sy)
            {
                for(int z: sz)
                {
                    QVector3D pixelShift(childSize.x() - std::floor(childSize.x()),
                                         childSize.y() - std::floor(childSize.y()),
                                         childSize.z() - std::floor(childSize.z()));

                    auto child = std::make_unique<Node>();
                    child->bb.setSize(childSize + pixelShift*QVector3D(x, y, z));
                    child->bb.setCenter(center + QVector3D(x, y, z)*0.5*childSize - 0.5*pixelShift);
                    child->level = parent->level + 1;
                    parent->children.push_back(std::move(child));
                }
            }
        }

        for(auto& c: parent->children)
            split(c);
    };
    split(root);

    std::function<void(NodePtr&)> computeMinMax = [&](NodePtr& node)
    {
        if(!node->children.empty())
        {
            for(auto& c: node->children)
                computeMinMax(c);
            node->min = node->children[0]->min;
            node->max = node->children[0]->max;
            for(auto& c: node->children)
            {
                node->min = std::min(node->min, c->min);
                node->max = std::max(node->max, c->max);
            }
        }
        else
        {
            QVector3D offset = node->bb.center() - 0.5*node->bb.size();
            QVector3D size = node->bb.size();

            int zStart = offset.z() + centerOffset.z();
            int zEnd = zStart + size.z();
            int yStart = offset.y() + centerOffset.y();
            int yEnd = yStart + size.y();
            int xStart = offset.x() + centerOffset.x();
            int xEnd = xStart + size.x();

            node->min = _volume->normalizedValue(xStart, yStart, zStart, 0);
            node->max = node->min;

            for(int z=zStart; z<zEnd; ++z)
            {
                for(int y=yStart; y<yEnd; ++y)
                {
                    for(int x=xStart; x<xEnd; ++x)
                    {
                        double value = _volume->normalizedValue(x, y, z, 0);
                        node->min = std::min(node->min, value);
                        node->max = std::max(node->max, value);
                    }
                }
            }
        }

        node->bb.setSize(node->bb.size() * _volume->spacing());
        node->bb.setCenter(node->bb.center() * _volume->spacing());
    };
    computeMinMax(root);

    std::swap(root, _root);
}
