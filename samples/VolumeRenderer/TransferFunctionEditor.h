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

signals:
	void numSamplesChanged(int numSamples);
	void transferFunctionChanged(const QList<QVariant>& transferFunction);

private:
	void updateTransferFunction();

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
	const double _controlPointRadius;
	int _numSamples;
	QList<QVariant> _transferFunction;

	int _draggedControlPoint;
	bool _dragShape;
	QPoint _dragStart;
	QVector<ControlPoint> _dragStartControlPoints;
};

#endif // TRANSFERFUNCTIONEDITOR_H
