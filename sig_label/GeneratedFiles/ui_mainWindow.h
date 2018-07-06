/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *fileLabel;
    QPushButton *fileSelectBtn;
    QCustomPlot *wholePlot;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *firstPageBtn;
    QPushButton *previPageBtn;
    QLineEdit *pageNumLineEdit;
    QPushButton *nextPageBtn;
    QPushButton *gotoBtn;
    QSpacerItem *horizontalSpacer_2;
    QCustomPlot *truePlot;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *labelMenu;
    QMenu *deletMenu;
    QMenu *selectMenu;
    QMenu *zoomResetMenu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName(QStringLiteral("mainWindowClass"));
        mainWindowClass->resize(1078, 640);
        actionOpen = new QAction(mainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(mainWindowClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        centralWidget = new QWidget(mainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        fileLabel = new QLabel(centralWidget);
        fileLabel->setObjectName(QStringLiteral("fileLabel"));

        horizontalLayout->addWidget(fileLabel);

        fileSelectBtn = new QPushButton(centralWidget);
        fileSelectBtn->setObjectName(QStringLiteral("fileSelectBtn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileSelectBtn->sizePolicy().hasHeightForWidth());
        fileSelectBtn->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(fileSelectBtn);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        wholePlot = new QCustomPlot(centralWidget);
        wholePlot->setObjectName(QStringLiteral("wholePlot"));
        wholePlot->setEnabled(false);
        wholePlot->setMinimumSize(QSize(200, 150));

        gridLayout->addWidget(wholePlot, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        firstPageBtn = new QPushButton(centralWidget);
        firstPageBtn->setObjectName(QStringLiteral("firstPageBtn"));

        horizontalLayout_2->addWidget(firstPageBtn);

        previPageBtn = new QPushButton(centralWidget);
        previPageBtn->setObjectName(QStringLiteral("previPageBtn"));

        horizontalLayout_2->addWidget(previPageBtn);

        pageNumLineEdit = new QLineEdit(centralWidget);
        pageNumLineEdit->setObjectName(QStringLiteral("pageNumLineEdit"));
        pageNumLineEdit->setMaximumSize(QSize(200, 16777215));
        pageNumLineEdit->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(pageNumLineEdit);

        nextPageBtn = new QPushButton(centralWidget);
        nextPageBtn->setObjectName(QStringLiteral("nextPageBtn"));

        horizontalLayout_2->addWidget(nextPageBtn);

        gotoBtn = new QPushButton(centralWidget);
        gotoBtn->setObjectName(QStringLiteral("gotoBtn"));

        horizontalLayout_2->addWidget(gotoBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        truePlot = new QCustomPlot(centralWidget);
        truePlot->setObjectName(QStringLiteral("truePlot"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(truePlot->sizePolicy().hasHeightForWidth());
        truePlot->setSizePolicy(sizePolicy2);
        truePlot->setMinimumSize(QSize(300, 300));

        gridLayout->addWidget(truePlot, 3, 0, 1, 1);

        mainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1078, 23));
        fileMenu = new QMenu(menuBar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        labelMenu = new QMenu(menuBar);
        labelMenu->setObjectName(QStringLiteral("labelMenu"));
        deletMenu = new QMenu(menuBar);
        deletMenu->setObjectName(QStringLiteral("deletMenu"));
        selectMenu = new QMenu(menuBar);
        selectMenu->setObjectName(QStringLiteral("selectMenu"));
        zoomResetMenu = new QMenu(menuBar);
        zoomResetMenu->setObjectName(QStringLiteral("zoomResetMenu"));
        mainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(fileMenu->menuAction());
        menuBar->addAction(labelMenu->menuAction());
        menuBar->addAction(deletMenu->menuAction());
        menuBar->addAction(selectMenu->menuAction());
        menuBar->addAction(zoomResetMenu->menuAction());
        fileMenu->addAction(actionOpen);
        fileMenu->addAction(actionSave);

        retranslateUi(mainWindowClass);

        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QApplication::translate("mainWindowClass", "mainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("mainWindowClass", "\346\211\223\345\274\200(&O)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("mainWindowClass", "\346\211\223\345\274\200\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("mainWindowClass", "\344\277\235\345\255\230(&S)", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("mainWindowClass", "\344\277\235\345\255\230\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("mainWindowClass", "\346\226\207\344\273\266\357\274\232", Q_NULLPTR));
        fileLabel->setText(QApplication::translate("mainWindowClass", "TextLabel", Q_NULLPTR));
        fileSelectBtn->setText(QApplication::translate("mainWindowClass", "\351\200\211\346\213\251\346\226\207\344\273\266", Q_NULLPTR));
        firstPageBtn->setText(QApplication::translate("mainWindowClass", "\351\246\226\351\241\265", Q_NULLPTR));
        previPageBtn->setText(QApplication::translate("mainWindowClass", "\344\270\212\344\270\200\351\241\265", Q_NULLPTR));
        nextPageBtn->setText(QApplication::translate("mainWindowClass", "\344\270\213\344\270\200\351\241\265", Q_NULLPTR));
        gotoBtn->setText(QApplication::translate("mainWindowClass", "\350\267\263\350\275\254", Q_NULLPTR));
        fileMenu->setTitle(QApplication::translate("mainWindowClass", "\346\226\207\344\273\266", Q_NULLPTR));
        labelMenu->setTitle(QApplication::translate("mainWindowClass", "\346\240\207\350\256\260", Q_NULLPTR));
        deletMenu->setTitle(QApplication::translate("mainWindowClass", "\345\210\240\351\231\244", Q_NULLPTR));
        selectMenu->setTitle(QApplication::translate("mainWindowClass", "\351\200\211\346\213\251", Q_NULLPTR));
        zoomResetMenu->setTitle(QApplication::translate("mainWindowClass", "\346\201\242\345\244\215", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
