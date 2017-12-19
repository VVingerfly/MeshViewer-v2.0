//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/10 11:22:03
// Last Change  : 2017/Jan/10 11:22:05
// Summary      :
//===============================================================

#include "MeshViewer.h"

#include <iostream>
#include <algorithm>

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QKeyEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QTextCodec>


#include "mainwindow.h"
#include "ArcBall.h"
#include "globalFunctions.h"
#include "ui_SaveSelection.h"

#define SWAP(a,b,T) {T tmp=(a); (a)=(b); (b)=tmp;}
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b

MeshViewer::MeshViewer(QWidget *parent, MainWindow* mainwindow)
	: QGLWidget(parent), ptr_mainwindow_(mainwindow),
	is_load_mesh_(false), is_load_texture_(false), 
	is_draw_axes_(false), is_draw_bbox_(false), is_draw_boundary_(false), is_draw_texture_(false)
{
	ptr_arcball_	= new CArcBall(width(), height());
	ptr_mesh_		= new MyMesh;
	ptr_selector_   = new MeshSelector;
	bbox_min_		= OpenMesh::Vec3d(-1.0, -1.0, -1.0);
	bbox_max_		= OpenMesh::Vec3d(1.0, 1.0, 1.0);


	

	mesh_select_mode_	= SELECT_NONE;
	mouse_mode_			= MOUSE_NAVIGATE_MESH;
	mesh_render_mode_	= MESH_FLAT_LINES;
	//setFormat(QGLFormat(QGL::SampleBuffers));

	setAcceptDrops(true);
}

MeshViewer::~MeshViewer()
{
	SafeDelete(ptr_arcball_);
	SafeDelete(ptr_mesh_);
}

#if 0

void MeshViewer::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	Render();
	drawLegend(&painter);
}

#else


void MeshViewer::initializeGL()
{

	// OpenGL state
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClearColor(0.196, 0.196, 0.196, 1.0);
	glDisable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	
	// Material
	setDefaultMaterial();
	// Lighting
	setDefaultLight();
	// Angle of View
	setDefaultViewAngle();
	
	// scene pos and size
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//logI("initialize OpenGL done.\n");
}

void MeshViewer::resizeGL(int w, int h)
{
	h = (h == 0) ? 1 : h;

	ptr_arcball_->reSetBound(w, h);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, GLdouble(w) / GLdouble(h), 0.001, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MeshViewer::paintGL()
{
	//glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	MyMesh::Point eyepos = eye_distance_*eye_direction_;

	gluLookAt(eyepos[0], eyepos[1], eyepos[2],
		eye_goal_[0], eye_goal_[1], eye_goal_[2],
		0.0, 1.0, 0.0);
	glPushMatrix();

	glMultMatrixf(ptr_arcball_->GetBallMatrix());
	
	render();

	
	//drawLegend();

	glPopMatrix();
}

#endif

void MeshViewer::timerEvent(QTimerEvent * e)
{
	updateGL();
}

void MeshViewer::mousePressEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		if (mouse_mode_ == MOUSE_NAVIGATE_MESH)
		{
			ptr_arcball_->MouseDown(e->pos());
		}
		else if (mouse_mode_ == MOUSE_SELECT_MODE)
		{
			getSelectedPoint(e->x(), e->y());
			switch (mesh_select_mode_)
			{
			case SELECT_NONE:
				break;
			case SELECT_VERTEX:
				ptr_selector_->pickVert(selected_point_);
				break;
			case SELECT_EDGE:
				ptr_selector_->pickEdge(selected_point_);
				break;
			case SELECT_FACE:
				ptr_selector_->pickFace(selected_point_);
				break;
			default:
				break;
			}
		}

		break;
	case Qt::MidButton:
		current_position_ = e->pos();
		break;
	default:
		break;
	}

	updateGL();
}
void MeshViewer::mouseMoveEvent(QMouseEvent *e)
{
	switch (e->buttons())
	{
		setCursor(Qt::ClosedHandCursor);
	case Qt::LeftButton:
		if (mouse_mode_ == MOUSE_NAVIGATE_MESH)
		{
			ptr_arcball_->MouseMove(e->pos());
		}
		
		break;
	case Qt::MidButton:
		if (mouse_mode_ == MOUSE_NAVIGATE_MESH)
		{
			eye_goal_[0] -= 4.0*GLfloat(e->x() - current_position_.x()) / GLfloat(width());
			eye_goal_[1] += 4.0*GLfloat(e->y() - current_position_.y()) / GLfloat(height());
			current_position_ = e->pos();
		}

		break;
	default:
		break;
	}

	updateGL();
}
void MeshViewer::mouseDoubleClickEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		break;
	default:
		break;
	}
	updateGL();
}
void MeshViewer::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		if (mouse_mode_ == MOUSE_NAVIGATE_MESH)
		{
			ptr_arcball_->MouseUp(e->pos());
			setCursor(Qt::ArrowCursor);
		}
		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
	updateGL();
}
void MeshViewer::wheelEvent(QWheelEvent *e)
{
	eye_distance_ += e->delta()*0.001;
	eye_distance_ = eye_distance_ < 0 ? 0 : eye_distance_;

	updateGL();
}
void MeshViewer::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
	updateGL();
}
void MeshViewer::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
	updateGL();
}

