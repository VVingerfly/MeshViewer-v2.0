//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/10 11:18:50
// Last Change  : 2017/Jan/10 11:18:54
// Summary      :
//===============================================================

#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include "Common.h"
#include "MeshModel.h"
#include "MeshSelector.h"
// Qt
#include <QEvent>
#include <QtOpenGL/QGLWidget>
#include <QtWidgets/QtWidgets>

class MainWindow;
class CArcBall;

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

enum CurrActiveMesh
{
    SOURCE_ACTIVE = 0,
    TARGET_ACTIVE
};

class MeshViewer : public QGLWidget
{
    Q_OBJECT

public:
    MeshViewer(QWidget* parent, MainWindow* mainwindow = 0);
    ~MeshViewer();

protected:
#if 0
	void paintEvent(QPaintEvent *event);
#else
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
#endif

    void timerEvent(QTimerEvent* e);

    // mouse events
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);

    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);


signals:
    void meshInfo(int, int, int);
    void operatorInfo(QString);
    void setSelectMode(bool bv);
    void setRenderMode(MeshRenderMode mode);
    void setDrawTextureMode(bool bv);
    void setDrawBBox(bool bv);
    void setDrawBoundary(bool bv);
    void updateSourceLabelColor(QColor c);
    void updateTargetLabelColor(QColor c);

public slots:
    void setBackground();
    void loadMesh();
    void loadTexture();
    void saveMesh();
    void saveScreen();

    void resetArcBall();
    void about();

    void setDrawSourceMesh(bool bv);
    void setDrawTargetMesh(bool bv);
    void setMeshInvisible();
    void setDrawMeshPointSet();
    void setDrawMeshWireFrame();
    void setDrawMeshHiddenLines();
    void setDrawMeshFlatLines();
    void setDrawMeshSolidFlat();
    void setDrawMeshSolidSmooth();
    void setDrawMeshTexture(bool bv);
    void setDrawBBox();
    void setDrawBoundary();

    void setDrawAxes();
    void setColor();

    void setSourceMeshActive();
    void setTargetMeshActive();
    void swapMesh();
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
        renderText(x, y, z, str, QFont("Courier", 10, QFont::Normal, false));  // QFont::Arial
    }
    void renderTextStr(double x, double y, double z, const QString& str, double c1, double c2, double c3, int sz = 10)
    {
        glColor3f(c1, c2, c3);
        renderText(x, y, z, str, QFont("Courier", sz, QFont::Normal, false));
    }

private:
    void render();
    void setDefaultLight();
    void setDefaultMaterial();
    void setDefaultViewAngle();

    void drawAxes();
    void drawLegend(QPainter* painter);

    void getSelectedPoint(double x, double y);
    void pickMeshElem();

    bool validSelectMode();
    void setSelectMode(MeshSelectMode mode) { mesh_select_mode_ = mode; }
    void setMouseMode(MouseActionMode mode)
    {
        if (getCurrentMouseMode() == MOUSE_SELECT_MODE && mode == MOUSE_NAVIGATE_MESH) {
            QApplication::setOverrideCursor(Qt::ArrowCursor);
            // QApplication::restoreOverrideCursor();
        }
        if (getCurrentMouseMode() == MOUSE_NAVIGATE_MESH && mode == MOUSE_SELECT_MODE) {
            QApplication::setOverrideCursor(Qt::PointingHandCursor);
        }
        mouse_mode_ = mode;
    }

    MeshSelectMode  getCurrentSelectMode() { return mesh_select_mode_; }
    MouseActionMode getCurrentMouseMode() { return mouse_mode_; }

public:
    MainWindow* ptr_mainwindow_;
    CArcBall*   ptr_arcball_;

    Model* ptr_source_model_;
    Model* ptr_target_model_;


    CurrActiveMesh active_mesh_;  // current active mesh

    MyMesh::Point current_3d_position_;  // current selected 3D point
    QPoint        current_2d_position_;  // current cursor position

    MeshSelectMode  mesh_select_mode_;
    MouseActionMode mouse_mode_;

    bool is_draw_axes_ = false;
    bool is_draw_source_mesh_ = true;
    bool is_draw_target_mesh_ = true;
    // eye
    GLfloat       eye_distance_;
    MyMesh::Point eye_goal_;
    MyMesh::Point eye_direction_;

#ifdef MESH_DEFORMER
    MeshDeformer* ptr_source_deformer_;
#endif  // MESH_DEFORMER
};

#endif  // RENDERINGWIDGET_H
