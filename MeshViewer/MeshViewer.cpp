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

#include "mainwindow.h"
#include "ArcBall.h"
#include "ui_SaveSelection.h"

#define SWAP(a,b,T) {T tmp=(a); (a)=(b); (b)=tmp;}
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b

MeshViewer::MeshViewer(QWidget *parent, MainWindow* mainwindow)
	: QGLWidget(parent), ptr_mainwindow_(mainwindow)
{
	ptr_arcball_	= new CArcBall(width(), height());
	ptr_source_model_ = new Model;
	ptr_target_model_ = new Model;

	active_mesh_ = SOURCE_ACTIVE;
	mesh_select_mode_	= SELECT_NONE;
	mouse_mode_			= MOUSE_NAVIGATE_MESH;
	//mesh_render_mode_	= MESH_FLAT_LINES;
	//setFormat(QGLFormat(QGL::SampleBuffers));

	setAcceptDrops(true);
}

MeshViewer::~MeshViewer()
{
	SafeDelete(ptr_arcball_);
	SafeDelete(ptr_source_model_);
	SafeDelete(ptr_target_model_);
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
			pickMeshElem();
		}

		break;
	case Qt::MidButton:
		current_2d_position_ = e->pos();
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
			eye_goal_[0] -= 4.0*GLfloat(e->x() - current_2d_position_.x()) / GLfloat(width());
			eye_goal_[1] += 4.0*GLfloat(e->y() - current_2d_position_.y()) / GLfloat(height());
			current_2d_position_ = e->pos();
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
		bool flag = false;
		if (active_mesh_ == SOURCE_ACTIVE)
			flag = ptr_source_model_->loadMesh(fileName);
		else if (active_mesh_ == TARGET_ACTIVE)
			flag = ptr_target_model_->loadMesh(fileName);
		else
			flag = false;
		
	}
	else if (fileName.endsWith(".jpg") || fileName.endsWith(".png") || fileName.endsWith(".bmp"))
	{
		bool flag = false;
		if (active_mesh_ == SOURCE_ACTIVE)
			flag = ptr_source_model_->loadTexture(fileName);
		else if (active_mesh_ == TARGET_ACTIVE)
			flag = ptr_target_model_->loadTexture(fileName);
		else
			flag = false;
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


void MeshViewer::setDrawSourceMesh(bool bv)
{
	is_draw_source_mesh_ = bv;
	updateGL();
}
void MeshViewer::setDrawTargetMesh(bool bv)
{
	is_draw_target_mesh_ = bv;
	updateGL();
}

void MeshViewer::setMeshInvisible()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_HIDDEN);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_HIDDEN);
	updateGL();
}
void MeshViewer::setDrawMeshPointSet()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_POINT_SET);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_POINT_SET);
	updateGL();
}
void MeshViewer::setDrawMeshWireFrame()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_WIREFRAME);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_WIREFRAME);
	updateGL();
}
void MeshViewer::setDrawMeshHiddenLines()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_HIDDEN_LINES);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_HIDDEN_LINES);
	updateGL();
}
void MeshViewer::setDrawMeshFlatLines()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_FLAT_LINES);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_FLAT_LINES);
	updateGL();
}
void MeshViewer::setDrawMeshSolidFlat()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_SOLID_FLAT);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_SOLID_FLAT);
	updateGL();
}
void MeshViewer::setDrawMeshSolidSmooth()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setMeshRenderMode(MESH_SOLID_SMOOTH);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setMeshRenderMode(MESH_SOLID_SMOOTH);
	updateGL();
}

void MeshViewer::setDrawMeshTexture(bool bv)
{
	//if (!is_load_texture_)
	//{
	//	logW("no texture image loaded yet.\n");
	//	QMessageBox::information(this, tr("About"), "No texture image loaded yet.");
	//	emit(setDrawTextureMode(false));
	//	return;
	//}
	//is_draw_texture_ = bv;
	updateGL();
}

void MeshViewer::setDrawAxes()
{
	is_draw_axes_ = !is_draw_axes_;
	updateGL();
}

void MeshViewer::setColor()
{
	QColorDialog *palette = new QColorDialog(Qt::white, this);
	if (palette->exec() == QDialog::Accepted)
	{
		QColor color = palette->currentColor();
		OpenMesh::Vec3f c = OpenMesh::Vec3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
		if (active_mesh_ == SOURCE_ACTIVE)
		{
			ptr_source_model_->setColor(c);
			//emit(ptr_mainwindow_->setTargetLabelColor(color));
			emit(updateSourceLabelColor(color));
		}
		else if (active_mesh_ == TARGET_ACTIVE)
		{
			ptr_target_model_->setColor(c);
			emit(updateTargetLabelColor(color));
		}
	}
	updateGL();
}