void MeshViewer::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
	updateGL();
}
void MeshViewer::dropEvent(QDropEvent* event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;
	QString fileName = urls.first().toLocalFile();
	if (fileName.isEmpty())
		return;

	if (fileName.endsWith(".off") || fileName.endsWith(".obj") || fileName.endsWith(".stl") || fileName.endsWith(".ply"))
	{
		loadMesh(fileName);
	}
	else if (fileName.endsWith(".jpg") || fileName.endsWith(".png") || fileName.endsWith(".bmp"))
	{
		loadTexture(fileName);
	}
	else
	{
		QMessageBox::warning(this, "Error", "Unsupported mesh or texture image file");
	}
	updateGL();
}
void MeshViewer::setDefaultLight()
{
#if 1
	// lighting
	GLfloat pos1[] = { 10.0f, 10.0f, -10.0f, 0.0f };
	GLfloat pos2[] = { -10.0f, 10.0f, -10.0f, 0.0f };
	GLfloat pos3[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	/*GLfloat col1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat col2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };*/
	GLfloat col1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat col2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, col2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, col2);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col3);
#else
	// lighting
	GLfloat pos3[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col3);
#endif

#if 0
	static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat mat_shininess[] = { 50.0 };
	static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	static GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
	static GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
#endif
}
void MeshViewer::setDefaultMaterial()
{
	// material
#if 1
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };

	/*GLfloat mat_a[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
	GLfloat mat_d[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
	GLfloat mat_s[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
	GLfloat shine[] = { 51.2f };*/
#else
	GLfloat mat_a[] = { 0.0, 0.5, 0.75, 1.0 };
	GLfloat mat_d[] = { 0.0, 0.5, 1.0, 1.0 };
	GLfloat mat_s[] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat emission[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat shine[] = { 120.0 };
#endif

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}

void MeshViewer::setDefaultViewAngle()
{
	eye_distance_ = 5.0;
	eye_goal_[0] = 0.0;
	eye_goal_[1] = 0.0;
	eye_goal_[2] = 1.0;
	eye_direction_[0] = 0.0;
	eye_direction_[1] = 0.0;
	eye_direction_[2] = 1.0;
}

void MeshViewer::setBackground()
{
	QColor color = QColorDialog::getColor(Qt::white, this, tr("background color"));
	GLfloat r = (color.red()) / 255.0f;
	GLfloat g = (color.green()) / 255.0f;
	GLfloat b = (color.blue()) / 255.0f;
	GLfloat alpha = color.alpha() / 255.0f;
	glClearColor(r, g, b, alpha);
	updateGL();
}

void MeshViewer::setMeshInvisible()
{
	mesh_render_mode_ = MESH_HIDDEN;
	updateGL();
}
void MeshViewer::setDrawMeshPointSet()
{
	mesh_render_mode_ = MESH_POINT_SET;
	updateGL();
}
void MeshViewer::setDrawMeshWireFrame()
{
	mesh_render_mode_ = MESH_WIREFRAME;
	updateGL();
}
void MeshViewer::setDrawMeshHiddenLines()
{
	mesh_render_mode_ = MESH_HIDDEN_LINES;
	updateGL();
}
void MeshViewer::setDrawMeshFlatLines()
{
	mesh_render_mode_ = MESH_FLAT_LINES;
	updateGL();
}
void MeshViewer::setDrawMeshSolidFlat()
{
	mesh_render_mode_ = MESH_SOLID_FLAT;
	updateGL();
}
void MeshViewer::setDrawMeshSolidSmooth()
{
	mesh_render_mode_ = MESH_SOLID_SMOOTH;
	updateGL();
}

void MeshViewer::setDrawMeshTexture(bool bv)
{
	if (!is_load_texture_)
	{
		logW("no texture image loaded yet.\n");
		QMessageBox::information(this, tr("About"), "No texture image loaded yet.");
		emit(setDrawTextureMode(false));
		return;
	}
	is_draw_texture_ = bv;
	updateGL();
}

void MeshViewer::setDrawAxes()
{
	is_draw_axes_ = !is_draw_axes_;
	updateGL();
}
void MeshViewer::setDrawBBox()
{
	is_draw_bbox_ = !is_draw_bbox_;
	updateGL();
}
void MeshViewer::setDrawBoundary()
{
	is_draw_boundary_ = !is_draw_boundary_;
	updateGL();
}


void MeshViewer::showMeshInfo()
{

	QImage *image = new QImage();
	vector<string> name;
	//name.push_back("meshPointSet.png");
	//name.push_back("meshWirFramee.png");
	//name.push_back("meshHiddenLines.png");
	//name.push_back("meshFlatLines.png");
	//name.push_back("meshSolidFlat.png");
	//name.push_back("meshSolidSmooth.png");
	//name.push_back("axis.png");
	//name.push_back("info.png");
	//name.push_back("meshBBox.png");
	//name.push_back("meshBoundary.png");
	//name.push_back("texture.png");
	//name.push_back("search.png");
	//name.push_back("cursor.png");
	//name.push_back("open.png");
	//name.push_back("save.png");
	//name.push_back("saveScreen.png");

	//name.push_back("clearSelectedEdges.png");
	//name.push_back("clearSelectedFaces.png");
	//name.push_back("clearSelectedVertices.png");
	//name.push_back("saveSelectedEdges.png");
	//name.push_back("saveSelectedFaces.png");
	//name.push_back("saveSelectedVertices.png");
	//name.push_back("saveSelection.png");
	//name.push_back("clearSelection.png");

	//for (int i = 0; i < name.size(); i++)
	//{
	//	image->load(QString::fromStdString("D:/myCloudDisk/OneDrive/MeshViewer/MeshViewer/Resources/images/" + name[i]));
	//	image->save(QString::fromStdString("D:/myCloudDisk/OneDrive/MeshViewer/MeshViewer/Resources/" + name[i]));
	//}



	QString info;
	if (is_load_mesh_)
	{
		info = QString(
			"Vertices  %1 \n"
			"Edges     %2 \n"
			"Faces     %3 \n"
			"Path      %4"
		).arg(ptr_mesh_->n_vertices()).arg(ptr_mesh_->n_edges()).arg(ptr_mesh_->n_faces())
			.arg(mesh_path_);
	}
	else
		info = QString("No mesh loaded yet");
	

	QMessageBox::information(this, tr("MeshInfo"), info);
}

