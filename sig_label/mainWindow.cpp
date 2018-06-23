#include "mainWindow.h"
#include "sig.h"
#include <QFileDialog>
#include <iostream>
#include <algorithm>
#include "CustomPlotZoom.h"
#include <cmath>

#ifdef _DEBUG
#define PRINT   printf
#else
#define PRINT(...)
#endif

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	std::cout << "start debug" << std::endl;

	ui.setupUi(this);
	wRubberBand = new QRubberBand(QRubberBand::Rectangle, ui.wholePlot);
	rubberBand = new QRubberBand(QRubberBand::Rectangle, ui.truePlot);

	setWindowState(Qt::WindowMaximized);

	truePlotSize = ui.truePlot->size();

	ui.fileLabel->clear();
	ui.truePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
									QCP::iSelectLegend | QCP::iSelectPlottables);


	connect(ui.fileSelectBtn, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(ui.truePlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
	connect(ui.truePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui.truePlot->xAxis, SLOT(setRange(QCPRange)));
	connect(ui.truePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui.truePlot->yAxis, SLOT(setRange(QCPRange)));


	//连接鼠标事件发出的信号，实现绑定
	connect(ui.truePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
	connect(ui.truePlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
	connect(ui.truePlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));
}
mainWindow::~mainWindow()
{
	delete signal;
	delete rubberBand;
}

void mainWindow::plotSpec(QVector<double>x, QVector<double>y, int k, QColor color)
{
	auto minmax = std::minmax_element(y.begin(), y.end());
	int min_y = y[minmax.first - y.begin()];
	int max_y = y[minmax.second - y.begin()];

	QPen pen;
	pen.setColor(color);
	if (k == 1) {
		ui.truePlot->addGraph();
		ui.truePlot->graph()->setPen(pen);
		ui.truePlot->graph()->setLineStyle(QCPGraph::lsLine);
		//ui.truePlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
		ui.truePlot->graph()->setData(x, y);

		ui.truePlot->xAxis->setRange(x[0] - 1, x[x.size() - 1] + 1);
		

		ui.truePlot->yAxis->setRange(min_y - int(max_y - min_y), 270);
		//ui.truePlot->yAxis->setRange(y[minmax.first - y.begin()], y[minmax.second - y.begin()]);
		ui.truePlot->replot();
	}
	else if (k == 0){
		pen.setWidth(2);
		ui.wholePlot->addGraph();
		ui.wholePlot->graph()->setPen(pen);
		ui.wholePlot->graph()->setLineStyle(QCPGraph::lsLine);
		ui.wholePlot->graph(0)->setData(x, y);
		ui.wholePlot->xAxis->setRange(x[0], x[x.size() - 1]);
		//auto minmax = std::minmax_element(y.begin(), y.end());
		ui.wholePlot->yAxis->setRange(minSpecV, maxSpecV);
		ui.wholePlot->replot();
	}
	

	//int xp1 = ui.wholePlot->xAxis->coordToPixel(x[0]);
	//int xp2 = ui.wholePlot->xAxis->coordToPixel(x[2000]);
	//int yp1 = ui.wholePlot->yAxis->coordToPixel(min_y);
	//int yp2 = ui.wholePlot->yAxis->coordToPixel(max_y);

	//printf("位置：(%d, %d), (%d, %d)\n", xp1, yp2, xp2, yp1);

	//wRubberBand->setGeometry(QRect(QPoint(xp1, yp2), QPoint(xp2 , yp1)));
	//
	//wRubberBand->show();

}

void mainWindow::setWholePlotRubbreBand()
{
	if (freqs.isEmpty())
		return;

	const QRect zoomRect = ui.truePlot->geometry();
	int xp1, yp1, xp2, yp2;
	zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
	double x1 = ui.truePlot->xAxis->pixelToCoord(xp1);
	double x2 = ui.truePlot->xAxis->pixelToCoord(xp2);

	if (x1 < freqs[0]) x1 = freqs[0];
	if (x2 > freqs.back()) x2 = freqs.back();

	int xxp1 = ui.wholePlot->xAxis->coordToPixel(x1);
	int xxp2 = ui.wholePlot->xAxis->coordToPixel(x2);
	int yyp1 = ui.wholePlot->yAxis->coordToPixel(minSpecV);
	int yyp2 = ui.wholePlot->yAxis->coordToPixel(maxSpecV);

	printf("truePlot位置：(%d, %d), (%d, %d)\n", xxp1, yyp1, xxp2, yyp2);

	wRubberBand->setGeometry(QRect(QPoint(xxp1, yyp2), QPoint(xxp2, yyp1)));

	wRubberBand->show();
	ui.wholePlot->replot();

}

void mainWindow::resizeEvent(QResizeEvent * event)
{
	if (truePlotSize != ui.truePlot->size()) {
		truePlotSize = ui.truePlot->size();
		setWholePlotRubbreBand();

		std::cout << "尺寸改变！！！" << std::endl;
	}
}


/************************信号槽函数***************************/
void mainWindow::openFile() {
	fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开信号文件"), "./data");
	signal->openFile(fileName.toStdString());
	if (!signal->state) {
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"),
			QString::fromLocal8Bit("文件无法打开，请检查文件是否正确！"));
		ui.fileLabel->setText(QString::fromLocal8Bit("文件无法打开，请检查文件是否正确！"));
		return;
	}
	QStringList sTemp = fileName.split("_", QString::SkipEmptyParts);
	double midFreq = (sTemp[sTemp.size() - 2]).toDouble();
	double start = midFreq - 18.;

	double space = 36. / WAVELENS;
	for (int i = 0; i < WAVELENS; ++i) {
		freqs.append(start + i*space);
	}

	ui.fileLabel->setText(fileName);
	
	for (auto tm : signal->curFrameInfo.aveSpectrum) {
		aveSpec.append(tm);
	}
	for (auto tm : signal->curFrameInfo.maxSpectrum) {
		maxSpec.append(tm);
	}

	auto minmax = std::minmax_element(aveSpec.begin(), aveSpec.end());
	int min_aveSpec = aveSpec[minmax.first - aveSpec.begin()];
	int max_aveSpec = aveSpec[minmax.second - aveSpec.begin()];
	minmax = std::minmax_element(maxSpec.begin(), maxSpec.end());
	int min_maxSpec = maxSpec[minmax.first - maxSpec.begin()];
	int max_maxSpec = maxSpec[minmax.second - maxSpec.begin()];

	minSpecV = fmin(min_aveSpec, min_maxSpec);
	maxSpecV = fmax(min_maxSpec, max_maxSpec);

	plotSpec(freqs, aveSpec, 0, Qt::red);
	plotSpec(freqs, maxSpec, 0, Qt::magenta);

	plotSpec(freqs, aveSpec, 1, Qt::blue);
	plotSpec(freqs, maxSpec, 1, Qt::black);

	setWholePlotRubbreBand();





	double alpha = (maxSpecV - minSpecV) / maxSpecV;

	QVector<double> x, y, y1;
	//QVector<QCPBars *> bars, bars1;

	for (auto res : signal->curFrameInfo.result) {
		x.clear();
		y.clear();
		y1.clear();

		//bars.append(new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis));
		//bars1.append(new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis));

		QCPBars *bars1 = new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis);
		QCPBars *bars = new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis);
		bars->moveAbove(bars1);
		x.append(res.dFreqMid);
		y1.append((1 - alpha)*maxSpecV);

		bars1->setWidth(res.dBW);
		bars1->setData(x, y1);
		bars1->setPen(Qt::NoPen);
		bars1->setBrush(Qt::NoBrush);
		y.append(alpha*maxSpecV);

		bars->setWidth(res.dBW);
		bars->setData(x, y);
		bars->setPen(QPen(QColor(255, 0, 0, 100).lighter(130)));
		bars->setBrush(QColor(0, 168, 140, 100));
/*
		bars.back()->moveAbove(bars1.back());
		x.append(res.dFreqMid);
		y1.append((1 - alpha)*maxSpecV);

		bars1.back()->setWidth(res.dBW);
		bars1.back()->setData(x, y1);
		bars1.back()->setPen(Qt::NoPen);
		bars1.back()->setBrush(Qt::NoBrush);
		y.append(alpha*maxSpecV);

		bars.back()->setWidth(res.dBW);
		bars.back()->setData(x, y);
		bars.back()->setPen(QPen(QColor(255, 0, 0, 100).lighter(130)));
		bars.back()->setBrush(QColor(0, 168, 140, 100));
*/

	}
	ui.truePlot->replot();

}