void MeshViewer::setDrawBBox()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setDrawBBox();
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setDrawBBox();
	updateGL();
}
void MeshViewer::setDrawBoundary()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->setDrawBoundary();
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->setDrawBoundary();
	updateGL();
}

void MeshViewer::setSourceMeshActive()
{
	active_mesh_ = SOURCE_ACTIVE;
	emit(setRenderMode(ptr_source_model_->getMeshRenderMode()));
	emit(setDrawBBox(ptr_source_model_->isDrawBBox()));
	emit(setDrawBoundary(ptr_source_model_->isDrawBoundary()));
	emit(setDrawTextureMode(ptr_source_model_->isDrawTexture()));
	updateGL();
}

void MeshViewer::setTargetMeshActive()
{
	active_mesh_ = TARGET_ACTIVE;
	emit(setRenderMode(ptr_target_model_->getMeshRenderMode()));
	emit(setDrawBBox(ptr_target_model_->isDrawBBox()));
	emit(setDrawBoundary(ptr_target_model_->isDrawBoundary()));
	emit(setDrawTextureMode(ptr_target_model_->isDrawTexture()));
	updateGL();
}

void MeshViewer::swapMesh()
{
	Swap(ptr_source_model_, ptr_target_model_);
	emit(updateSourceLabelColor(ptr_source_model_->getColor()));
	emit(setDrawBBox(ptr_source_model_->isDrawBBox()));
	emit(setDrawBoundary(ptr_source_model_->isDrawBoundary()));
	emit(setDrawTextureMode(ptr_source_model_->isDrawTexture()));

	emit(setDrawBBox(ptr_target_model_->isDrawBBox()));
	emit(setDrawBoundary(ptr_target_model_->isDrawBoundary()));
	emit(updateTargetLabelColor(ptr_target_model_->getColor()));
	emit(setDrawTextureMode(ptr_target_model_->isDrawTexture()));
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
	if (active_mesh_ == SOURCE_ACTIVE)
	{
		info = QString("Source mesh\n");
		info += ptr_source_model_->getMeshInfo();
	}
	else if (active_mesh_ == TARGET_ACTIVE)
	{
		info = QString("Target mesh\n");
		info += ptr_target_model_->getMeshInfo();
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
	if (!validSelectMode()) return;

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
	if (!validSelectMode()) return;

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
	if (!validSelectMode()) return;

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
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->showSelectedVert(bv);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->showSelectedVert(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedEdge(bool bv)
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->showSelectedEdge(bv);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->showSelectedEdge(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedFace(bool bv)
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->showSelectedFace(bv);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->showSelectedFace(bv);
	updateGL();
}

void MeshViewer::setDrawSelectedVertTag(bool bv)
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->showSelectedVertTag(bv);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->showSelectedVertTag(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedEdgeTag(bool bv)
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->showSelectedEdgeTag(bv);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->showSelectedEdgeTag(bv);
	updateGL();
}
void MeshViewer::setDrawSelectedFaceTag(bool bv)
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->showSelectedFaceTag(bv);
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->showSelectedFaceTag(bv);
	updateGL();
}

void MeshViewer::selectVertByPos()
{
	if (!validSelectMode()) 
		return;

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
		if (active_mesh_ == SOURCE_ACTIVE)
			ptr_source_model_->selector()->findVertByPos(p);
		else if (active_mesh_ == TARGET_ACTIVE)
			ptr_target_model_->selector()->findVertByPos(p);
	}
	updateGL();
}
void MeshViewer::selectVertByIdx()
{
	if (!validSelectMode()) return;

	QDialog *window = new QDialog;
	window->setWindowTitle("Select Vertex");

	QSpinBox *spinBox = new QSpinBox();
	if (active_mesh_ == SOURCE_ACTIVE)
		spinBox->setRange(0, ptr_source_model_->vertNum());
	else if (active_mesh_ == TARGET_ACTIVE)
		spinBox->setRange(0, ptr_target_model_->vertNum());
	
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
		if (active_mesh_ == SOURCE_ACTIVE)
			ptr_source_model_->selector()->findVertByIdx(spinBox->value());
		else if (active_mesh_ == TARGET_ACTIVE)
			ptr_target_model_->selector()->findVertByIdx(spinBox->value());
	}
	updateGL();
}
void MeshViewer::selectEdgeByIdx()
{

	if (!validSelectMode()) return;


	QDialog *window = new QDialog;
	window->setWindowTitle("Select Edge");

	QLabel *label = new QLabel;
	label->setText("Edge Id");

	QSpinBox *spinBox = new QSpinBox();
	if (active_mesh_ == SOURCE_ACTIVE)
		spinBox->setRange(0, ptr_source_model_->edgeNum());
	else if (active_mesh_ == TARGET_ACTIVE)
		spinBox->setRange(0, ptr_target_model_->edgeNum());

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
		if (active_mesh_ == SOURCE_ACTIVE)
			ptr_source_model_->selector()->findEdgeByIdx(spinBox->value());
		else if (active_mesh_ == TARGET_ACTIVE)
			ptr_target_model_->selector()->findEdgeByIdx(spinBox->value());
	}
	updateGL();
}
void MeshViewer::selectFaceByIdx()
{
	if (!validSelectMode()) return;

	QDialog *window = new QDialog;
	window->setWindowTitle("Select Face");

	QSpinBox *spinBox = new QSpinBox();
	if (active_mesh_ == SOURCE_ACTIVE)
		spinBox->setRange(0, ptr_source_model_->faceNum());
	else if (active_mesh_ == TARGET_ACTIVE)
		spinBox->setRange(0, ptr_target_model_->faceNum());
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
		if (active_mesh_ == SOURCE_ACTIVE)
			ptr_source_model_->selector()->findFaceByIdx(spinBox->value());
		else if (active_mesh_ == TARGET_ACTIVE)
			ptr_target_model_->selector()->findFaceByIdx(spinBox->value());
	}
	updateGL();
}