void MeshViewer::setNoneSelectMode()
{
	setSelectMode(SELECT_NONE);
	setMouseMode(MOUSE_NAVIGATE_MESH);
	emit(setSelectMode(false));
	updateGL();
}
void MeshViewer::setVertSelectMode()
{
	if (!is_load_mesh_)
	{
		emit(setSelectMode(false));
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet!");
		return;
	}
	if (getCurrentSelectMode() == SELECT_VERTEX)
	{
		setNoneSelectMode();
	}
	else
	{
		setSelectMode(SELECT_VERTEX);
		setMouseMode(MOUSE_SELECT_MODE);
	}
		
	updateGL();
}
void MeshViewer::setEdgeSelectMode()
{
	if (!is_load_mesh_)
	{
		emit(setSelectMode(false));
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet!");
		return;
	}
	if (getCurrentSelectMode() == SELECT_EDGE)
	{
		setNoneSelectMode();
	}
	else
	{
		setSelectMode(SELECT_EDGE);
		setMouseMode(MOUSE_SELECT_MODE);
	}
	updateGL();
}
void MeshViewer::setFaceSelectMode()
{
	if (!is_load_mesh_)
	{
		emit(setSelectMode(false));
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet!");
		return;
	}
	if (getCurrentSelectMode() == SELECT_FACE)
	{
		setNoneSelectMode();
	}
	else
	{
		setSelectMode(SELECT_FACE);
		setMouseMode(MOUSE_SELECT_MODE);
	}
	updateGL();
}

