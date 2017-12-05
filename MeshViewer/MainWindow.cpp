//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/10 13:33:12
// Last Change  : 2017/Jan/10 13:33:23
// Summary      :
//===============================================================

#include "MainWindow.h"


#include <QtWidgets/QLayout>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	meshViewer_ = new MeshViewer(this);

	QVBoxLayout *layout_left = new QVBoxLayout;
	//layout_left->addWidget(ui.groupBox_render);
	//layout_left->addStretch(4);

	QHBoxLayout *layout_main = new QHBoxLayout;
	layout_main->addLayout(layout_left);
	layout_main->addWidget(meshViewer_);
	layout_main->setStretch(1, 1);

	
	//qq = QGroupButton
	//	ui->setId(ui->radioButton, 0);
	//	ui->BG->setId(ui->radioButton_2, 1);
	//	ui->BG->setId(ui->radioButton_3, 2);
	//	ui->radioButton->setChecked(true);
	CreateStatusBar();
	CreateActionGroup();
	CreateAction();
	setMouseTracking(true);
	this->centralWidget()->setLayout(layout_main);
}

MainWindow::~MainWindow()
{

}



void MainWindow::CreateActionGroup()
{
	QActionGroup *meshRenderGroup = new QActionGroup(this);
	meshRenderGroup->addAction(ui.actionHidden);
	meshRenderGroup->addAction(ui.actionPointSet);
	meshRenderGroup->addAction(ui.actionWireFrame);
	meshRenderGroup->addAction(ui.actionHiddenLines);
	meshRenderGroup->addAction(ui.actionFlatLines);
	meshRenderGroup->addAction(ui.actionSolidFlat);
	meshRenderGroup->addAction(ui.actionSolidSmooth);

	QActionGroup *meshSelectionGroup = new QActionGroup(this);
	meshSelectionGroup->addAction(ui.actionPickVert);
	meshSelectionGroup->addAction(ui.actionPickEdge);
	meshSelectionGroup->addAction(ui.actionPickFace);
}

void MainWindow::CreateAction()
{
	connect(ui.actionOpen, SIGNAL(triggered()), meshViewer_, SLOT(loadMesh()));
	connect(ui.actionLoadTexture, SIGNAL(triggered()), meshViewer_, SLOT(loadTexture()));
	connect(ui.actionSave, SIGNAL(triggered()), meshViewer_, SLOT(saveMesh()));
	connect(ui.actionSaveScreen, SIGNAL(triggered()), meshViewer_, SLOT(saveScreen()));
	connect(ui.actionResetArcBall, SIGNAL(triggered()), meshViewer_, SLOT(resetArcBall()));

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionAbout, SIGNAL(triggered()), meshViewer_, SLOT(about()));

	//connect(meshViewer_, SIGNAL(operatorInfo(QString)), this->statusBar(), SLOT(showMessage(QString)));


	CreateActionSelection();
	CreateActionView();

	
	//connect(ui.radioButton_point, SIGNAL(clicked(bool)), renderingwidget_, SLOT(checkDrawPoint(bool)));
	//ui.radioButton_point->setChecked(true);

	//connect(ui.checkBox_point, SIGNAL(clicked(bool)), renderingwidget_, SLOT(checkDrawPoint(bool)));
	//ui.checkBox_point->setChecked(true);
	//connect(ui.checkBox_edge, SIGNAL(clicked(bool)), renderingwidget_, SLOT(checkDrawEdge(bool)));
//	connect(ui.checkBox_face, SIGNAL(clicked(bool)), renderingwidget_, SLOT(checkDrawFace(bool)));
//	connect(ui.checkBox_light, SIGNAL(clicked(bool)), renderingwidget_, SLOT(checkLight(bool)));
//	connect(ui.checkBox_texture, SIGNAL(clicked(bool)), renderingwidget_, SLOT(checkDrawTexture(bool)));

}

void MainWindow::CreateActionView()
{
	connect(meshViewer_, SIGNAL(setRenderMode(MeshRenderMode)), this, SLOT(setRenderMode(MeshRenderMode)));
	connect(ui.actionHidden, SIGNAL(triggered()), meshViewer_, SLOT(setMeshInvisible()));
	connect(ui.actionPointSet, SIGNAL(triggered()), meshViewer_, SLOT(setDrawMeshPointSet()));
	connect(ui.actionWireFrame, SIGNAL(triggered()), meshViewer_, SLOT(setDrawMeshWireFrame()));
	connect(ui.actionHiddenLines, SIGNAL(triggered()), meshViewer_, SLOT(setDrawMeshHiddenLines()));
	connect(ui.actionFlatLines, SIGNAL(triggered()), meshViewer_, SLOT(setDrawMeshFlatLines()));
	connect(ui.actionSolidFlat, SIGNAL(triggered()), meshViewer_, SLOT(setDrawMeshSolidFlat()));
	connect(ui.actionSolidSmooth, SIGNAL(triggered()), meshViewer_, SLOT(setDrawMeshSolidSmooth()));

	connect(ui.actionTexture, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawMeshTexture(bool)));
	connect(meshViewer_, SIGNAL(setDrawTextureMode(bool)), this->ui.actionTexture, SLOT(setChecked(bool)));

	connect(ui.actionAxis, SIGNAL(triggered()), meshViewer_, SLOT(setDrawAxes()));
	connect(ui.actionBBox, SIGNAL(triggered()), meshViewer_, SLOT(setDrawBBox()));
	connect(ui.actionBoundary, SIGNAL(triggered()), meshViewer_, SLOT(setDrawBoundary()));
	//connect(ui.actionTexture, SIGNAL(triggered()), meshViewer_, SLOT(setDrawTexture()));

	connect(ui.actionMeshInfo, SIGNAL(triggered()), meshViewer_, SLOT(showMeshInfo()));

}

