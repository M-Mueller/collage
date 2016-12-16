#include "TransferFunctionEditor.h"
#include <QtGui/QPainter>
#include <QtGui/QVector3D>
#include <QtCore/QDebug>
#include <cmath>
#include <cassert>

QPointF TransferFunctionEditor::ControlPoint::absPos() const
{
	return QPointF(
		pos*(parent->width() - 1),
		(1.0 - alpha)*(parent->height() - 1)
	);
}

void TransferFunctionEditor::ControlPoint::setAbsPos(QPointF p)
{
	pos = p.x()/(parent->width() - 1);
	alpha = (1.0 - p.y()/(parent->height() - 1));
	pos = std::min(1.0, std::max(0.0, pos));
	alpha = std::min(1.0, std::max(0.0, alpha));
}

TransferFunctionEditor::TransferFunctionEditor(QQuickItem* parent):
	QQuickPaintedItem(parent),
	_controlPointRadius(5),
	_numSamples(256),
	_draggedControlPoint(-1)
{
	connect(this, &TransferFunctionEditor::transferFunctionChanged, this, [this]()
	{
		_transferFunction.clear();
	});

	setAcceptedMouseButtons(Qt::LeftButton);

	ControlPoint p1 = {
		this, 0.0, 0.0, QColor(Qt::black)
	};
	ControlPoint p2 = {
		this, 1.0, 1.0, QColor(Qt::white)
	};
	_controlPoints.append(p1);
	_controlPoints.append(p2);
}

void TransferFunctionEditor::paint(QPainter* painter)
{
	if (!painter)
		return;

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

	// draw polygon defined by controlpoints
	QPolygonF polygon;
	QGradientStops gradientStops;
	for (const auto& cp: _controlPoints)
	{
		polygon.append(cp.absPos());
		gradientStops.append({cp.pos, cp.color});
	}

	// add connection to baseline
	polygon.prepend(QPointF(_controlPoints.first().absPos().x(), height() - 1));
	polygon.append(QPointF(_controlPoints.last().absPos().x(), height() - 1));

	// linear gradient from left to right with controlpoints as stops
	QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(1.0, 0.0));
	gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
	gradient.setStops(gradientStops);

	painter->setBrush(QBrush(gradient));
	painter->drawPolygon(polygon);

	// draw individual control points as circles
	for (const auto& cp: _controlPoints)
	{
		painter->setBrush(QBrush(cp.color));
		painter->drawEllipse(cp.absPos(), _controlPointRadius, _controlPointRadius);
	}

	painter->restore();
}

void TransferFunctionEditor::mousePressEvent(QMouseEvent* event)
{
	if (!event)
		return;

	_draggedControlPoint = -1;
	_dragShape = false;
	_dragStart = event->pos();
	_dragStartControlPoints = _controlPoints;

	assert(_controlPoints.size() >= 2);
	if (event->button() == Qt::LeftButton)
	{
		QPolygonF shape;
		shape.append(QPointF(_controlPoints.first().absPos().x(), height()-1));
		for (int i=0; i<_controlPoints.count(); ++i)
		{
			const auto& cp = _controlPoints[i];
			shape.append(cp.absPos());
			// manhattanLength is accurate enough and faster
			if ((cp.absPos() - event->pos()).manhattanLength() <= _controlPointRadius)
			{
				_draggedControlPoint = i;
				event->accept();
				break;
			}
		}
		shape.append(QPointF(_controlPoints.last().absPos().x(), height()-1));

		if (_draggedControlPoint == -1)
		{
			if (shape.containsPoint(event->pos(), Qt::OddEvenFill))
			{
				_dragShape = true;
				event->accept();
			}
		}
	}
}

void TransferFunctionEditor::mouseMoveEvent(QMouseEvent* event)
{
	if (!event)
		return;

	assert(!(_draggedControlPoint != -1 && _dragShape));
	if (_draggedControlPoint != -1)
	{
		auto& cp = _controlPoints[_draggedControlPoint];
		cp.setAbsPos(event->pos());
		event->accept();
		updateTransferFunction();
	}
	else if (_dragShape)
	{
		for (int i=0; i<_controlPoints.count(); ++i)
		{
			const auto& cpStart = _dragStartControlPoints[i];
			auto& cp = _controlPoints[i];

			double offset = event->pos().x() - _dragStart.x();
			QPointF pos = cpStart.absPos();
			pos.rx() += offset;
			cp.setAbsPos(pos);
			event->accept();
			updateTransferFunction();
		}
	}
}