void MeshViewer::setDrawSelectedVert(bool bv)
{
	ptr_selector_->showSelectedVert(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedEdge(bool bv)
{
	ptr_selector_->showSelectedEdge(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedFace(bool bv)
{
	ptr_selector_->showSelectedFace(bv);
	updateGL();
}

void MeshViewer::setDrawSelectedVertTag(bool bv)
{
	ptr_selector_->showSelectedVertTag(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedEdgeTag(bool bv)
{
	ptr_selector_->showSelectedEdgeTag(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedFaceTag(bool bv)
{
	ptr_selector_->showSelectedFaceTag(bv);
	updateGL();
}

void MeshViewer::selectVertByPos()
{
	if (!is_load_mesh_)
	{
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet");
		return;
	}

	QDialog *window = new QDialog;
	window->setWindowTitle("Select Closest Vertex");

	QLabel *label = new QLabel;
	label->setText("Position");
	QHBoxLayout *layoutCoord = new QHBoxLayout;
	layoutCoord->addWidget(label);

	QLineEdit *lineEditPos[3];
	for (int i = 0; i < 3; i++)
	{
		lineEditPos[i] = new QLineEdit(this);
		lineEditPos[i]->setFixedWidth(60);
		lineEditPos[i]->setAlignment(Qt::AlignCenter);
		lineEditPos[i]->setValidator(new QDoubleValidator(this));
		lineEditPos[i]->setText("0.0");
		layoutCoord->addWidget(lineEditPos[i]);
	}


	QDialogButtonBox *button = new QDialogButtonBox(window);
	button->addButton("Cancel", QDialogButtonBox::NoRole);
	button->addButton("OK", QDialogButtonBox::YesRole);


	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(layoutCoord);
	mainLayout->addWidget(button);

	window->setLayout(mainLayout);
	window->show();

	connect(button, SIGNAL(accepted()), window, SLOT(accept()));
	connect(button, SIGNAL(rejected()), window, SLOT(reject()));


	if (window->exec() == QDialog::Accepted)
	{
		MyMesh::Point p;
		for (int i = 0; i < 3; i++)
		{
			if ((lineEditPos[i]->text()).isEmpty())
			{
				p[i] = std::numeric_limits<double>::quiet_NaN();
			}
			else
				p[i] = lineEditPos[i]->text().toDouble();
		}
		ptr_selector_->findVertByPos(p);
	}
	updateGL();
}
void MeshViewer::selectVertByIdx()
{
	if (!is_load_mesh_)
	{
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet");
		return;
	}
	QDialog *window = new QDialog;
	window->setWindowTitle("Select Vertex");

	QSpinBox *spinBox = new QSpinBox();
	spinBox->setRange(0, ptr_mesh_->n_vertices());  // 范围
	spinBox->setSingleStep(1);  // 步长
	//spinBox->setSuffix("%");  // 前缀
	//spinBox->setSpecialValueText(tr("Automatic"));  // 特殊文本值
	spinBox->setValue(0);  // 当前值
	spinBox->setAlignment(Qt::AlignCenter);
	spinBox->setFixedWidth(120);
	QLabel *label = new QLabel;
	label->setText("Vertex Id");

	QDialogButtonBox *button = new QDialogButtonBox(window);
	button->addButton("Cancel", QDialogButtonBox::NoRole);
	button->addButton("OK", QDialogButtonBox::YesRole);

	QHBoxLayout *layoutTop = new QHBoxLayout;
	layoutTop->addWidget(label);
	layoutTop->addWidget(spinBox);
	QHBoxLayout *layoutBtm = new QHBoxLayout;
	layoutBtm->addWidget(button);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(layoutTop);
	layout->addLayout(layoutBtm);
	//layout->addWidget(button);
	window->setLayout(layout);
	window->show();

	connect(button, SIGNAL(accepted()), window, SLOT(accept()));
	connect(button, SIGNAL(rejected()), window, SLOT(reject()));

	
	if (window->exec() == QDialog::Accepted)
	{
		ptr_selector_->findVertByIdx(spinBox->value());
	}
	updateGL();
}
void MeshViewer::selectEdgeByIdx()
{
	if (!is_load_mesh_)
	{
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet");
		return;
	}

	QDialog *window = new QDialog;
	window->setWindowTitle("Select Edge");

	QLabel *label = new QLabel;
	label->setText("Edge Id");

	QSpinBox *spinBox = new QSpinBox();
	spinBox->setRange(0, ptr_mesh_->n_edges()); 
	spinBox->setSingleStep(1);
	spinBox->setValue(0); 
	spinBox->setAlignment(Qt::AlignCenter);

	QDialogButtonBox *button = new QDialogButtonBox(window);
	button->addButton("Cancel", QDialogButtonBox::NoRole);
	button->addButton("OK", QDialogButtonBox::YesRole);
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(spinBox);
	layout->addWidget(button);

	window->setLayout(layout);
	window->show();

	connect(button, SIGNAL(accepted()), window, SLOT(accept()));
	connect(button, SIGNAL(rejected()), window, SLOT(reject()));


	if (window->exec() == QDialog::Accepted)
	{
		ptr_selector_->findEdgeByIdx(spinBox->value());
	}
	updateGL();
}
void MeshViewer::selectFaceByIdx()
{
	if (!is_load_mesh_)
	{
		QMessageBox::warning(this, tr("Warning"), "No mesh loaded yet");
		return;
	}

	QDialog *window = new QDialog;
	window->setWindowTitle("Select Face");

	QSpinBox *spinBox = new QSpinBox();
	spinBox->setRange(0, ptr_mesh_->n_faces());
	spinBox->setSingleStep(1); 
	spinBox->setValue(0);
	spinBox->setAlignment(Qt::AlignCenter);
	QLabel *label = new QLabel;
	label->setText("Face Id");

	QDialogButtonBox *button = new QDialogButtonBox(window);
	button->addButton("Cancel", QDialogButtonBox::NoRole);
	button->addButton("OK", QDialogButtonBox::YesRole);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(spinBox);
	layout->addWidget(button);
	window->setLayout(layout);

	window->show();

	connect(button, SIGNAL(accepted()), window, SLOT(accept()));
	connect(button, SIGNAL(rejected()), window, SLOT(reject()));


	if (window->exec() == QDialog::Accepted)
	{
		ptr_selector_->findFaceByIdx(spinBox->value());
	}
	updateGL();
}

void MeshViewer::clearAllSelection()
{
	ptr_selector_->clearSelectedVert();
	ptr_selector_->clearSelectedEdge();
	ptr_selector_->clearSelectedFace();
	updateGL();
}
void MeshViewer::clearVertSelection()
{
	ptr_selector_->clearSelectedVert();
	updateGL();
}
void MeshViewer::clearEdgeSelection()
{
	ptr_selector_->clearSelectedEdge();
	updateGL();
}
void MeshViewer::clearFaceSelection()
{
	ptr_selector_->clearSelectedFace();
	updateGL();
}

void MeshViewer::saveSelectedVert()
{
	Ui::DialogSaveSelection saveSelection;
	QDialog *d = new QDialog;
	saveSelection.setupUi(d);
	d->show();
	saveSelection.textEditHeadLines->setText("Selected Vertices\n"
		"NUmber #v");
	saveSelection.lineEditData->setText("v #MV #SV #PX #PY #PZ");

	int res = d->exec();
	if (res == QDialog::Accepted)
	{
		QString headLine = saveSelection.textEditHeadLines->toPlainText();
		cout << headLine.toStdString() << endl;
		QString dataLine = saveSelection.lineEditData->text();
		cout << dataLine.toStdString() << endl;
	}
	if (res == QDialog::Rejected)
	{
		QMessageBox::information(this, "INFORMATION", "You clicked CANCEL button!");
	}

	ptr_selector_->saveSelectedVert("./selectedVert.txt");

}
void MeshViewer::saveSelectedEdge()
{
	ptr_selector_->saveSelectedEdge("./selectedEdge.txt");
}
void MeshViewer::saveSelectedFace()
{
	ptr_selector_->saveSelectedFace("./selectedFace.txt");
}

void MeshViewer::render()
{
	drawSelection();
	drawSelectionTag();

	drawBBox();
	drawBoundary();
	drawAxes();

	drawTexture();

	drawMesh(mesh_render_mode_);
}

void MeshViewer::drawMesh(int drawmode)
{
	if (ptr_mesh_->n_vertices() == 0) { return; }

	switch (drawmode)
	{
	case MESH_HIDDEN:
		break;
	case MESH_POINT_SET:
		glDisable(GL_LIGHTING);
		drawMeshPointSet();
		break;
	case MESH_WIREFRAME:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawMeshWireFrame();
		//draw_meshpointset();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case MESH_HIDDEN_LINES:
		glDisable(GL_LIGHTING);
		drawMeshHiddenLines();
		break;
	case MESH_SOLID_FLAT:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawMeshSolidFlat();
		//draw_meshpointset();
		glDisable(GL_LIGHTING);
		break;
	case MESH_FLAT_LINES:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawMeshSolidFlat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		//draw_meshpointset();
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawMeshWireFrame();
		//draw_meshpointset();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case MESH_SOLID_SMOOTH:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		drawMeshSolidSmooth();
		//draw_meshpointset();
		glDisable(GL_LIGHTING);
		break;
	default:
		break;
	}
}
void MeshViewer::drawMeshPointSet()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);

	glColor3f(0.0, 1.0, 1.0);
	glPointSize(8);
	MyMesh::VertexIter v_it = ptr_mesh_->vertices_begin();
	glBegin(GL_POINTS);
	for (v_it; v_it != ptr_mesh_->vertices_end(); ++v_it)
	{
		glVertex3dv(ptr_mesh_->point(v_it).data());
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void MeshViewer::drawMeshWireFrame()
{
	glLineWidth(1);
	//glColor3f(0.753, 0.753, 0.753);
	//glColor3f(0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.25);
	//if(meshMode() != TRIANGLE && meshMode() != QUAD)
	{
		MyMesh::ConstFaceIter fIt(ptr_mesh_->faces_begin()),
			fEnd(ptr_mesh_->faces_end());
		MyMesh::ConstFaceVertexIter fvIt;
		for (; fIt != fEnd; ++fIt)
		{
			//GL::glNormal(dualMesh.normal(f_it));
			fvIt = ptr_mesh_->cfv_iter(fIt);
			glBegin(GL_POLYGON);
			for (fvIt; fvIt; ++fvIt)
			{
				glVertex3dv(ptr_mesh_->point(fvIt).data());
			}
			glEnd();
		}
	}
}
void MeshViewer::drawMeshHiddenLines()
{
	MyMesh::ConstFaceIter f_it(ptr_mesh_->faces_begin());
	MyMesh::ConstFaceIter f_end(ptr_mesh_->faces_end());
	MyMesh::ConstFaceVertexIter fv_it;
	glLineWidth(2.0);
	glColor3f(0.0, 1.0, 1.0);

	glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);
	glBegin(GL_TRIANGLES);
	for (; f_it != f_end; ++f_it)
	{
		fv_it = ptr_mesh_->cfv_iter(f_it);
		for (fv_it; fv_it; ++fv_it)
		{
			glVertex3dv(&ptr_mesh_->point(fv_it)[0]);
		}

	}
	glEnd();


	glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	glBegin(GL_TRIANGLES);
	for (f_it = ptr_mesh_->faces_begin(); f_it != f_end; ++f_it)
	{
		fv_it = ptr_mesh_->cfv_iter(f_it);
		for (fv_it; fv_it; ++fv_it)
		{
			glVertex3dv(&ptr_mesh_->point(fv_it)[0]);
		}
	}
	glEnd();

	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void MeshViewer::drawMeshSolidFlat()
{
	MyMesh::ConstFaceIter fIt(ptr_mesh_->faces_begin()),
		fEnd(ptr_mesh_->faces_end());
	MyMesh::ConstFaceVertexIter fvIt;

	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	glBegin(GL_TRIANGLES);
	for (fIt; fIt != fEnd; ++fIt)
	{
		glNormal3dv(ptr_mesh_->normal(fIt).data());
		fvIt = ptr_mesh_->cfv_iter(fIt.handle());
		for (fvIt; fvIt; ++fvIt)
		{
			glVertex3dv(ptr_mesh_->point(fvIt).data());
		}
	}
	glEnd();
	//logI("draw mesh solid flat called\n");
}
void MeshViewer::drawMeshSolidSmooth()
{
	bool drawOK = false;
	glLoadName(ptr_mesh_->n_vertices());

	MyMesh::ConstFaceIter fIt(ptr_mesh_->faces_begin()),
		fEnd(ptr_mesh_->faces_end());
	MyMesh::ConstFaceVertexIter fvIt;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, ptr_mesh_->points());
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_DOUBLE, 0, ptr_mesh_->vertex_normals());

	for (; fIt != fEnd; ++fIt)
	{
		glBegin(GL_POLYGON);
		fvIt = ptr_mesh_->cfv_iter(fIt.handle());
		for (fvIt; fvIt; --fvIt)
		{
			glArrayElement(fvIt.handle().idx());
		}
		glEnd();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}


void MeshViewer::drawAxes()
{
	if (!is_draw_axes_)
		return;

	//x axis
	float len = 1.0;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(len, 0.0, 0.0);
	glEnd();
	glPushMatrix();
	glTranslatef(len, 0, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	//y axis
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, len, 0.0);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0, len, 0);
	glRotatef(90, -1.0, 0.0, 0.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	//z axis
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 0.0, len);
	glEnd();
	glPushMatrix();
	glTranslatef(0.0, 0, len);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	glColor3f(0.0, 0.0, 0.0);

	renderTextStr(len + 0.1, 0, 0, "X", 1.0, 0.0, 0.0, 16);
	renderTextStr(0, len + 0.1, 0, "Y", 0.0, 1.0, 0.0, 16);
	renderTextStr(0, 0, len + 0.1, "Z", 0.0, 0.0, 1.0, 16);
}
void MeshViewer::drawBBox()
{
	if (!is_draw_bbox_) return;

	OpenMesh::Vec3d temp0 = bbox_min_;
	OpenMesh::Vec3d temp1;
	glLineWidth(2.0);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	temp1 = bbox_min_; temp1[0] = bbox_max_[0];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());
	temp1 = bbox_min_; temp1[1] = bbox_max_[1];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());
	temp1 = bbox_min_; temp1[2] = bbox_max_[2];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_min_; temp0[0] = bbox_max_[0];
	temp1 = bbox_max_; temp1[1] = bbox_min_[1];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_min_; temp0[0] = bbox_max_[0];
	temp1 = bbox_max_; temp1[2] = bbox_min_[2];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_min_; temp0[1] = bbox_max_[1];
	temp1 = bbox_max_; temp1[2] = bbox_min_[2];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_min_; temp0[1] = bbox_max_[1];
	temp1 = bbox_max_; temp1[0] = bbox_min_[0];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_min_; temp0[2] = bbox_max_[2];
	temp1 = bbox_max_; temp1[1] = bbox_min_[1];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_min_; temp0[2] = bbox_max_[2];
	temp1 = bbox_max_; temp1[0] = bbox_min_[0];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());

	temp0 = bbox_max_;
	temp1 = bbox_max_; temp1[0] = bbox_min_[0];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());
	temp1 = bbox_max_; temp1[1] = bbox_min_[1];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());
	temp1 = bbox_max_; temp1[2] = bbox_min_[2];
	glVertex3dv(temp0.data());
	glVertex3dv(temp1.data());
	glEnd();


	int prec = 4;
	int width = 4;
	QString texMin  = QString("min:  %1  %2  %3").arg(double(bbox_min_[0]), width, 'g', prec, QChar(' ')).arg(double(bbox_min_[1]), width, 'g', prec, QChar(' ')).arg(double(bbox_min_[2]), width, 'g', prec, QChar(' '));
	QString texMax  = QString("max:  %1  %2  %3").arg(double(bbox_max_[0]), width, 'g', prec, QChar(' ')).arg(double(bbox_max_[1]), width, 'g', prec, QChar(' ')).arg(double(bbox_max_[2]), width, 'g', prec, QChar(' '));
	QString texSize = QString("size: %1  %2  %3").arg(double(bbox_max_[0] - bbox_min_[0]), width, 'g', prec, QChar(' ')).arg(double(bbox_max_[1] - bbox_min_[1]), width, 'g', prec, QChar(' ')).arg(double(bbox_max_[2] - bbox_min_[2]), width, 'g', prec, QChar(' '));
	renderTextStr(10, 20, texMin);
	renderTextStr(10, 35, texMax);
	renderTextStr(10, 50, texSize);
}
void MeshViewer::drawBoundary()
{
	if (!is_draw_boundary_) return;

	glLineWidth(4.0);
	glColor3f(1.0, 0.5, 0.0);
	glBegin(GL_LINES);
	for (MyMesh::EdgeIter e_it = ptr_mesh_->edges_begin(); e_it != ptr_mesh_->edges_end(); ++e_it)
	{
		if (ptr_mesh_->is_boundary(e_it))
		{
			MyMesh::HalfedgeHandle heh0 = ptr_mesh_->halfedge_handle(e_it, 0);
			glVertex3dv(ptr_mesh_->point(ptr_mesh_->to_vertex_handle(heh0)).data());
			glVertex3dv(ptr_mesh_->point(ptr_mesh_->from_vertex_handle(heh0)).data());
		}
	}
	glEnd();
}
void MeshViewer::drawTexture()
{
	if (!is_draw_texture_)
		return;
	//if (ptr_mesh_->n_faces() == 0 || !is_load_texture_) return;
#if 0
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
#endif

	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	
	glBegin(GL_TRIANGLES);
	for (auto face = ptr_mesh_->faces_begin(); face != ptr_mesh_->faces_end(); ++face)
	{
		for (auto it = ptr_mesh_->fv_begin(face); it != ptr_mesh_->fv_end(face); ++it)
		{
			auto vertex = it.handle();
			OpenMesh::Vec2f tex = ptr_mesh_->texcoord2D(vertex);
			glTexCoord2f(tex[0], tex[1]);
			MyMesh::Normal normal = ptr_mesh_->normal(vertex);
			glNormal3dv(normal.data());
			auto point = ptr_mesh_->point(it.handle());
			glVertex3dv(point.data());
			//printf("tex %d = (%f, %f)\n", vertex.idx(), tex[0], tex[1]);
		}
	}
	glEnd();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	
}

void MeshViewer::drawSelection()
{
	ptr_selector_->drawSelectedVert();
	ptr_selector_->drawSelectedEdge();
	ptr_selector_->drawSelectedFace();
}
void MeshViewer::drawSelectionTag()
{
	ptr_selector_->drawSelectedVertTag(this);
	ptr_selector_->drawSelectedEdgeTag(this);
	ptr_selector_->drawSelectedFaceTag(this);
}
void MeshViewer::drawLegend(QPainter *painter)
//void RenderingWidget::drawLegend()
{
	//QPainter *painter = new QPainter;
	const int Margin = 11;
	const int Padding = 6;

	QTextDocument textDocument;
	textDocument.setDefaultStyleSheet("* { color: #FFEFEF }");
	textDocument.setHtml("<h4 align=\"center\">Vowel Categories</h4>"
		"<p align=\"center\"><table width=\"100%\">"
		"<tr><td>Open:<td>a<td>e<td>o<td>&ouml;"
		"<tr><td>Close:<td>&#305;<td>i<td>u<td>&uuml;"
		"<tr><td>Front:<td>e<td>i<td>&ouml;<td>&uuml;"
		"<tr><td>Back:<td>a<td>&#305;<td>o<td>u"
		"<tr><td>Round:<td>o<td>&ouml;<td>u<td>&uuml;"
		"<tr><td>Unround:<td>a<td>e<td>&#305;<td>i"
		"</table>");
	textDocument.setTextWidth(textDocument.size().width());

	QRect rect(QPoint(0, 0), textDocument.size().toSize()
		+ QSize(2 * Padding, 2 * Padding));
	painter->translate(width() - rect.width() - Margin,
		height() - rect.height() - Margin);
	painter->setPen(QColor(255, 239, 239));
	painter->setBrush(QColor(255, 0, 0, 31));
	painter->drawRect(rect);

	painter->translate(Padding, Padding);
	textDocument.drawContents(painter);
}


void MeshViewer::loadMesh()
{
	//QString filename = QFileDialog::getOpenFileName(this, tr("Read Mesh"), "..", tr("Meshes (*.obj)"));
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open mesh file"),
		tr("../mesh/"),
		tr("OBJ Files (*.obj);;"
			"OFF Files (*.off);;"
			"PLY Files (*.ply);;"
			"STL Files (*.stl);;"
			"All Files (*)"));

	if (filename.isEmpty())
	{
		emit(operatorInfo(QString("Failed to load mesh from ") + filename));
		logE("failed to load mesh from '%s'\n", filename.toStdString());
		return;
	}
	loadMesh(filename);
	updateGL();
}
void MeshViewer::loadTexture()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Load Texture"), 
		"../mesh/", 
		tr("Images(*.bmp *.jpg *.png *.jpeg)"));
	if (filename.isEmpty())
	{
		emit(operatorInfo(QString("Failed to load texture image from ") + filename));
		logE("failed to load texture image from '%s'\n", filename.toStdString());
		return;
	}
	loadTexture(filename);
	updateGL();
}
void MeshViewer::saveMesh()
{
	//QString filename = QFileDialog::getSaveFileName(this, tr("Write Mesh"), "..", tr("Meshes (*.obj)"));
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save mesh file"),
		tr("../mesh/untitled.obj"),
		tr("OBJ Files (*.obj);;"
			"OFF Files (*.off);;"
			"PLY Files (*.ply);;"
			"STL Files (*.stl);;"
			"All Files (*)"));

	OpenMesh::IO::Options opt_write = OpenMesh::IO::Options::Default;
	//opt_write += OpenMesh::IO::Options::VertexNormal;
	//opt_write += OpenMesh::IO::Options::VertexTexCoord;
	if (filename.isEmpty() || !OpenMesh::IO::write_mesh(*ptr_mesh_, filename.toStdString(), opt_write, 6))
	{
		emit(operatorInfo(QString("Failed to save mesh to ") + filename));
		logE("failed to save current mesh to '%s'\n", filename.toStdString());
		return;
	}
	emit(operatorInfo(QString("Save mesh to ") + filename + QString(" Done")));
	logE("save current mesh to '%s'\n", filename.toStdString());
}
void MeshViewer::saveScreen()
{
	QString filename = QFileDialog::getSaveFileName(this,
		("Save screen as image file"),
		("../mesh/untitled.png"),
		("PNG Files (*.png);;BMP Files (*.bmp);;JPG Files (*.jpg);;"
			"All Files (*)"));
	QImage image = grabFrameBuffer();
	if (filename.isEmpty() || !image.save(filename))
	{
		emit(operatorInfo(QString("Failed to save screen to ") + filename));
		logE("failed to save screen to '%s'\n", filename.toStdString());
	}
	emit(operatorInfo(QString("Save screen to ") + filename + QString(" Done")));
	logE("save screen to '%s'\n", filename.toStdString());
}


