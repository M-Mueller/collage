#ifndef TRANSFERFUNCTIONEDITOR_H
#define TRANSFERFUNCTIONEDITOR_H

#include <QtQuick/QQuickPaintedItem>
#include <QtCore/QVector>

class TransferFunctionEditor: public QQuickPaintedItem
{
	Q_OBJECT
	Q_PROPERTY(int numSamples READ numSamples WRITE setNumSamples NOTIFY numSamplesChanged)
	Q_PROPERTY(QList<QVariant> transferFunction READ transferFunction NOTIFY transferFunctionChanged)

public:
	TransferFunctionEditor(QQuickItem* parent=nullptr);

	void paint(QPainter* painter) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

	int numSamples() const;
	QList<QVariant> transferFunction() const;

public slots:
	void setNumSamples(int numSamples);
	void addControlPoint(double pos, double alpha, QColor color=QColor());
	void removeControlPoint(int controlPoint);
	void setControlPointColor(int controlPoint, QColor color);

signals:
	void numSamplesChanged(int numSamples);
	void transferFunctionChanged(const QList<QVariant>& transferFunction);
	void controlPointClicked(int controlPoint, const QPoint& origin);

private:
	void updateTransferFunction();
	int reorderControlPoint(int controlPointIndex);

private:
	struct ControlPoint
	{
		TransferFunctionEditor* parent;
		double pos;
		double alpha;
		QColor color;

		QPointF absPos() const;
		void setAbsPos(QPointF p);
	};
	QVector<ControlPoint> _controlPoints;
	int _numSamples;
	QList<QVariant> _transferFunction;

	int _draggedControlPoint; ///< Index of the control point that was clicked for dragging (-1 if unset)
	bool _dragShape; ///< true if the whole shape should be moved instead of a single controlpoint
	QPoint _dragStart; ///< point where the drag was started
	bool _dragMoved; ///< true once a control point was actively dragged on not just clicked
	QVector<ControlPoint> _dragStartControlPoints; ///< position of all controlpoints before the drag (might be different order than current controlpoints)

	const double _controlPointRadius;
	const int _minDragDistance;
	const int _minNumberOfPoints;
};

#endif // TRANSFERFUNCTIONEDITOR_H