void MainWindow::CreateActionSelection()
{
	connect(ui.actionResetSelection, SIGNAL(triggered()), meshViewer_, SLOT(setNoneSelectMode()));

	connect(ui.actionFindPoint, SIGNAL(triggered()), meshViewer_, SLOT(selectVertByPos()));
	connect(ui.actionFindVert, SIGNAL(triggered()), meshViewer_, SLOT(selectVertByIdx()));
	connect(ui.actionFindEdge, SIGNAL(triggered()), meshViewer_, SLOT(selectEdgeByIdx()));
	connect(ui.actionFindFace, SIGNAL(triggered()), meshViewer_, SLOT(selectFaceByIdx()));

	connect(ui.actionPickVert, SIGNAL(triggered()), meshViewer_, SLOT(setVertSelectMode()));
	connect(ui.actionPickEdge, SIGNAL(triggered()), meshViewer_, SLOT(setEdgeSelectMode()));
	connect(ui.actionPickFace, SIGNAL(triggered()), meshViewer_, SLOT(setFaceSelectMode()));
	connect(meshViewer_, SIGNAL(setSelectMode(bool)), ui.actionPickVert, SLOT(setChecked(bool)));
	connect(meshViewer_, SIGNAL(setSelectMode(bool)), ui.actionPickEdge, SLOT(setChecked(bool)));
	connect(meshViewer_, SIGNAL(setSelectMode(bool)), ui.actionPickFace, SLOT(setChecked(bool)));

	connect(ui.actionSaveSelectedVert, SIGNAL(triggered()), meshViewer_, SLOT(saveSelectedVert()));
	connect(ui.actionSaveSelectedEdge, SIGNAL(triggered()), meshViewer_, SLOT(saveSelectedEdge()));
	connect(ui.actionSaveSelectedFace, SIGNAL(triggered()), meshViewer_, SLOT(saveSelectedFace()));


	connect(ui.actionClearSelectedVert, SIGNAL(triggered()), meshViewer_, SLOT(clearVertSelection()));
	connect(ui.actionClearSelectedEdge, SIGNAL(triggered()), meshViewer_, SLOT(clearEdgeSelection()));
	connect(ui.actionClearSelectedFace, SIGNAL(triggered()), meshViewer_, SLOT(clearFaceSelection()));
	connect(ui.actionClearSelectedAll, SIGNAL(triggered()), meshViewer_, SLOT(clearAllSelection()));

	connect(ui.actionShowSelectedVert, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawSelectedVert(bool)));
	connect(ui.actionShowSelectedEdge, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawSelectedEdge(bool)));
	connect(ui.actionShowSelectedFace, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawSelectedFace(bool)));
	connect(ui.actionTagSelectedVert, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawSelectedVertTag(bool)));
	connect(ui.actionTagSelectedEdge, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawSelectedEdgeTag(bool)));
	connect(ui.actionTagSelectedFace, SIGNAL(triggered(bool)), meshViewer_, SLOT(setDrawSelectedFaceTag(bool)));

}

void MainWindow::CreateStatusBar()
{
	label_meshinfo_ = new QLabel(QString(" V%1 E%2 F%3 ").arg(0).arg(0).arg(0));
	label_meshinfo_->setAlignment(Qt::AlignCenter);
	label_meshinfo_->setMinimumSize(label_meshinfo_->sizeHint());

	label_operatorinfo_ = new QLabel();
	label_operatorinfo_->setAlignment(Qt::AlignVCenter);

	statusBar()->addWidget(label_meshinfo_);
	connect(meshViewer_, SIGNAL(meshInfo(int, int, int)), this, SLOT(showMeshInfo(int, int, int)));

	statusBar()->addWidget(label_operatorinfo_);
	connect(meshViewer_, SIGNAL(operatorInfo(QString)), label_operatorinfo_, SLOT(setText(QString)));
}

void MainWindow::setRenderMode(MeshRenderMode mode)
{
	switch (mode)
	{
	case MESH_HIDDEN:
		ui.actionHidden->setChecked(true);
		break;
	case MESH_POINT_SET:
		ui.actionPointSet->setChecked(true);
		break;
	case MESH_WIREFRAME:
		ui.actionWireFrame->setChecked(true);
		break;
	case MESH_HIDDEN_LINES:
		ui.actionHiddenLines->setChecked(true);
		break;
	case MESH_FLAT_LINES:
		ui.actionFlatLines->setChecked(true);
		break;
	case MESH_SOLID_FLAT:
		ui.actionSolidFlat->setChecked(true);
		break;
	case MESH_SOLID_SMOOTH:
		ui.actionSolidSmooth->setChecked(true);
		break;
	default:
		break;
	}
}

void MainWindow::showMeshInfo(int npoint, int nedge, int nface)
{
	label_meshinfo_->setText(QString(" V%1 E%2 F%3 ").arg(npoint).arg(nedge).arg(nface));
}