void MeshViewer::resetArcBall()
{
	ptr_arcball_->InitBall();
	setDefaultViewAngle();
	updateGL();
}

void MeshViewer::about()
{
	QString info = 
		"A simple triangle mesh processing framework!\n"
		"If you have any problem, please contact with Wei Li (wei587me@163.com)";
	QMessageBox::information(this, tr("About"), info);
}

bool MeshViewer::loadMesh(QString filename)
{
	// Support for path containing Chinese characters
	QTextCodec *code = QTextCodec::codecForName("gd18030");
	QTextCodec::setCodecForLocale(code);
	QByteArray byfilename = filename.toLocal8Bit();
	OpenMesh::IO::Options opt_read = OpenMesh::IO::Options::Default;
	opt_read += OpenMesh::IO::Options::VertexNormal;    ptr_mesh_->request_vertex_normals();
	opt_read += OpenMesh::IO::Options::VertexTexCoord;  ptr_mesh_->request_vertex_texcoords2D();
	ptr_mesh_->clear();
	if (!OpenMesh::IO::read_mesh(*ptr_mesh_, byfilename.data(), opt_read))
	{
		emit(operatorInfo(QString("Failed to load mesh from ") + filename));
		logE("failed to load mesh from '%s'\n", filename.toStdString());
		QMessageBox::warning(this, tr("Error"), QString("failed to load the mesh from %1").arg(filename));
		return false;
	}
	is_load_mesh_ = true;
	mesh_path_ = filename;
	initMesh();

	ptr_selector_->setMesh(ptr_mesh_);
	ptr_selector_->buildSearchTree();

	emit(operatorInfo(QString("Load mesh from ") + filename + QString(" Done")));
	emit(meshInfo(ptr_mesh_->n_vertices(), ptr_mesh_->n_edges(), ptr_mesh_->n_faces()));
	emit(setRenderMode(mesh_render_mode_));
	bool statusVN = opt_read.check(OpenMesh::IO::Options::VertexNormal);
	bool statusVT = opt_read.check(OpenMesh::IO::Options::VertexTexCoord);
	logI("succeed in loading a mesh with %s%s\n", (statusVN ? "[vertex normals]" : ""), (statusVT ? "[vertex uv coordinates]" : ""));
	return true;
}
bool MeshViewer::loadTexture(QString filename)
{
	QImage tex, buf;
	if (!buf.load(filename))
	{
		emit(operatorInfo(QString("Failed to load mesh from ") + filename));
		logE("failed to load texture image from '%s'\n", filename.toStdString());
		QMessageBox::warning(this, tr("Error"), tr("failed to load texture image from %1").arg(filename.at(0)));
		return false;
	}
#if 0
	QImage dummy(128, 128, QImage::Format_ARGB32);
	dummy.fill(Qt::green);
	buf = dummy;
#endif
	if (is_load_texture_)
	{
		glDeleteTextures(1, &texture_[0]);
	}
	tex = QGLWidget::convertToGLFormat(buf);
	//创建纹理  
	glGenTextures(1, &texture_[0]);
	//使用来自位图数据生成的典型纹理，将纹理名字texture[0]绑定到纹理目标上  
	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	is_load_texture_ = true;
	emit(operatorInfo(QString("Load texture from ") + filename + QString(" Done")));
	logI("succeed in loading a texture image from '%s'\n", filename.toStdString());
	return true;
}
void MeshViewer::initMesh()
{
	ptr_mesh_->request_vertex_status();
	ptr_mesh_->request_edge_status();
	ptr_mesh_->request_face_status();

	ptr_mesh_->request_face_normals();
	ptr_mesh_->request_vertex_normals();

	updateMeshBBox();
	unifyMesh(2.f);

	//updateMesh();
	updateMeshNormal();
	updateMeshBBox();
}

