#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "sig.h"

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = Q_NULLPTR);
	~mainWindow();
	



private:
	Ui::mainWindowClass ui;
	Sig *signal = new Sig();
	QVector<double> freqs;
	QVector<double> aveSpec, maxSpec;
	double minSpecV, maxSpecV;
	QSize truePlotSize;

	QVector<QRect> resultRect;

	QString fileName;
	QRubberBand *wRubberBand;
	QRubberBand *rubberBand;
	QPoint rubberOrigin;

	void plotSpec(QVector<double>x, QVector<double>y, int k, QColor color);

	void setWholePlotRubbreBand();


	void resizeEvent(QResizeEvent *event);




private slots:
	void openFile();
	void mouseWheel();
	void mousePress(QMouseEvent * mevent);

	void mouseMove(QMouseEvent * mevent);

	void mouseRelease(QMouseEvent * mevent);

	void slotBtn();


};
