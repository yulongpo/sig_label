#include "mainWindow.h"
#include "sig.h"
#include <QFileDialog>
#include <iostream>
#include <algorithm>
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
	lRubberBand = new QRubberBand(QRubberBand::Rectangle, ui.truePlot);

	setWindowState(Qt::WindowMaximized);

	truePlotSize = ui.truePlot->size();

	ui.fileLabel->clear();
	ui.truePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
									QCP::iSelectLegend | QCP::iSelectPlottables);

	// ui.truePlot->setCursor(Qt::OpenHandCursor); //手型


	connect(ui.fileSelectBtn, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));

	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));

	connect(this, SIGNAL(sendPageNum(unsigned long)), this, SLOT(openPage(unsigned long)));
	connect(ui.firstPageBtn, SIGNAL(clicked()), this, SLOT(firstPage()));
	connect(ui.previPageBtn, SIGNAL(clicked()), this, SLOT(prevPage()));
	connect(ui.nextPageBtn, SIGNAL(clicked()), this, SLOT(nextPage()));
	connect(ui.gotoBtn, SIGNAL(clicked()), this, SLOT(gotoPage()));

	connect(ui.pageNumLineEdit, SIGNAL(returnPressed()), this, SLOT(gotoPage()));


	connect(ui.truePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui.truePlot->xAxis, SLOT(setRange(QCPRange)));
	connect(ui.truePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui.truePlot->yAxis, SLOT(setRange(QCPRange)));


	//连接鼠标事件发出的信号，实现绑定
	connect(ui.truePlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
	connect(ui.truePlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
	connect(ui.truePlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent*)));
	connect(ui.truePlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent*)));

	connect(ui.zoomResetMenu, SIGNAL(aboutToShow()), this, SLOT(slotBtn()));

	connect(ui.labelMenu, SIGNAL(aboutToShow()), this, SLOT(test_drag()));

	connect(ui.deletMenu, SIGNAL(aboutToShow()), this, SLOT(removeSelectedGraph()));


	connect(ui.truePlot, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*, int)));
	connect(ui.truePlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionClosed()));

	// std::cout << barSelected << std::endl;
}

mainWindow::~mainWindow()
{
	delete signal;
	delete rubberBand;
}