void MeshViewer::unifyMesh(float size)
{
	float scaleX = bbox_max_[0] - bbox_min_[0];
	float scaleY = bbox_max_[1] - bbox_min_[1];
	float scaleZ = bbox_max_[2] - bbox_min_[2];

	float scaleMax;

	if (scaleX < scaleY)
	{
		scaleMax = scaleY;
	}
	else
	{
		scaleMax = scaleX;
	}
	if (scaleMax < scaleZ)
	{
		scaleMax = scaleZ;
	}
	float scaleV = size / scaleMax;
	MyMesh::Point centerPos((bbox_max_[0] + bbox_min_[0]) / 2.f, (bbox_max_[1] + bbox_min_[1]) / 2.f, (bbox_max_[2] + bbox_min_[2]) / 2.f);

	for (auto it = ptr_mesh_->vertices_begin(); it != ptr_mesh_->vertices_end(); ++it)
	{
		auto vertex = it.handle();
		ptr_mesh_->point(vertex) = (ptr_mesh_->point(vertex) - centerPos) * scaleV;

	}
}

void MeshViewer::printMeshInfo()
{
	printf("BoundingBox:\nX : [ %f , %f ]\n", bbox_min_[0], bbox_max_[0]);
	printf("Y : [ %f , %f ]\n", bbox_min_[1], bbox_max_[1]);
	printf("Z : [ %f , %f ]\n", bbox_min_[2], bbox_max_[2]);
	printf("Diag length of BBox : %f\n", (bbox_max_ - bbox_min_).norm());
	
}