void mainWindow::mouseWheel()
{
	 //if an axis is selected, only allow the direction of that axis to be zoomed
	 //if no axis is selected, both directions may be zoomed
	
	if (ui.truePlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
		ui.truePlot->axisRect()->setRangeZoom(ui.truePlot->xAxis->orientation());
	else if (ui.truePlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
		ui.truePlot->axisRect()->setRangeZoom(ui.truePlot->yAxis->orientation());
	else
		ui.truePlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

	setWholePlotRubbreBand();
}


// 鼠标按下槽函数
void mainWindow::mousePress(QMouseEvent* mevent)
{
	if (mevent->button() == Qt::RightButton)
	{
		rubberOrigin = mevent->pos();
		//ui.statusBar->showMessage("(" + QString(rubberOrigin.x()) + ", " + QString(rubberOrigin.y()) + ")");
		//std::cout << "按下：(" << rubberOrigin.x() << ", " << rubberOrigin.y() << ")" << endl;

		rubberBand->setGeometry(QRect(rubberOrigin, QSize()));
		rubberBand->show();
	}
}
//鼠标移动槽函数
void mainWindow::mouseMove(QMouseEvent *mevent)
{
	if (rubberBand->isVisible())
		rubberBand->setGeometry(QRect(rubberOrigin, mevent->pos()).normalized());
}
//鼠标释放槽函数
void mainWindow::mouseRelease(QMouseEvent *mevent)
{
	Q_UNUSED(mevent);
	if (rubberBand->isVisible())
	{
		const QRect zoomRect = rubberBand->geometry();
		int xp1, yp1, xp2, yp2;
		zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
		printf("(%d, %d), (%d, %d)\n", xp1, yp1, xp2, yp2);
		if (abs(xp1 - xp2) < 5 || abs(yp1 - yp2) < 5)
			return;
		double x1 = ui.truePlot->xAxis->pixelToCoord(xp1);
		double x2 = ui.truePlot->xAxis->pixelToCoord(xp2);
		double y1 = ui.truePlot->yAxis->pixelToCoord(yp1);
		double y2 = ui.truePlot->yAxis->pixelToCoord(yp2);

		ui.truePlot->xAxis->setRange(x1, x2);
		ui.truePlot->yAxis->setRange(y1, y2);

		rubberBand->hide();
		ui.truePlot->replot();
		
	}
	setWholePlotRubbreBand();
}
//槽函数实现1:1恢复功能
void mainWindow::slotBtn()
{
	//ui.menu_2->hideTearOffMenu();
	ui.truePlot->rescaleAxes();
	ui.truePlot->replot();
}