void mainWindow::plotSpec(QVector<double>x, QVector<double>y, int k, QColor color, QString name = "")
{
	auto minmax = std::minmax_element(y.begin(), y.end());
	int min_y = y[minmax.first - y.begin()];
	int max_y = y[minmax.second - y.begin()];

	QPen pen;
	pen.setColor(color);
	if (k == 1) {  // 绘制工作区图
		ui.truePlot->addGraph();
		ui.truePlot->graph()->setSelectable(QCP::SelectionType::stNone); //无法选中
		ui.truePlot->graph()->setName(name);
		ui.truePlot->graph()->setPen(pen);
		ui.truePlot->graph()->setLineStyle(QCPGraph::lsLine);
		//ui.truePlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
		ui.truePlot->graph()->setData(x, y);
		ui.truePlot->xAxis->setRange(x[0] - 0.5, x[x.size() - 0.5] + 1);
		ui.truePlot->yAxis->setRange(min_y - int(max_y - min_y), 0);
		//ui.truePlot->yAxis->setRange(y[minmax.first - y.begin()], y[minmax.second - y.begin()]);
		ui.truePlot->replot();
	}
	else if (k == 0){ // 绘制总览定位区
		pen.setWidth(2);
		ui.wholePlot->addGraph();
		ui.wholePlot->graph()->setPen(pen);
		ui.wholePlot->graph()->setLineStyle(QCPGraph::lsLine);
		ui.wholePlot->graph()->setData(x, y);
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

//void mainWindow::setBar(double x1, double y1, double x2, double y2)
//{
//	double mid_x = (x1 + x2) / 2;
//	double min_x = fmin(x1, x2);
//	double max_x = fmax(x1, x2);
//
//	double min_y = fmin(y1, y2);
//	double max_y = fmax(y1, y2);
//	double y_bar, y_bar1;
//
//	QCPBars *bars1 = new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis);
//	QCPBars *bars = new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis);
//	if (min_y >= 0) { //y1 > 0, y2 > 0
//		y_bar = max_y - min_y;
//		y_bar1 = min_y;
//
//		bars1->setPen(Qt::NoPen);// QPen(Qt::red));
//		bars1->setBrush(Qt::NoBrush);
//	}
//	else {
//		if (max_y >= 0) { //max_y > 0, min_y < 0
//			y_bar = max_y;
//			y_bar1 = min_y;
//
//			bars1->setPen(QColor(0, 168, 140, 100));// QPen(QColor(255, 0, 0, 100).lighter(130))); // QPen(Qt::red));
//			bars1->setBrush(QColor(0, 168, 140, 100));
//		}
//		else { //max_y < 0, min_y < 0
//			y_bar = min_y - max_y;
//			y_bar1 = max_y;
//
//			bars1->setPen(Qt::NoPen);// QPen(Qt::red));
//			bars1->setBrush(Qt::NoBrush);
//		}
//	}
//	printf("y_bar1, y_bar:(%f, %f)\n", y_bar1, y_bar);
//
//	QVector<double> x, y;
//	QVector<double> yy;
//
//	bars->moveAbove(bars1);
//	x.append(mid_x);
//
//	yy.append(y_bar1);
//	bars1->setWidth(max_x - min_x);
//	bars1->setData(x, yy);
//
//	y.append(y_bar);
//
//	bars->setWidth(max_x - min_x);
//	bars->setData(x, y);
//	bars->setPen(QColor(0, 168, 140, 100));// QPen(QColor(255, 0, 0, 100).lighter(130)));
//	bars->setBrush(QColor(0, 168, 140, 100));
//
//	bars->setName(QString::number(mid_x)); // name
//	bars1->setSelectable(QCP::SelectionType::stNone); //无法选中
//
//	ui.truePlot->replot();
//}

void mainWindow::setBar(double midx, double width, double min_y, double max_y, bool replot = true) 
{
	double y_bar, y_bar1;

	QCPBars *bars1 = new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis);
	QCPBars *bars = new QCPBars(ui.truePlot->xAxis, ui.truePlot->yAxis);
	if (min_y >= 0) { //y1 > 0, y2 > 0
		y_bar = max_y - min_y;
		y_bar1 = min_y;

		bars1->setPen(Qt::NoPen);// QPen(Qt::red));
		bars1->setBrush(Qt::NoBrush);
	}
	else {
		if (max_y >= 0) { //max_y > 0, min_y < 0
			y_bar = max_y;
			y_bar1 = min_y;

			bars1->setPen(QColor(0, 168, 140, 100));// QPen(QColor(255, 0, 0, 100).lighter(130))); // QPen(Qt::red));
			bars1->setBrush(QColor(0, 168, 140, 100));
		}
		else { //max_y < 0, min_y < 0
			y_bar = min_y - max_y;
			y_bar1 = max_y;

			bars1->setPen(Qt::NoPen);// QPen(Qt::red));
			bars1->setBrush(Qt::NoBrush);
		}
	}
	printf("y_bar1, y_bar:(%f, %f)\n", y_bar1, y_bar);

	QVector<double> x, y;
	QVector<double> yy;

	bars->moveAbove(bars1);
	x.append(midx);

	yy.append(y_bar1);
	bars1->setWidth(width);
	bars1->setData(x, yy);

	y.append(y_bar);

	bars->setWidth(width);
	bars->setData(x, y);
	bars->setPen(QColor(0, 168, 140, 100));// QPen(QColor(255, 0, 0, 100).lighter(130)));
	bars->setBrush(QColor(0, 168, 140, 100));

	bars->setName(QString::number(midx)); // name
	bars1->setSelectable(QCP::SelectionType::stNone); //无法选中
	if (replot)
		ui.truePlot->replot();
}

void mainWindow::setBar(barInfo barI, bool replot)
{
	setBar(barI.midx, barI.width, barI.ymin, barI.ymax, replot);
}

void mainWindow::setBars()
{
	const QRect zoomRect = lRubberBand->geometry();
	int xp1, yp1, xp2, yp2;
	zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
	printf("左键松开: (%d, %d), (%d, %d)\n", xp1, yp1, xp2, yp2);

	double x1 = ui.truePlot->xAxis->pixelToCoord(xp1);
	double x2 = ui.truePlot->xAxis->pixelToCoord(xp2);
	double y1 = ui.truePlot->yAxis->pixelToCoord(yp1);
	double y2 = ui.truePlot->yAxis->pixelToCoord(yp2);

	printf("坐标对应: (%f, %f), (%f, %f)\n", x1, y1, x2, y2);

	barInfo barI;

	barI.midx = (x1 + x2) / 2;
	barI.width = abs(x1 - x2);
	barI.ymin = fmin(y1, y2);
	barI.ymax = fmax(y1, y2);

	mapBarInfo[QString::number(barI.midx)] = barI;

	setBar(barI.midx, barI.width, barI.ymin, barI.ymax);

}


//todo: 保存最大谱及其标签位置信息
//保存平均谱及其标签位置信息，保存格式为二进制
void mainWindow::writeFile(std::string fileName, QMap<QString, barInfo> mapBarInfo, QVector<double> freqs)
{
	outStream.open(fileName, std::ios::binary);
	if (!outStream) {
		return;
	}

	int length = 25288;

	outStream.write(reinterpret_cast<char*>(&length), sizeof(int));
	outStream.write(reinterpret_cast<char*>(signal->curFrameInfo.aveSpectrum), 25288 * sizeof(char));
	QVector<labelResult> res;

	foreach(QString name, mapBarInfo.keys()) {
		labelResult resTmp;
		resTmp.start = my_binary_search(freqs, mapBarInfo[name].midx - mapBarInfo[name].width / 2, 0);
		resTmp.end = my_binary_search(freqs, mapBarInfo[name].midx + mapBarInfo[name].width / 2, 1);

		res.append(resTmp);

	}
	int label_nums = res.size();
	outStream.write(reinterpret_cast<char*>(&label_nums), sizeof(int));
	outStream.write(reinterpret_cast<char*>(res.data()), res.size() * sizeof(labelResult));

	outStream.close();
	saveState = true;

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

	ui.pageNumLineEdit->setText(QString::number(signal->curFrameInfo.curFrameNum) + " / " + QString::number(signal->allFrameNums));

	saveFileName_tmp = fileName.split("/", QString::SkipEmptyParts).back();

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
	//plotSpec(freqs, maxSpec, 0, Qt::magenta);

	plotSpec(freqs, aveSpec, 1, QColor(60, 60, 60), "ave");
	//plotSpec(freqs, maxSpec, 1, Qt::black, "max");

	setWholePlotRubbreBand();

	double alpha = abs((maxSpecV - minSpecV) / minSpecV);

	QVector<double> x, y, y1;

	for (auto res : signal->curFrameInfo.result) {
		barInfo barI;

		barI.midx = res.dFreqMid;
		barI.width = res.dBW;
		barI.ymax = res.dSignalLevel;
		barI.ymin = res.dSignalLevel - res.dCN;

		mapBarInfo[QString::number(barI.midx)] = barI;
		setBar(barI.midx, barI.width, barI.ymin, barI.ymax, false);

	}
	ui.truePlot->replot();

}

void mainWindow::saveFile()
{
	QString name = "label_" + QString::number(signal->curFrameInfo.curFrameNum) + "_" + saveFileName_tmp + ".label";
	QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("打开保存文件"), "./labelFile/" + name);

	//saveFileName_tmp = fileName;

	//std::cout << fileName.toStdString() << "-----------------"<< std::endl;
	ui.statusBar->showMessage(QString::fromLocal8Bit("保存文件：") + fileName);

	writeFile(fileName.toStdString(), mapBarInfo, freqs);

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
		std::cout << "press: (" << rubberOrigin.x() << ", " << rubberOrigin.y() << ")" << std::endl;
		rubberBand->setGeometry(QRect(rubberOrigin, QSize()));
		rubberBand->show();
	}

	if (mevent->button() == Qt::LeftButton && !dragAbel && !barSelected) {
		lRubberOrigin = mevent->pos();
		std::cout << "press: (" << lRubberOrigin.x() << ", " << lRubberOrigin.y() << ")" << std::endl;
		lRubberBand->setGeometry(QRect(lRubberOrigin, QSize()));
		lRubberBand->show();
	}
	setWholePlotRubbreBand();

	//Todo：移动一个Bar
	//问题：移动一个Bar与创建一个新的Bar的条件确立问题，当然针对当前问题而言两者并不矛盾，
	//		但需要重新确立条件并重新写移动后显示新Bar的方法。

	//if (mevent->button() == Qt::LeftButton && barSelected) {
	//	barOrigin = mevent->pos();
	//}
}
//鼠标移动槽函数
void mainWindow::mouseMove(QMouseEvent *mevent)
{
	if (rubberBand->isVisible())
		rubberBand->setGeometry(QRect(rubberOrigin, mevent->pos()).normalized());
	if (lRubberBand->isVisible())
		lRubberBand->setGeometry(QRect(lRubberOrigin, mevent->pos()).normalized());
	setWholePlotRubbreBand();

	//if (!barOrigin.isNull()) {
	//	if (ui.truePlot->selectedPlottables().size() > 0)
	//	{
	//		barInfo newBar = mapBarInfo[ui.truePlot->selectedPlottables().first()->name()];
	//		ui.truePlot->removePlottable(ui.truePlot->selectedPlottables().first());

	//		double x_start = ui.truePlot->xAxis->pixelToCoord(barOrigin.x());
	//		double y_start = ui.truePlot->yAxis->pixelToCoord(barOrigin.y());
	//		double x_end = ui.truePlot->xAxis->pixelToCoord(mevent->pos().x());
	//		double y_end = ui.truePlot->yAxis->pixelToCoord(mevent->pos().y());

	//		newBar.midx += (x_end - x_start);
	//		newBar.ymin += (y_end - y_start);
	//		newBar.ymax += (y_end - y_start);

	//		setBar(newBar);

	//		barSelected = false;
	//		ui.truePlot->replot();
	//	}
	//}
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
		printf("mouse release: (%d, %d), (%d, %d)\n", xp1, yp1, xp2, yp2);
		if (abs(xp1 - xp2) < 5 || abs(yp1 - yp2) < 5)
		{
			rubberBand->hide();
			return;
		}
		double x1 = ui.truePlot->xAxis->pixelToCoord(xp1);
		double x2 = ui.truePlot->xAxis->pixelToCoord(xp2);
		double y1 = ui.truePlot->yAxis->pixelToCoord(yp1);
		double y2 = ui.truePlot->yAxis->pixelToCoord(yp2);

		ui.truePlot->xAxis->setRange(x1, x2);
		ui.truePlot->yAxis->setRange(y1, y2);

		rubberBand->hide();
		ui.truePlot->replot();

		setWholePlotRubbreBand();
	}
	if (lRubberBand->isVisible()) {
		setBars();
		lRubberBand->hide();
	}
	//if (barSelected) {
	//	barOrigin = QPoint();
	//}
}


