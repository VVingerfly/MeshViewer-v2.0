/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionWireFrame;
    QAction *actionPointSet;
    QAction *actionHiddenLines;
    QAction *actionSolidFlat;
    QAction *actionSolidSmooth;
    QAction *actionFlatLines;
    QAction *actionBBox;
    QAction *actionAxis;
    QAction *actionTexture;
    QAction *actionFindVert;
    QAction *actionFindFace;
    QAction *actionFindEdge;
    QAction *actionPickVert;
    QAction *actionPickEdge;
    QAction *actionPickFace;
    QAction *actionSaveSelectedVert;
    QAction *actionSaveSelectedEdge;
    QAction *actionSaveSelectedFace;
    QAction *actionFindPoint;
    QAction *actionClearSelectedVert;
    QAction *actionClearSelectedEdge;
    QAction *actionClearSelectedFace;
    QAction *actionClearSelectedAll;
    QAction *actionBoundary;
    QAction *actionMeshInfo;
    QAction *actionShowSelectedVert;
    QAction *actionShowSelectedEdge;
    QAction *actionShowSelectedFace;
    QAction *actionResetSelection;
    QAction *actionTagSelectedVert;
    QAction *actionTagSelectedEdge;
    QAction *actionTagSelectedFace;
    QAction *actionAbout;
    QAction *actionExit;
    QAction *actionSaveScreen;
    QAction *actionHidden;
    QAction *actionLoadTexture;
    QAction *actionResetArcBall;
    QAction *actionSetColor;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuTool;
    QMenu *menuFind;
    QMenu *menu;
    QMenu *menuPick;
    QMenu *menuClear;
    QMenu *menuShow;
    QMenu *menuTag;
    QMenu *menuHelp;
    QToolBar *toolBarFile;
    QStatusBar *statusBar;
    QToolBar *toolBarMeshRenderMode;
    QToolBar *toolBarSelection;
    QDockWidget *dockWidgetCommand;
    QWidget *dockWidgetContents;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *labelSource;
    QRadioButton *radioButtonActiveSource;
    QCheckBox *checkBoxDrawSource;
    QLabel *labelTarget;
    QRadioButton *radioButtonActiveTarget;
    QCheckBox *checkBoxDrawTarget;
    QPushButton *pushButtonSwap;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(830, 568);
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/Resources/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon);
        actionSave = new QAction(MainWindowClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/Resources/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionWireFrame = new QAction(MainWindowClass);
        actionWireFrame->setObjectName(QStringLiteral("actionWireFrame"));
        actionWireFrame->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/Resources/images/meshWirFramee.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWireFrame->setIcon(icon2);
        actionPointSet = new QAction(MainWindowClass);
        actionPointSet->setObjectName(QStringLiteral("actionPointSet"));
        actionPointSet->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/Resources/images/meshPointSet.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPointSet->setIcon(icon3);
        actionHiddenLines = new QAction(MainWindowClass);
        actionHiddenLines->setObjectName(QStringLiteral("actionHiddenLines"));
        actionHiddenLines->setCheckable(true);
        actionHiddenLines->setChecked(false);
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/Resources/images/meshHiddenLines.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHiddenLines->setIcon(icon4);
        actionSolidFlat = new QAction(MainWindowClass);
        actionSolidFlat->setObjectName(QStringLiteral("actionSolidFlat"));
        actionSolidFlat->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/Resources/images/meshSolidFlat.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSolidFlat->setIcon(icon5);
        actionSolidSmooth = new QAction(MainWindowClass);
        actionSolidSmooth->setObjectName(QStringLiteral("actionSolidSmooth"));
        actionSolidSmooth->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/Resources/images/meshSolidSmooth.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSolidSmooth->setIcon(icon6);
        actionFlatLines = new QAction(MainWindowClass);
        actionFlatLines->setObjectName(QStringLiteral("actionFlatLines"));
        actionFlatLines->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/Resources/images/meshFlatLines.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFlatLines->setIcon(icon7);
        actionBBox = new QAction(MainWindowClass);
        actionBBox->setObjectName(QStringLiteral("actionBBox"));
        actionBBox->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/Resources/images/meshBBox.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBBox->setIcon(icon8);
        actionAxis = new QAction(MainWindowClass);
        actionAxis->setObjectName(QStringLiteral("actionAxis"));
        actionAxis->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/Resources/images/axis.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAxis->setIcon(icon9);
        actionTexture = new QAction(MainWindowClass);
        actionTexture->setObjectName(QStringLiteral("actionTexture"));
        actionTexture->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/Resources/images/texture.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTexture->setIcon(icon10);
        actionFindVert = new QAction(MainWindowClass);
        actionFindVert->setObjectName(QStringLiteral("actionFindVert"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/Resources/images/selectVerticesFind.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFindVert->setIcon(icon11);
        actionFindFace = new QAction(MainWindowClass);
        actionFindFace->setObjectName(QStringLiteral("actionFindFace"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/images/Resources/images/selectFacesFind.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFindFace->setIcon(icon12);
        actionFindEdge = new QAction(MainWindowClass);
        actionFindEdge->setObjectName(QStringLiteral("actionFindEdge"));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/images/Resources/images/selectEdgesFind.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFindEdge->setIcon(icon13);
        actionPickVert = new QAction(MainWindowClass);
        actionPickVert->setObjectName(QStringLiteral("actionPickVert"));
        actionPickVert->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/images/Resources/images/selectVerticesClick.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPickVert->setIcon(icon14);
        actionPickEdge = new QAction(MainWindowClass);
        actionPickEdge->setObjectName(QStringLiteral("actionPickEdge"));
        actionPickEdge->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/images/Resources/images/selectEdgesClick.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPickEdge->setIcon(icon15);
        actionPickFace = new QAction(MainWindowClass);
        actionPickFace->setObjectName(QStringLiteral("actionPickFace"));
        actionPickFace->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/images/Resources/images/selectFacesClick.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPickFace->setIcon(icon16);
        actionSaveSelectedVert = new QAction(MainWindowClass);
        actionSaveSelectedVert->setObjectName(QStringLiteral("actionSaveSelectedVert"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/images/Resources/images/saveSelectedVertices.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveSelectedVert->setIcon(icon17);
        actionSaveSelectedEdge = new QAction(MainWindowClass);
        actionSaveSelectedEdge->setObjectName(QStringLiteral("actionSaveSelectedEdge"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/images/Resources/images/saveSelectedEdges.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveSelectedEdge->setIcon(icon18);
        actionSaveSelectedFace = new QAction(MainWindowClass);
        actionSaveSelectedFace->setObjectName(QStringLiteral("actionSaveSelectedFace"));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/images/Resources/images/saveSelectedFaces.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveSelectedFace->setIcon(icon19);
        actionFindPoint = new QAction(MainWindowClass);
        actionFindPoint->setObjectName(QStringLiteral("actionFindPoint"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/images/Resources/images/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFindPoint->setIcon(icon20);
        actionClearSelectedVert = new QAction(MainWindowClass);
        actionClearSelectedVert->setObjectName(QStringLiteral("actionClearSelectedVert"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/images/Resources/images/clearSelectedVertices.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearSelectedVert->setIcon(icon21);
        actionClearSelectedEdge = new QAction(MainWindowClass);
        actionClearSelectedEdge->setObjectName(QStringLiteral("actionClearSelectedEdge"));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/images/Resources/images/clearSelectedEdges.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearSelectedEdge->setIcon(icon22);
        actionClearSelectedFace = new QAction(MainWindowClass);
        actionClearSelectedFace->setObjectName(QStringLiteral("actionClearSelectedFace"));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/images/Resources/images/clearSelectedFaces.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearSelectedFace->setIcon(icon23);
        actionClearSelectedAll = new QAction(MainWindowClass);
        actionClearSelectedAll->setObjectName(QStringLiteral("actionClearSelectedAll"));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/images/Resources/images/clearSelection.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearSelectedAll->setIcon(icon24);
        actionBoundary = new QAction(MainWindowClass);
        actionBoundary->setObjectName(QStringLiteral("actionBoundary"));
        actionBoundary->setCheckable(true);
        QIcon icon25;
        icon25.addFile(QStringLiteral(":/images/Resources/images/meshBoundary.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBoundary->setIcon(icon25);
        actionMeshInfo = new QAction(MainWindowClass);
        actionMeshInfo->setObjectName(QStringLiteral("actionMeshInfo"));
        QIcon icon26;
        icon26.addFile(QStringLiteral(":/images/Resources/images/info.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMeshInfo->setIcon(icon26);
        actionShowSelectedVert = new QAction(MainWindowClass);
        actionShowSelectedVert->setObjectName(QStringLiteral("actionShowSelectedVert"));
        actionShowSelectedVert->setCheckable(true);
        actionShowSelectedVert->setChecked(true);
        actionShowSelectedEdge = new QAction(MainWindowClass);
        actionShowSelectedEdge->setObjectName(QStringLiteral("actionShowSelectedEdge"));
        actionShowSelectedEdge->setCheckable(true);
        actionShowSelectedEdge->setChecked(true);
        actionShowSelectedFace = new QAction(MainWindowClass);
        actionShowSelectedFace->setObjectName(QStringLiteral("actionShowSelectedFace"));
        actionShowSelectedFace->setCheckable(true);
        actionShowSelectedFace->setChecked(true);
        actionResetSelection = new QAction(MainWindowClass);
        actionResetSelection->setObjectName(QStringLiteral("actionResetSelection"));
        QIcon icon27;
        icon27.addFile(QStringLiteral(":/images/Resources/images/cursor.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionResetSelection->setIcon(icon27);
        actionTagSelectedVert = new QAction(MainWindowClass);
        actionTagSelectedVert->setObjectName(QStringLiteral("actionTagSelectedVert"));
        actionTagSelectedVert->setCheckable(true);
        actionTagSelectedEdge = new QAction(MainWindowClass);
        actionTagSelectedEdge->setObjectName(QStringLiteral("actionTagSelectedEdge"));
        actionTagSelectedEdge->setCheckable(true);
        actionTagSelectedFace = new QAction(MainWindowClass);
        actionTagSelectedFace->setObjectName(QStringLiteral("actionTagSelectedFace"));
        actionTagSelectedFace->setCheckable(true);
        actionAbout = new QAction(MainWindowClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSaveScreen = new QAction(MainWindowClass);
        actionSaveScreen->setObjectName(QStringLiteral("actionSaveScreen"));
        QIcon icon28;
        icon28.addFile(QStringLiteral(":/images/Resources/images/saveScreen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveScreen->setIcon(icon28);
        actionHidden = new QAction(MainWindowClass);
        actionHidden->setObjectName(QStringLiteral("actionHidden"));
        actionHidden->setCheckable(true);
        QIcon icon29;
        icon29.addFile(QStringLiteral(":/images/Resources/images/eye_invisible.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHidden->setIcon(icon29);
        actionLoadTexture = new QAction(MainWindowClass);
        actionLoadTexture->setObjectName(QStringLiteral("actionLoadTexture"));
        actionResetArcBall = new QAction(MainWindowClass);
        actionResetArcBall->setObjectName(QStringLiteral("actionResetArcBall"));
        actionSetColor = new QAction(MainWindowClass);
        actionSetColor->setObjectName(QStringLiteral("actionSetColor"));
        actionSetColor->setCheckable(false);
        QIcon icon30;
        icon30.addFile(QStringLiteral(":/images/Resources/images/Color_Palette.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetColor->setIcon(icon30);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 830, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        menuFind = new QMenu(menuTool);
        menuFind->setObjectName(QStringLiteral("menuFind"));
        menuFind->setBaseSize(QSize(2, 0));
        menuFind->setIcon(icon20);
        menu = new QMenu(menuTool);
        menu->setObjectName(QStringLiteral("menu"));
        QIcon icon31;
        icon31.addFile(QStringLiteral(":/images/Resources/images/saveSelection.png"), QSize(), QIcon::Normal, QIcon::Off);
        menu->setIcon(icon31);
        menuPick = new QMenu(menuTool);
        menuPick->setObjectName(QStringLiteral("menuPick"));
        QIcon icon32;
        icon32.addFile(QStringLiteral(":/images/Resources/images/hand_up_pointing.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuPick->setIcon(icon32);
        menuClear = new QMenu(menuTool);
        menuClear->setObjectName(QStringLiteral("menuClear"));
        QIcon icon33;
        icon33.addFile(QStringLiteral(":/images/Resources/images/broom.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuClear->setIcon(icon33);
        menuShow = new QMenu(menuTool);
        menuShow->setObjectName(QStringLiteral("menuShow"));
        QIcon icon34;
        icon34.addFile(QStringLiteral(":/images/Resources/images/eye.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuShow->setIcon(icon34);
        menuTag = new QMenu(menuTool);
        menuTag->setObjectName(QStringLiteral("menuTag"));
        QIcon icon35;
        icon35.addFile(QStringLiteral(":/images/Resources/images/tag.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuTag->setIcon(icon35);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindowClass->setMenuBar(menuBar);
        toolBarFile = new QToolBar(MainWindowClass);
        toolBarFile->setObjectName(QStringLiteral("toolBarFile"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBarFile);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        toolBarMeshRenderMode = new QToolBar(MainWindowClass);
        toolBarMeshRenderMode->setObjectName(QStringLiteral("toolBarMeshRenderMode"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBarMeshRenderMode);
        toolBarSelection = new QToolBar(MainWindowClass);
        toolBarSelection->setObjectName(QStringLiteral("toolBarSelection"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBarSelection);
        dockWidgetCommand = new QDockWidget(MainWindowClass);
        dockWidgetCommand->setObjectName(QStringLiteral("dockWidgetCommand"));
        dockWidgetCommand->setMinimumSize(QSize(93, 101));
        dockWidgetCommand->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        layoutWidget = new QWidget(dockWidgetContents);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 79, 65));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelSource = new QLabel(layoutWidget);
        labelSource->setObjectName(QStringLiteral("labelSource"));
        labelSource->setAutoFillBackground(false);
        labelSource->setMidLineWidth(1);
        labelSource->setIndent(0);

        gridLayout->addWidget(labelSource, 0, 0, 1, 1);

        radioButtonActiveSource = new QRadioButton(layoutWidget);
        radioButtonActiveSource->setObjectName(QStringLiteral("radioButtonActiveSource"));
        radioButtonActiveSource->setChecked(true);

        gridLayout->addWidget(radioButtonActiveSource, 0, 1, 1, 1);

        checkBoxDrawSource = new QCheckBox(layoutWidget);
        checkBoxDrawSource->setObjectName(QStringLiteral("checkBoxDrawSource"));
        checkBoxDrawSource->setChecked(true);

        gridLayout->addWidget(checkBoxDrawSource, 0, 2, 1, 1);

        labelTarget = new QLabel(layoutWidget);
        labelTarget->setObjectName(QStringLiteral("labelTarget"));
        labelTarget->setIndent(1);

        gridLayout->addWidget(labelTarget, 1, 0, 1, 1);

        radioButtonActiveTarget = new QRadioButton(layoutWidget);
        radioButtonActiveTarget->setObjectName(QStringLiteral("radioButtonActiveTarget"));

        gridLayout->addWidget(radioButtonActiveTarget, 1, 1, 1, 1);

        checkBoxDrawTarget = new QCheckBox(layoutWidget);
        checkBoxDrawTarget->setObjectName(QStringLiteral("checkBoxDrawTarget"));
        checkBoxDrawTarget->setChecked(true);

        gridLayout->addWidget(checkBoxDrawTarget, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        pushButtonSwap = new QPushButton(layoutWidget);
        pushButtonSwap->setObjectName(QStringLiteral("pushButtonSwap"));

        gridLayout_2->addWidget(pushButtonSwap, 1, 0, 1, 1);

        dockWidgetCommand->setWidget(dockWidgetContents);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetCommand);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionLoadTexture);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionSaveScreen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionHidden);
        menuView->addAction(actionPointSet);
        menuView->addAction(actionWireFrame);
        menuView->addAction(actionHiddenLines);
        menuView->addAction(actionFlatLines);
        menuView->addAction(actionSolidFlat);
        menuView->addAction(actionSolidSmooth);
        menuView->addSeparator();
        menuView->addAction(actionAxis);
        menuView->addAction(actionBBox);
        menuView->addAction(actionBoundary);
        menuView->addAction(actionTexture);
        menuView->addSeparator();
        menuView->addAction(actionMeshInfo);
        menuView->addAction(actionResetArcBall);
        menuView->addAction(actionSetColor);
        menuTool->addAction(actionResetSelection);
        menuTool->addAction(menuPick->menuAction());
        menuTool->addAction(menuFind->menuAction());
        menuTool->addAction(menuShow->menuAction());
        menuTool->addAction(menuTag->menuAction());
        menuTool->addAction(menu->menuAction());
        menuTool->addAction(menuClear->menuAction());
        menuFind->addAction(actionFindPoint);
        menuFind->addAction(actionFindVert);
        menuFind->addAction(actionFindEdge);
        menuFind->addAction(actionFindFace);
        menu->addAction(actionSaveSelectedVert);
        menu->addAction(actionSaveSelectedEdge);
        menu->addAction(actionSaveSelectedFace);
        menuPick->addAction(actionPickVert);
        menuPick->addAction(actionPickEdge);
        menuPick->addAction(actionPickFace);
        menuClear->addAction(actionClearSelectedVert);
        menuClear->addAction(actionClearSelectedEdge);
        menuClear->addAction(actionClearSelectedFace);
        menuClear->addAction(actionClearSelectedAll);
        menuShow->addAction(actionShowSelectedVert);
        menuShow->addAction(actionShowSelectedEdge);
        menuShow->addAction(actionShowSelectedFace);
        menuTag->addAction(actionTagSelectedVert);
        menuTag->addAction(actionTagSelectedEdge);
        menuTag->addAction(actionTagSelectedFace);
        menuHelp->addAction(actionAbout);
        toolBarFile->addAction(actionOpen);
        toolBarFile->addAction(actionSave);
        toolBarFile->addAction(actionSaveScreen);
        toolBarMeshRenderMode->addAction(actionSetColor);
        toolBarMeshRenderMode->addAction(actionHidden);
        toolBarMeshRenderMode->addAction(actionPointSet);
        toolBarMeshRenderMode->addAction(actionWireFrame);
        toolBarMeshRenderMode->addAction(actionHiddenLines);
        toolBarMeshRenderMode->addAction(actionFlatLines);
        toolBarMeshRenderMode->addAction(actionSolidFlat);
        toolBarMeshRenderMode->addAction(actionSolidSmooth);
        toolBarSelection->addAction(actionResetSelection);
        toolBarSelection->addAction(actionFindPoint);
        toolBarSelection->addAction(actionFindVert);
        toolBarSelection->addAction(actionFindEdge);
        toolBarSelection->addAction(actionFindFace);
        toolBarSelection->addAction(actionPickVert);
        toolBarSelection->addAction(actionPickEdge);
        toolBarSelection->addAction(actionPickFace);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0));
        actionOpen->setText(QApplication::translate("MainWindowClass", "&Open", 0));
#ifndef QT_NO_STATUSTIP
        actionOpen->setStatusTip(QApplication::translate("MainWindowClass", "Load a triangular mesh", 0));
#endif // QT_NO_STATUSTIP
        actionOpen->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindowClass", "&Save", 0));
#ifndef QT_NO_STATUSTIP
        actionSave->setStatusTip(QApplication::translate("MainWindowClass", "Save current mesh", 0));
#endif // QT_NO_STATUSTIP
        actionSave->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0));
        actionWireFrame->setText(QApplication::translate("MainWindowClass", "WireFrame", 0));
#ifndef QT_NO_STATUSTIP
        actionWireFrame->setStatusTip(QApplication::translate("MainWindowClass", "Draw wireframe", 0));
#endif // QT_NO_STATUSTIP
        actionPointSet->setText(QApplication::translate("MainWindowClass", "PointSet", 0));
#ifndef QT_NO_STATUSTIP
        actionPointSet->setStatusTip(QApplication::translate("MainWindowClass", "Draw point set", 0));
#endif // QT_NO_STATUSTIP
        actionHiddenLines->setText(QApplication::translate("MainWindowClass", "HiddenLines", 0));
#ifndef QT_NO_STATUSTIP
        actionHiddenLines->setStatusTip(QApplication::translate("MainWindowClass", "Draw hiddenlines", 0));
#endif // QT_NO_STATUSTIP
        actionSolidFlat->setText(QApplication::translate("MainWindowClass", "SolidFlat", 0));
#ifndef QT_NO_STATUSTIP
        actionSolidFlat->setStatusTip(QApplication::translate("MainWindowClass", "Draw solid flat", 0));
#endif // QT_NO_STATUSTIP
        actionSolidSmooth->setText(QApplication::translate("MainWindowClass", "SolidSmooth", 0));
#ifndef QT_NO_STATUSTIP
        actionSolidSmooth->setStatusTip(QApplication::translate("MainWindowClass", "Draw solid smooth", 0));
#endif // QT_NO_STATUSTIP
        actionFlatLines->setText(QApplication::translate("MainWindowClass", "FlatLines", 0));
#ifndef QT_NO_STATUSTIP
        actionFlatLines->setStatusTip(QApplication::translate("MainWindowClass", "Draw flatlines", 0));
#endif // QT_NO_STATUSTIP
        actionBBox->setText(QApplication::translate("MainWindowClass", "BBox", 0));
#ifndef QT_NO_STATUSTIP
        actionBBox->setStatusTip(QApplication::translate("MainWindowClass", "Draw mesh bounding box", 0));
#endif // QT_NO_STATUSTIP
        actionAxis->setText(QApplication::translate("MainWindowClass", "Axis", 0));
#ifndef QT_NO_STATUSTIP
        actionAxis->setStatusTip(QApplication::translate("MainWindowClass", "Draw XYZ axes in world coordinates", 0));
#endif // QT_NO_STATUSTIP
        actionTexture->setText(QApplication::translate("MainWindowClass", "Texture", 0));
#ifndef QT_NO_STATUSTIP
        actionTexture->setStatusTip(QApplication::translate("MainWindowClass", "Draw mesh texture", 0));
#endif // QT_NO_STATUSTIP
        actionFindVert->setText(QApplication::translate("MainWindowClass", "FindVert", 0));
#ifndef QT_NO_STATUSTIP
        actionFindVert->setStatusTip(QApplication::translate("MainWindowClass", "Find a vertex by given index", 0));
#endif // QT_NO_STATUSTIP
        actionFindFace->setText(QApplication::translate("MainWindowClass", "FindFace", 0));
#ifndef QT_NO_STATUSTIP
        actionFindFace->setStatusTip(QApplication::translate("MainWindowClass", "Find a face by given index", 0));
#endif // QT_NO_STATUSTIP
        actionFindEdge->setText(QApplication::translate("MainWindowClass", "FindEdge", 0));
#ifndef QT_NO_STATUSTIP
        actionFindEdge->setStatusTip(QApplication::translate("MainWindowClass", "Find an edge by given index", 0));
#endif // QT_NO_STATUSTIP
        actionPickVert->setText(QApplication::translate("MainWindowClass", "PickVert", 0));
#ifndef QT_NO_STATUSTIP
        actionPickVert->setStatusTip(QApplication::translate("MainWindowClass", "Pick a vertex by a single click", 0));
#endif // QT_NO_STATUSTIP
        actionPickEdge->setText(QApplication::translate("MainWindowClass", "PickEdge", 0));
#ifndef QT_NO_STATUSTIP
        actionPickEdge->setStatusTip(QApplication::translate("MainWindowClass", "Pick an edge by a single click", 0));
#endif // QT_NO_STATUSTIP
        actionPickFace->setText(QApplication::translate("MainWindowClass", "PickFace", 0));
#ifndef QT_NO_STATUSTIP
        actionPickFace->setStatusTip(QApplication::translate("MainWindowClass", "Pick a face by a single click", 0));
#endif // QT_NO_STATUSTIP
        actionSaveSelectedVert->setText(QApplication::translate("MainWindowClass", "SaveSelectedVert", 0));
#ifndef QT_NO_STATUSTIP
        actionSaveSelectedVert->setStatusTip(QApplication::translate("MainWindowClass", "Save current selected vertices to file", 0));
#endif // QT_NO_STATUSTIP
        actionSaveSelectedEdge->setText(QApplication::translate("MainWindowClass", "SaveSelectedEdge", 0));
#ifndef QT_NO_STATUSTIP
        actionSaveSelectedEdge->setStatusTip(QApplication::translate("MainWindowClass", "Save current selected edges to file", 0));
#endif // QT_NO_STATUSTIP
        actionSaveSelectedFace->setText(QApplication::translate("MainWindowClass", "SaveSelectedFace", 0));
#ifndef QT_NO_STATUSTIP
        actionSaveSelectedFace->setStatusTip(QApplication::translate("MainWindowClass", "Save current selected faces to file", 0));
#endif // QT_NO_STATUSTIP
        actionFindPoint->setText(QApplication::translate("MainWindowClass", "FindPoint", 0));
#ifndef QT_NO_STATUSTIP
        actionFindPoint->setStatusTip(QApplication::translate("MainWindowClass", "Find the closest vertex to a given 3D point", 0));
#endif // QT_NO_STATUSTIP
        actionClearSelectedVert->setText(QApplication::translate("MainWindowClass", "ClearSelectedVert", 0));
#ifndef QT_NO_STATUSTIP
        actionClearSelectedVert->setStatusTip(QApplication::translate("MainWindowClass", "Clear current selected vertices", 0));
#endif // QT_NO_STATUSTIP
        actionClearSelectedEdge->setText(QApplication::translate("MainWindowClass", "ClearSelectedEdge", 0));
#ifndef QT_NO_STATUSTIP
        actionClearSelectedEdge->setStatusTip(QApplication::translate("MainWindowClass", "Clear current selected edges", 0));
#endif // QT_NO_STATUSTIP
        actionClearSelectedFace->setText(QApplication::translate("MainWindowClass", "ClearSelectedFace", 0));
#ifndef QT_NO_STATUSTIP
        actionClearSelectedFace->setStatusTip(QApplication::translate("MainWindowClass", "Clear current selected faces", 0));
#endif // QT_NO_STATUSTIP
        actionClearSelectedAll->setText(QApplication::translate("MainWindowClass", "ClearSelectedAll", 0));
#ifndef QT_NO_STATUSTIP
        actionClearSelectedAll->setStatusTip(QApplication::translate("MainWindowClass", "Clear current selected vertices, edges and faces", 0));
#endif // QT_NO_STATUSTIP
        actionBoundary->setText(QApplication::translate("MainWindowClass", "Boundary", 0));
#ifndef QT_NO_STATUSTIP
        actionBoundary->setStatusTip(QApplication::translate("MainWindowClass", "Draw mesh boundary", 0));
#endif // QT_NO_STATUSTIP
        actionMeshInfo->setText(QApplication::translate("MainWindowClass", "MeshInfo", 0));
#ifndef QT_NO_STATUSTIP
        actionMeshInfo->setStatusTip(QApplication::translate("MainWindowClass", "Show mesh information", 0));
#endif // QT_NO_STATUSTIP
        actionShowSelectedVert->setText(QApplication::translate("MainWindowClass", "ShowVert", 0));
#ifndef QT_NO_STATUSTIP
        actionShowSelectedVert->setStatusTip(QApplication::translate("MainWindowClass", "Draw current selected vertices or not", 0));
#endif // QT_NO_STATUSTIP
        actionShowSelectedEdge->setText(QApplication::translate("MainWindowClass", "ShowEdge", 0));
#ifndef QT_NO_STATUSTIP
        actionShowSelectedEdge->setStatusTip(QApplication::translate("MainWindowClass", "Draw current selected edges or not", 0));
#endif // QT_NO_STATUSTIP
        actionShowSelectedFace->setText(QApplication::translate("MainWindowClass", "ShowFace", 0));
#ifndef QT_NO_STATUSTIP
        actionShowSelectedFace->setStatusTip(QApplication::translate("MainWindowClass", "Draw current selected faces or not", 0));
#endif // QT_NO_STATUSTIP
        actionResetSelection->setText(QApplication::translate("MainWindowClass", "Reset", 0));
#ifndef QT_NO_STATUSTIP
        actionResetSelection->setStatusTip(QApplication::translate("MainWindowClass", "Reset selection mode", 0));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        actionResetSelection->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        actionTagSelectedVert->setText(QApplication::translate("MainWindowClass", "TagVert", 0));
#ifndef QT_NO_STATUSTIP
        actionTagSelectedVert->setStatusTip(QApplication::translate("MainWindowClass", "Draw current selected vertex tags", 0));
#endif // QT_NO_STATUSTIP
        actionTagSelectedEdge->setText(QApplication::translate("MainWindowClass", "TagEdge", 0));
#ifndef QT_NO_STATUSTIP
        actionTagSelectedEdge->setStatusTip(QApplication::translate("MainWindowClass", "Draw current selected edge tags", 0));
#endif // QT_NO_STATUSTIP
        actionTagSelectedFace->setText(QApplication::translate("MainWindowClass", "TagFace", 0));
#ifndef QT_NO_STATUSTIP
        actionTagSelectedFace->setStatusTip(QApplication::translate("MainWindowClass", "Draw current selected face tags", 0));
#endif // QT_NO_STATUSTIP
        actionAbout->setText(QApplication::translate("MainWindowClass", "About", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
#ifndef QT_NO_STATUSTIP
        actionExit->setStatusTip(QApplication::translate("MainWindowClass", "Exit", 0));
#endif // QT_NO_STATUSTIP
        actionExit->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Q", 0));
        actionSaveScreen->setText(QApplication::translate("MainWindowClass", "Save Screen", 0));
#ifndef QT_NO_STATUSTIP
        actionSaveScreen->setStatusTip(QApplication::translate("MainWindowClass", "Save Screen", 0));
#endif // QT_NO_STATUSTIP
        actionHidden->setText(QApplication::translate("MainWindowClass", "Hidden", 0));
#ifndef QT_NO_STATUSTIP
        actionHidden->setStatusTip(QApplication::translate("MainWindowClass", "Hide current mesh", 0));
#endif // QT_NO_STATUSTIP
        actionLoadTexture->setText(QApplication::translate("MainWindowClass", "Load Texture", 0));
#ifndef QT_NO_STATUSTIP
        actionLoadTexture->setStatusTip(QApplication::translate("MainWindowClass", "Load a texture image", 0));
#endif // QT_NO_STATUSTIP
        actionResetArcBall->setText(QApplication::translate("MainWindowClass", "Reset ArcBall", 0));
#ifndef QT_NO_STATUSTIP
        actionResetArcBall->setStatusTip(QApplication::translate("MainWindowClass", "Reset ArcBall", 0));
#endif // QT_NO_STATUSTIP
        actionSetColor->setText(QApplication::translate("MainWindowClass", "Color", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "&File", 0));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0));
        menuTool->setTitle(QApplication::translate("MainWindowClass", "Selection", 0));
        menuFind->setTitle(QApplication::translate("MainWindowClass", "Find", 0));
        menu->setTitle(QApplication::translate("MainWindowClass", "Save", 0));
        menuPick->setTitle(QApplication::translate("MainWindowClass", "Pick", 0));
        menuClear->setTitle(QApplication::translate("MainWindowClass", "Clear", 0));
        menuShow->setTitle(QApplication::translate("MainWindowClass", "Show", 0));
        menuTag->setTitle(QApplication::translate("MainWindowClass", "Tag", 0));
        menuHelp->setTitle(QApplication::translate("MainWindowClass", "Help", 0));
        toolBarFile->setWindowTitle(QApplication::translate("MainWindowClass", "toolBarFile", 0));
        toolBarMeshRenderMode->setWindowTitle(QApplication::translate("MainWindowClass", "toolBarRender", 0));
        toolBarSelection->setWindowTitle(QApplication::translate("MainWindowClass", "toolBarSelection", 0));
        dockWidgetCommand->setWindowTitle(QApplication::translate("MainWindowClass", "Command", 0));
        labelSource->setText(QApplication::translate("MainWindowClass", "source", 0));
        radioButtonActiveSource->setText(QString());
        checkBoxDrawSource->setText(QString());
        labelTarget->setText(QApplication::translate("MainWindowClass", "target", 0));
        radioButtonActiveTarget->setText(QString());
        checkBoxDrawTarget->setText(QString());
        pushButtonSwap->setText(QApplication::translate("MainWindowClass", "Swap", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