void MeshViewer::getSelectedPoint(double x, double y)
{
	//printf("2D point [%f, %f]\n", x, y);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble winX = double(x);
	//GLdouble winY = double(height() - y);
	GLdouble winY = double(viewport[3] - y);
	GLfloat winZ = 0.0;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	GLdouble ProjectionMatrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, ProjectionMatrix);
	GLdouble ModelViewMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMatrix);
	GLdouble globalXYZ[3];
	gluUnProject(winX, winY, (GLdouble)winZ, ModelViewMatrix, ProjectionMatrix, viewport, &globalXYZ[0], &globalXYZ[1], &globalXYZ[2]);
	//printf("viewport cmp %f vs %f\n", height(), viewport[3]);
	//printf("3D point [%f, %f, %f]\n", selectedPoint_[0], selectedPoint_[1], selectedPoint_[2]);

	//ptr_arcball_->printBallMatrix();
	float* g2mMatrix = ptr_arcball_->GetBallMatrix();  // global2modelMatrix
	selected_point_[0] = globalXYZ[0] * g2mMatrix[0] + globalXYZ[1] * g2mMatrix[1] + globalXYZ[2] * g2mMatrix[2];
	selected_point_[1] = globalXYZ[0] * g2mMatrix[4] + globalXYZ[1] * g2mMatrix[5] + globalXYZ[2] * g2mMatrix[6];
	selected_point_[2] = globalXYZ[0] * g2mMatrix[8] + globalXYZ[1] * g2mMatrix[9] + globalXYZ[2] * g2mMatrix[10];
	
}