void TransferFunctionEditor::mouseReleaseEvent(QMouseEvent* /*event*/)
{
	_draggedControlPoint = -1;
	_dragShape = false;
}

void TransferFunctionEditor::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (!event)
		return;

	if (event->button() == Qt::LeftButton)
	{
		double pos = event->pos().x() / (width() - 1);
		double alpha = event->pos().y() / (height() - 1);
		alpha = 1.0 - alpha;
		addControlPoint(pos, alpha);
	}
}

QList<QVariant> TransferFunctionEditor::transferFunction() const
{
	return _transferFunction;
}

int TransferFunctionEditor::numSamples() const
{
	return _numSamples;
}

void TransferFunctionEditor::setNumSamples(int numSamples)
{
	if (_numSamples == numSamples)
		return;

	if (numSamples <= 0)
		numSamples = 1;

	_numSamples = numSamples;
	updateTransferFunction();
	emit numSamplesChanged(numSamples);
}

void TransferFunctionEditor::addControlPoint(double pos, double alpha, QColor color)
{
	pos = std::min(1.0, std::max(0.0, pos));
	alpha = std::min(1.0, std::max(0.0, alpha));

	int insertIndex = 0; // new controlpoint will be inserted before this index
	for (; insertIndex<_controlPoints.count(); ++insertIndex)
	{
		const auto& cp = _controlPoints[insertIndex];
		if (pos < cp.pos)
			break;
	}

	if (!color.isValid())
	{
		if (insertIndex == 0)
		{
			color = _controlPoints.first().color;
		}
		else if (insertIndex == _controlPoints.count())
		{
			color = _controlPoints.last().color;
		}
		else
		{
			// interpolate color between existing points
			const auto& prev = _controlPoints[insertIndex-1];
			const auto& next = _controlPoints[insertIndex];

			double ratio = (pos - prev.pos)/(next.pos - prev.pos);
			color.setRedF((1.0 - ratio)*prev.color.redF() + ratio*next.color.redF());
			color.setGreenF((1.0 - ratio)*prev.color.greenF() + ratio*next.color.greenF());
			color.setBlueF((1.0 - ratio)*prev.color.blueF() + ratio*next.color.blueF());
		}
	}

	ControlPoint cp = {
		this, pos, alpha, color
	};
	_controlPoints.insert(insertIndex, cp);
	updateTransferFunction();
}

void TransferFunctionEditor::updateTransferFunction()
{
	_transferFunction = QList<QVariant>::fromVector(QVector<QVariant>(_numSamples, QVariant::fromValue(QColor(0, 0, 0, 0))));

	int prev = 0;
	int next = 1;
	for (int i=0; i<_numSamples; ++i)
	{
		double sample = double(i)/(_numSamples-1);
		if (sample < _controlPoints[prev].pos)
			continue;
		if (sample > _controlPoints[next].pos)
		{
			prev += 1;
			next += 1;
			if (next >= _controlPoints.count())
				break;
		}

		const auto& prevcp = _controlPoints[prev];
		const auto& nextcp = _controlPoints[next];

		double ratio = (sample - prevcp.pos)/(nextcp.pos - prevcp.pos);

		QColor c;
		c.setRedF((1.0 - ratio)*prevcp.color.redF() + ratio*nextcp.color.redF());
		c.setGreenF((1.0 - ratio)*prevcp.color.greenF() + ratio*nextcp.color.greenF());
		c.setBlueF((1.0 - ratio)*prevcp.color.blueF() + ratio*nextcp.color.blueF());
		c.setAlphaF((1.0 - ratio)*prevcp.alpha + ratio*nextcp.alpha);
		c.setAlphaF(c.alphaF()*c.alphaF());

		_transferFunction[i] = QVariant::fromValue(c);
	}

	update();
	emit transferFunctionChanged(_transferFunction);
}