//槽函数实现1:1恢复功能
void mainWindow::slotBtn()
{
	//ui.menu_2->hideTearOffMenu();
	ui.truePlot->rescaleAxes();
	ui.truePlot->replot();
	
}

void mainWindow::test_drag()
{
	if (dragAbel) {
		dragAbel = false;
		
		ui.truePlot->setInteractions(QCP::iRangeZoom | QCP::iSelectAxes |
			QCP::iSelectLegend | QCP::iSelectPlottables);
	}
	else {
		dragAbel = true;
		ui.truePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
			QCP::iSelectLegend | QCP::iSelectPlottables);
	}
	
}

void mainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
	// since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
	// usually it's better to first check whether interface1D() returns non-zero, and only then use it.
	if (!plottable->name().isEmpty())
	{
		double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
		QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
		ui.statusBar->showMessage(message, 2500);

		selectedName = plottable->name();

		barSelected = true;
		std::cout << barSelected << std::endl;
	}
}

void mainWindow::selectionClosed()
{
	bool sc = false;
	for (int i = 0; i<ui.truePlot->plottableCount(); ++i)
	{
		QCPAbstractPlottable *plot = ui.truePlot->plottable(i);
		//QCPPlottableLegendItem *item = ui.truePlot->legend->itemWithPlottable(graph);
		if (plot->selected())
		{
			std::cout << "selectionChanged!" << std::endl;
			sc = true;
			return;
		}
	}
	if(!sc)
	{
		barSelected = false;
		std::cout << barSelected << std::endl;
	}
	//if (barSelected)
	//	barSelected = false;
	
}

