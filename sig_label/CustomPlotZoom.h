#pragma once
#ifndef CUSTOMPLOTZOOM_H
#define CUSTOMPLOTZOOM_H

#include <QPoint>
#include "qcustomplot.h"

class QRubberBand;
class QMouseEvent;
class QWidget;

class CustomPlotZoom : public QCustomPlot
{
	Q_OBJECT

public:
	CustomPlotZoom(QWidget * parent = 0);
	virtual ~CustomPlotZoom() {};

	void setZoomMode(bool mode);


	QPoint P;

protected:
	void mousePressEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	//void paintEvent(QPaintEvent *event);

private:
	bool mZoomMode;
	QRubberBand * mRubberBand;
	QPoint mOrigin;
};


#endif // CUSTOMPLOTZOOM_H