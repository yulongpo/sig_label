#include "CustomPlotZoom.h"
#include <iostream>

CustomPlotZoom::CustomPlotZoom(QWidget * parent)
	: QCustomPlot(parent)
	, mZoomMode(false)
	, mRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{}

void CustomPlotZoom::setZoomMode(bool mode)
{
	mZoomMode = mode;
}
void CustomPlotZoom::mousePressEvent(QMouseEvent * event)
{
	if (mZoomMode)
	{
		if (event->button() == Qt::RightButton)
		{
			mOrigin = event->pos();
			std::cout << "press: (" << mOrigin.x() << ", " << mOrigin.y() << ")" << std::endl;
			mRubberBand->setGeometry(QRect(mOrigin, QSize()));
			mRubberBand->show();

			//P = event->pos();

		}
	}
	if (event->button() == Qt::LeftButton) {
		P = event->pos();
		std::cout << "press: (" << P.x() << ", " << P.y() << ")" << std::endl;
	}
	QCustomPlot::mousePressEvent(event);
}

void CustomPlotZoom::mouseMoveEvent(QMouseEvent * event)
{
	if (mRubberBand->isVisible())
	{
		mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
	}
	QCustomPlot::mouseMoveEvent(event);
}

void CustomPlotZoom::mouseReleaseEvent(QMouseEvent * event)
{
	if (mRubberBand->isVisible())
	{
		const QRect zoomRect = mRubberBand->geometry();
		int xp1, yp1, xp2, yp2;
		zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
		double x1 = xAxis->pixelToCoord(xp1);
		double x2 = xAxis->pixelToCoord(xp2);
		double y1 = yAxis->pixelToCoord(yp1);
		double y2 = yAxis->pixelToCoord(yp2);

		xAxis->setRange(x1, x2);
		yAxis->setRange(y1, y2);

		mRubberBand->hide();
		replot();
	}
	QCustomPlot::mouseReleaseEvent(event);
}

//void CustomPlotZoom::paintEvent(QPaintEvent * event)
//{
//	std::cout << "paint: (" << P.x() << ", " << P.y() << ")" << std::endl;
//
//	if (P.isNull())
//		return;
//	QCPPainter p;
//	QPen pen(Qt::red, 8);
//	p.begin(this);
//	p.setPen(pen);
//	p.drawPoint(P);
//	p.end();
//
//	QCustomPlot::paintEvent(event);
//}