void mainWindow::removeSelectedGraph()
{
	std::cout << "slot" << std::endl;
	if (ui.truePlot->selectedPlottables().size() > 0)
	{
		mapBarInfo.remove(ui.truePlot->selectedPlottables().first()->name()); //删除相对应的bar

		ui.truePlot->removePlottable(ui.truePlot->selectedPlottables().first());
		barSelected = false;
		ui.truePlot->replot();
	}
}

void mainWindow::openPage(unsigned long pageNum) //
{
	ui.truePlot->clearGraphs();
	ui.truePlot->clearPlottables();
	ui.truePlot->clearMask();

	ui.wholePlot->clearGraphs();
	ui.wholePlot->clearPlottables();
	ui.wholePlot->clearMask();

	aveSpec.clear();
	maxSpec.clear();
	mapBarInfo.clear();

	signal->curFrameInfo.curFrameNum = pageNum;
	signal->readFrame(signal->curFrameInfo);

	ui.pageNumLineEdit->setText(QString::number(signal->curFrameInfo.curFrameNum) + " / " + QString::number(signal->allFrameNums));

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
	//plotSpec(freqs, maxSpec, 0, Qt::magenta);

	plotSpec(freqs, aveSpec, 1, QColor(60, 60, 60), "ave");
	//plotSpec(freqs, maxSpec, 1, Qt::black, "max");

	setWholePlotRubbreBand();

	double alpha = abs((maxSpecV - minSpecV) / minSpecV);

	QVector<double> x, y, y1;

	for (auto res : signal->curFrameInfo.result) {
		barInfo barI;

		barI.midx = res.dFreqMid;
		barI.width = res.dBW;
		barI.ymax = res.dSignalLevel;
		barI.ymin = res.dSignalLevel - res.dCN;

		mapBarInfo[QString::number(barI.midx)] = barI;
		setBar(barI.midx, barI.width, barI.ymin, barI.ymax, false);

	}
	ui.truePlot->replot();

}

