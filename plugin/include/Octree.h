#ifndef OCTREE_H
#define OCTREE_H

#include "Entity.h"
#include "RendererProperty.h"

#include <memory>

namespace collage
{
	class Image;
	class BoundingBox;

	/**
	 * @brief The Octree class structures a volume into a hierachy of boxes.
	 * Each level of the hierachy splits the volume into up to 8 smaller boxes,
	 * and contains the minimum and maximum value of the volume inside its box.
	 * When rendering, only boxes on the lowest level that overlap with the
	 * target min/max range are rendered.
	 * The box coordinates are stored in world coordinates.
	 */
	class Octree : public Entity
	{
		Q_OBJECT
		Q_PROPERTY(collage::Image* volume READ volume WRITE setVolume NOTIFY volumeChanged)
		/**
		 * @brief Maximum number of octree levels.
		 * A value of -1 means unlimited number of levels.
		 * A value of 0 means only one top level.
		 * Default is -1.
		 */
		Q_PROPERTY(int maxDepth READ maxDepth WRITE setMaxDepth NOTIFY maxDepthChanged)

	public:
		Octree(QObject* parent=0);
		virtual ~Octree();

		void synchronize() override;
		void render(GlProgram& program) override;

		Image* volume() const;

		int maxDepth() const;

	public slots:
		void setVolume(Image* volume);
		void setMaxDepth(int maxDepth);

		void compute();

	signals:
		void volumeChanged(Image* volume);
		void maxDepthChanged(int maxDepth);

		void completed();

	private:
		Image* _volume;
		int _maxDepth;

		struct Node;
		typedef std::unique_ptr<Node> NodePtr;
		NodePtr _root;
	};
}

#endif // OCTREE_H