void MeshViewer::clearAllSelection()
{
	if (active_mesh_ == SOURCE_ACTIVE)
	{
		ptr_source_model_->selector()->clearSelectedVert();
		ptr_source_model_->selector()->clearSelectedEdge();
		ptr_source_model_->selector()->clearSelectedFace();
	}
	else if (active_mesh_ == TARGET_ACTIVE)
	{
		ptr_target_model_->selector()->clearSelectedVert();
		ptr_target_model_->selector()->clearSelectedEdge();
		ptr_target_model_->selector()->clearSelectedFace();
	}
		
	updateGL();
}
void MeshViewer::clearVertSelection()
{
	if (active_mesh_ == SOURCE_ACTIVE)
	{
		ptr_source_model_->selector()->clearSelectedVert();
	}
	else if (active_mesh_ == TARGET_ACTIVE)
	{
		ptr_target_model_->selector()->clearSelectedVert();
	}
	updateGL();
}
void MeshViewer::clearEdgeSelection()
{
	if (active_mesh_ == SOURCE_ACTIVE)
	{
		ptr_source_model_->selector()->clearSelectedEdge();
	}
	else if (active_mesh_ == TARGET_ACTIVE)
	{
		ptr_target_model_->selector()->clearSelectedEdge();
	}
	updateGL();
}
void MeshViewer::clearFaceSelection()
{
	if (active_mesh_ == SOURCE_ACTIVE)
	{
		ptr_source_model_->selector()->clearSelectedFace();
	}
	else if (active_mesh_ == TARGET_ACTIVE)
	{
		ptr_target_model_->selector()->clearSelectedFace();
	}
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
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->saveSelectedVert("./selectedVert-src.txt");
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->saveSelectedVert("./selectedVert-tar.txt");

}
void MeshViewer::saveSelectedEdge()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->saveSelectedEdge("./selectedEdge-src.txt");
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->saveSelectedEdge("./selectedEdge-tar.txt");
}
void MeshViewer::saveSelectedFace()
{
	if (active_mesh_ == SOURCE_ACTIVE)
		ptr_source_model_->selector()->saveSelectedFace("./selectedFace-src.txt");
	else if (active_mesh_ == TARGET_ACTIVE)
		ptr_target_model_->selector()->saveSelectedFace("./selectedFace-tar.txt");
}

