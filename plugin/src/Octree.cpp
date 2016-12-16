#include "Octree.h"
#include "BoundingBox.h"
#include "Image.h"

#include <glue/GlProgram.h>

namespace collage
{
	struct Octree::Node
	{
		double min, max;
		BoundingBox bb;
		int level;
		std::vector<NodePtr> children;
	};

	Octree::Octree(QObject* parent):
		Entity(parent),
		_volume(nullptr),
		_maxDepth(-1)
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
				if(node->min <= 0.5 && node->max >= 0.5)
				{
					if(node->children.empty())
					{
						node->bb.render(program);
					}
					else
					{
						for(auto& c: node->children)
							render(c);
					}
				}
			};

			render(_root);
		}
	}

	Image* Octree::volume() const
	{
		return _volume;
	}

	int Octree::maxDepth() const
	{
		return _maxDepth;
	}

	void Octree::setVolume(Image* volume)
	{
		if (_volume == volume)
			return;

		_volume = volume;
		emit volumeChanged(volume);

		compute();
	}

	void Octree::setMaxDepth(int maxLevels)
	{
		if (_maxDepth == maxLevels)
			return;

		_maxDepth = maxLevels;
		emit maxDepthChanged(maxLevels);
	}

	void Octree::compute()
	{
		if(!_volume)
			return;

		auto root = std::make_unique<Node>();
		root->bb.setRenderMode(BoundingBox::Solid);
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

			if (_maxDepth != -1 && parent->level >= _maxDepth)
				return;

			// determine in which directions we can still split
			std::vector<int> sx, sy, sz;
			if(childSize.x() < minBBSize.x())
				sx = {0}; // don't split
			else
				sx = {-1, 1}; // split in left/right

			if(childSize.y() < minBBSize.y())
				sy = {0};
			else
				sy = {-1, 1};

			if(childSize.z() < minBBSize.z())
				sz = {0};
			else
				sz = {-1, 1};

			// split current boundingbox into sub-boundingboxes
			for(int x: sx)
			{
				for(int y: sy)
				{
					for(int z: sz)
					{
						auto child = std::make_unique<Node>();
						child->bb.setRenderMode(parent->bb.renderMode());
						child->bb.setSize(childSize);
						child->bb.setCenter(center + QVector3D(x, y, z)*0.5*childSize);
						child->level = parent->level + 1;
						parent->children.push_back(std::move(child));
					}
				}
			}

			// recursively split the new sub-boundingboxes
			for(auto& c: parent->children)
				split(c);
		};
		split(root);

		std::function<void(NodePtr&)> computeMinMax = [&](NodePtr& node)
		{
			if(!node->children.empty())
			{
				// if the node has children, its min/max values are the overall
				// min/max values of its children
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
				// compute the min/max values inside the bounding box
				QVector3D offset = node->bb.center() - 0.5*node->bb.size();
				QVector3D size = node->bb.size();

				int zStart = static_cast<int>(std::floor(offset.z() + centerOffset.z()));
				int zEnd = static_cast<int>(std::ceil(zStart + size.z()));
				int yStart = static_cast<int>(std::floor(offset.y() + centerOffset.y()));
				int yEnd = static_cast<int>(std::ceil(yStart + size.y()));
				int xStart = static_cast<int>(std::floor(offset.x() + centerOffset.x()));
				int xEnd = static_cast<int>(std::ceil(xStart + size.x()));

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

			// convert from pixel to world coordinates after accessing pixel values
			// (otherwise we need to contantly convert coordinates)
			node->bb.setSize(node->bb.size() * _volume->spacing());
			node->bb.setCenter(node->bb.center() * _volume->spacing());
		};
		computeMinMax(root);

		std::swap(root, _root);
	}
}