void MeshViewer::updateMesh()
{
	updateMeshNormal();
	updateMeshBBox();
	updateMeshCenter();
}

void MeshViewer::updateMeshNormal()
{
	ptr_mesh_->update_face_normals();
	ptr_mesh_->update_vertex_normals();
}

void MeshViewer::updateMeshBBox()
{
	typedef MyMesh::Point Point;
	MyMesh::VertexIter vIt = ptr_mesh_->vertices_begin();
	MyMesh::VertexIter vEnd = ptr_mesh_->vertices_end();
	bbox_min_ = bbox_max_ = OpenMesh::vector_cast<OpenMesh::Vec3d>(ptr_mesh_->point(vIt));

	size_t count = 0;
	for (; vIt != vEnd; ++vIt, ++count)
	{
		bbox_min_.minimize(OpenMesh::vector_cast<OpenMesh::Vec3d>(ptr_mesh_->point(vIt)));
		bbox_max_.maximize(OpenMesh::vector_cast<OpenMesh::Vec3d>(ptr_mesh_->point(vIt)));
	}
}

void MeshViewer::updateMeshCenter()
{
	MyMesh::EdgeIter e_it = ptr_mesh_->edges_begin();
	MyMesh::EdgeIter e_end = ptr_mesh_->edges_end();
	double aveLen = 0.0;   // average edge length
	double maxLen = 0.0;   // max edge length
	double minLen = ptr_mesh_->calc_edge_length(e_it);  // min edge length
	double e_len = 0.0;
	for (; e_it != e_end; ++e_it)
	{
		double e_len = ptr_mesh_->calc_edge_length(e_it);
		if (e_len > maxLen)
		{
			maxLen = e_len;
		}
		else if (e_len < minLen)
		{
			minLen = e_len;
		}
		aveLen += e_len;
	}
	printf("Edge Length : Max : %f; Min : %f; AVG : %f\n", maxLen, minLen, aveLen / ptr_mesh_->n_edges());
	printMeshInfo();
}

