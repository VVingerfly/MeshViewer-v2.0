//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/10 11:18:50
// Last Change  : 2017/Jan/10 11:18:54
// Summary      :
//===============================================================

#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include "Common.h"
#include "MeshSelector.h"
// Qt
#include <QtWidgets/QtWidgets>
#include <QtOpenGL/QGLWidget>
#include <QEvent>


#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

class MainWindow;
class CArcBall;

enum MeshRenderMode
{
	MESH_HIDDEN = 0,
	MESH_POINT_SET,
	MESH_WIREFRAME,
	MESH_HIDDEN_LINES,
	MESH_FLAT_LINES,
	MESH_SOLID_FLAT,
	MESH_SOLID_SMOOTH
};

enum MeshSelectMode
{
	SELECT_NONE = 0,
	SELECT_VERTEX,
	SELECT_EDGE,
	SELECT_FACE
};

enum MouseActionMode
{
	MOUSE_NAVIGATE_MESH = 0,
	MOUSE_SELECT_MODE
};


class MeshViewer : public QGLWidget
{
	Q_OBJECT

public:
	MeshViewer(QWidget *parent, MainWindow* mainwindow=0);
	~MeshViewer();

protected:
	
#if 0
	void paintEvent(QPaintEvent *event);
#else
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
#endif
	
	void timerEvent(QTimerEvent *e);

	// mouse events
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

public:
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);

signals:
	void meshInfo(int, int, int);
	void operatorInfo(QString);
	void setSelectMode(bool bv);
	void setRenderMode(MeshRenderMode mode);
	void setDrawTextureMode(bool bv);

public slots:
	void setBackground();
	void loadMesh();
	void loadTexture();
	void saveMesh();
	void saveScreen();
	
	void resetArcBall();
	void about();

	void setMeshInvisible();
	void setDrawMeshPointSet();
	void setDrawMeshWireFrame();
	void setDrawMeshHiddenLines();
	void setDrawMeshFlatLines();
	void setDrawMeshSolidFlat();
	void setDrawMeshSolidSmooth();
	void setDrawMeshTexture(bool bv);
	void setDrawAxes();
	void setDrawBBox();
	void setDrawBoundary();


	void showMeshInfo();

	void setNoneSelectMode();
	void setVertSelectMode();
	void setEdgeSelectMode();
	void setFaceSelectMode();
	void setDrawSelectedVert(bool bv);
	void setDrawSelectedEdge(bool bv);
	void setDrawSelectedFace(bool bv);
	void setDrawSelectedVertTag(bool bv);
	void setDrawSelectedEdgeTag(bool bv);
	void setDrawSelectedFaceTag(bool bv);

	void selectVertByIdx();
	void selectVertByPos();
	void selectEdgeByIdx();
	void selectFaceByIdx();
	void clearAllSelection();
	void clearVertSelection();
	void clearEdgeSelection();
	void clearFaceSelection();

	void saveSelectedVert();
	void saveSelectedEdge();
	void saveSelectedFace();

private:

	void render();
	void setDefaultLight();
	void setDefaultMaterial();
	void setDefaultViewAngle();
public:
	void renderTextStr(int x, int y, const QString& str)
	{
		glColor3f(1.0, 0.0, 0.0);
		renderText(x, y, str, QFont("Courier", 10, QFont::Normal, false));
	}
	void renderTextStr(int x, int y, const QString& str, double c1, double c2, double c3, int sz = 10)
	{
		glColor3f(c1, c2, c3);
		renderText(x, y, str, QFont("Courier", sz, QFont::Normal, false));
	}
	void renderTextStr(double x, double y, double z, const QString& str)
	{
		glColor3f(1.0, 0.0, 0.0);
		renderText(x, y, z, str, QFont("Courier", 10, QFont::Normal, false));// QFont::Arial
	}
	void renderTextStr(double x, double y, double z, const QString& str, double c1, double c2, double c3, int sz = 10)
	{
		glColor3f(c1, c2, c3);
		renderText(x, y, z, str, QFont("Courier", sz, QFont::Normal, false));
	}

private:
	void drawMesh(int drawmode);
	void drawMeshPointSet();
	void drawMeshWireFrame();
	void drawMeshHiddenLines();
	void drawMeshSolidFlat();
	void drawMeshSolidSmooth();

	void drawAxes();
	void drawBBox();
	void drawBoundary();
	void drawTexture();

	void drawSelection();
	void drawSelectionTag();

	void drawLegend(QPainter *painter);
	//void drawLegend();

private:
	bool loadMesh(QString file);
	bool loadTexture(QString file);
	void initMesh();
	void updateMesh();
	void updateMeshNormal();
	void updateMeshBBox();
	void updateMeshCenter();
	void unifyMesh(float size);
	void printMeshInfo();
	void getSelectedPoint(double x, double y);

	MeshSelectMode getCurrentSelectMode() { return mesh_select_mode_; }
	void setSelectMode(MeshSelectMode mode) { mesh_select_mode_ = mode; }
	MouseActionMode getCurrentMouseMode() { return mouse_mode_; }
	void setMouseMode(MouseActionMode mode) 
	{ 
		if (getCurrentMouseMode() == MOUSE_SELECT_MODE && mode == MOUSE_NAVIGATE_MESH)
		{
			QApplication::setOverrideCursor(Qt::ArrowCursor);
			//QApplication::restoreOverrideCursor();
		}
		if (getCurrentMouseMode() == MOUSE_NAVIGATE_MESH && mode == MOUSE_SELECT_MODE)
		{
			QApplication::setOverrideCursor(Qt::PointingHandCursor);	
		}
		mouse_mode_ = mode; }
	MeshRenderMode getCurrentRenderMode() { return mesh_render_mode_; }
	void setMeshRenderMode(MeshRenderMode mode) { mesh_render_mode_ = mode; }

public:
	MainWindow					*ptr_mainwindow_;
	CArcBall					*ptr_arcball_;
	MyMesh                      *ptr_mesh_;
	MeshSelector                *ptr_selector_;

	OpenMesh::Vec3d				bbox_min_;
	OpenMesh::Vec3d				bbox_max_;
	MyMesh::Point				selected_point_;    // current selected 3D point
	QPoint						current_position_;  // current cursor position
	MeshRenderMode				mesh_render_mode_;
	MeshSelectMode				mesh_select_mode_;
	MouseActionMode             mouse_mode_;

	QString						mesh_path_;

	// Status info
	bool						is_load_mesh_;
	bool						is_load_texture_;
	// Render information
	bool						is_draw_axes_;
	bool						is_draw_bbox_;
	bool						is_draw_boundary_;
	bool						is_draw_texture_;

	// Texture
	GLuint						texture_[1];
	

	// eye
	GLfloat						eye_distance_;
	MyMesh::Point				eye_goal_;
	MyMesh::Point               eye_direction_;

	
};

#endif // RENDERINGWIDGET_H