void MeshViewer::render()
{

	drawAxes();

	if (is_draw_source_mesh_)
	{
		ptr_source_model_->drawMesh();
		ptr_source_model_->drawSelection();
		if (active_mesh_ == SOURCE_ACTIVE) ptr_source_model_->drawSelectionTag(this);
		ptr_source_model_->drawOthers();
	}
	if (is_draw_target_mesh_)
	{
		ptr_target_model_->drawMesh();
		ptr_target_model_->drawSelection();
		if (active_mesh_ == TARGET_ACTIVE) ptr_target_model_->drawSelectionTag(this);
		ptr_target_model_->drawOthers();
	}

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

	//renderTextStr(len + 0.1, 0, 0, "X", 1.0, 0.0, 0.0, 16);
	//renderTextStr(0, len + 0.1, 0, "Y", 0.0, 1.0, 0.0, 16);
	//renderTextStr(0, 0, len + 0.1, "Z", 0.0, 0.0, 1.0, 16);
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


bool MeshViewer::validSelectMode()
{
	if (active_mesh_ == SOURCE_ACTIVE && ptr_source_model_->emptyMesh())
	{
		emit(setSelectMode(false));
		QMessageBox::warning(this, tr("Warning"), "Empty source mesh!");
		return false;
	}
	else if (active_mesh_ == TARGET_ACTIVE && ptr_target_model_->emptyMesh())
	{
		emit(setSelectMode(false));
		QMessageBox::warning(this, tr("Warning"), "Empty target mesh!");
		return false;
	}
	return true;
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

	bool flag = false;
	if (active_mesh_ == SOURCE_ACTIVE)
		flag = ptr_source_model_->loadMesh(filename);
	else if (active_mesh_ == TARGET_ACTIVE)
		flag = ptr_target_model_->loadMesh(filename);
	else
		flag = false;
	if (flag)
	{
		emit(operatorInfo(QString("Load mesh from ") + filename + QString(" Done")));
		//emit(meshInfo(ptr_mesh_->n_vertices(), ptr_mesh_->n_edges(), ptr_mesh_->n_faces()));
		if (active_mesh_ == SOURCE_ACTIVE)
		{
			emit(setRenderMode(ptr_source_model_->getMeshRenderMode()));
			emit(updateSourceLabelColor(ptr_source_model_->getColor()));
		}
		else if (active_mesh_ == TARGET_ACTIVE)
		{
			ptr_target_model_->setColor(OpenMesh::Vec3f(1.0, 0.0, 0.0));
			emit(setRenderMode(ptr_target_model_->getMeshRenderMode()));
			emit(updateTargetLabelColor(ptr_target_model_->getColor()));
		}
	}
	else
	{
		emit(operatorInfo(QString("Failed to load mesh from ") + filename));
		QMessageBox::warning(this, tr("Error"), QString("failed to load the mesh from %1").arg(filename));
	}
	updateGL();
}
void MeshViewer::loadTexture()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Load Texture"), 
		"../mesh/", 
		tr("Images(*.bmp *.jpg *.png *.jpeg)"));

	bool flag = false;
	if (active_mesh_ == SOURCE_ACTIVE)
		flag = ptr_source_model_->loadTexture(filename);
	else if (active_mesh_ == TARGET_ACTIVE)
		flag = ptr_target_model_->loadTexture(filename);
	else
		flag = false;
	if (flag)
	{
		emit(operatorInfo(QString("Load texture image from ") + filename + QString(" Done")));
	}
	else
	{
		emit(operatorInfo(QString("Failed to load texture image from ") + filename));
		QMessageBox::warning(this, tr("Error"), tr("failed to load texture image from %1").arg(filename.at(0)));
		return;
	}
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

	bool flag = false;
	if (active_mesh_ == SOURCE_ACTIVE)
		flag = ptr_source_model_->saveMesh(filename);
	else if (active_mesh_ == TARGET_ACTIVE)
		flag = ptr_target_model_->saveMesh(filename);
	else
		flag = false;
	if (flag)
	{
		emit(operatorInfo(QString("Save mesh to ") + filename + QString(" Done")));
	}
	else
	{
		emit(operatorInfo(QString("Failed to save mesh to ") + filename));
		QMessageBox::warning(this, tr("Error"), tr("failed to save mesh to %1").arg(filename.at(0)));
		return;
	}
	updateGL();
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
	logI("save screen to '%s'\n", filename.toStdString());
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
	current_3d_position_[0] = globalXYZ[0] * g2mMatrix[0] + globalXYZ[1] * g2mMatrix[1] + globalXYZ[2] * g2mMatrix[2];
	current_3d_position_[1] = globalXYZ[0] * g2mMatrix[4] + globalXYZ[1] * g2mMatrix[5] + globalXYZ[2] * g2mMatrix[6];
	current_3d_position_[2] = globalXYZ[0] * g2mMatrix[8] + globalXYZ[1] * g2mMatrix[9] + globalXYZ[2] * g2mMatrix[10];
	
}

void MeshViewer::pickMeshElem()
{
	switch (mesh_select_mode_)
	{
	case SELECT_NONE:
		break;
	case SELECT_VERTEX:
		if (active_mesh_ == SOURCE_ACTIVE)      ptr_source_model_->selector()->pickVert(current_3d_position_);
		else if (active_mesh_ == TARGET_ACTIVE) ptr_target_model_->selector()->pickVert(current_3d_position_);
		break;
	case SELECT_EDGE:
		if (active_mesh_ == SOURCE_ACTIVE)      ptr_source_model_->selector()->pickEdge(current_3d_position_);
		else if (active_mesh_ == TARGET_ACTIVE) ptr_target_model_->selector()->pickEdge(current_3d_position_);
		break;
	case SELECT_FACE:
		if (active_mesh_ == SOURCE_ACTIVE)      ptr_source_model_->selector()->pickFace(current_3d_position_);
		else if (active_mesh_ == TARGET_ACTIVE) ptr_target_model_->selector()->pickFace(current_3d_position_);
		break;
	default:
		break;
	}
}



