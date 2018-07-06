#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "sig.h"
#include <QMap>
#include "myUtil.h"
#include <fstream>
#include <cassert>

enum return_priority {
	left_p,
	right_p
};

//搜索当前频率在频谱数组中的位置
size_t my_binary_search(QVector<double> vec, double x, int pr);

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
	QRubberBand *lRubberBand;
	QPoint lRubberOrigin;

	QPoint barOrigin;

	QPoint P;

	void plotSpec(QVector<double>x, QVector<double>y, int k, QColor color, QString name);
	void setWholePlotRubbreBand();
	void resizeEvent(QResizeEvent *event);

	void setBar(double midx, double width, double ymin, double ymax, bool);
	void setBar(barInfo, bool rebplot = true);
	//void setBarMid(double midx, double width, double ymin,	double ymax, bool);
	void setBars();

	void writeFile(std::string fileName, QMap<QString, barInfo> mapBarInfo, QVector<double> freqs);
	
	bool dragAbel = true;
	bool barSelected = false;
	QString selectedName;

	QMap<QString, barInfo> mapBarInfo;

	//文件保存
	std::ofstream outStream;
	QString saveFileName_tmp;
	bool saveState = false;


signals:
	void sendPageNum(unsigned long pageNum);

private slots:
	void openFile();
	void saveFile();

	void mouseWheel();
	void mousePress(QMouseEvent * mevent);

	void mouseMove(QMouseEvent * mevent);

	void mouseRelease(QMouseEvent * mevent);
	//void paintEvent(QPaintEvent *event);

	void slotBtn();

	void test_drag();
	void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
	void selectionClosed();
	void removeSelectedGraph();

	void openPage(unsigned long pageNum);
	void firstPage();
	void prevPage();
	void nextPage();
	void gotoPage();
};