void mainWindow::firstPage()
{
	emit sendPageNum(1);
}

void mainWindow::prevPage()
{
	unsigned long num = signal->curFrameInfo.curFrameNum - 1;
	if (num < 1) {
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"),
			QString::fromLocal8Bit("已经是第一页！！！"));
		return;
	}
	emit sendPageNum(num);
}

void mainWindow::nextPage()
{
	unsigned long num = signal->curFrameInfo.curFrameNum + 1;

	if (num > signal->allFrameNums) {
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"),
			QString::fromLocal8Bit("已经是最后一页！！！"));
		return;
	}
	emit sendPageNum(num);
}

void mainWindow::gotoPage()
{
	auto num = ui.pageNumLineEdit->text().split("/", QString::SkipEmptyParts).first().toULong();

	if (num < 1 || num > signal->allFrameNums) {
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"),
			QString::fromLocal8Bit("超出页码范围！！！"));
		return;
	}
	emit sendPageNum(num);
}

//搜索当前频率在频谱数组中的位置
size_t my_binary_search(QVector<double> vec, double x, int pr)
{
	// x不存在越界问题

	size_t low = 0;
	size_t high = vec.size() - 1;

	while (low < high) {
		size_t mid = (low + high) / 2;
		if (vec[mid] == x) {
			return mid;
		}
		else if (vec[mid] > x) {
			high = mid;
		}
		else {
			low = mid + 1;
		}
	}

	//遍历完整个数组，都不相等，则x必然在两个数之间， 此时定位low一定和high相等
	assert(low == high);

	size_t tmp; //总是保存比x小的一侧数的位置
	if (vec[low] > x)
		tmp = low - 1;
	else
		tmp = low;

	if (pr == 1) //根据需要返回的位置，相应优先返回左侧或右侧位置，一般起点返回左侧，终点返回右侧
		return tmp + 1;
	else
		return tmp;
}

