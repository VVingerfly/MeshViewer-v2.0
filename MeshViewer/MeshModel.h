/*! 
 * \file MeshModel.h
 * \brief   
 *    
 * Detailed explanation. 
 * \author Wei Li  GCL@USTC
 * \version 1.0
 * \date  2017/Dec/28 20:48:55
*/  
#pragma once
#include "Common.h"
#include "MeshSelector.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>


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


class Model
{
public:
	Model();
	~Model();


	//
	bool loadMesh(QString file);
	bool loadTexture(QString file);
	
	void initMesh();
	void updateMesh();
	void updateMeshNormal();
	void updateMeshBBox();
	void updateMeshCenter();
	void unifyMesh(float size);

	bool saveMesh(QString file);
	void printMeshInfo();

	MeshSelector* selector() { return ptr_selector_; }
	MyMesh*       meshPtr() { return ptr_mesh_; }

	// Rendering
	void drawMesh();
	void drawSelection();
	void drawSelectionTag(QGLWidget *widget);
	void drawOthers();

private:
	void drawMeshPointSet();
	void drawMeshWireFrame();
	void drawMeshHiddenLines();
	void drawMeshSolidFlat();
	void drawMeshSolidSmooth();

	void drawAxes();
	void drawBBox();
	void drawBoundary();
	void drawTexture();

public:
	bool emptyMesh();
	bool isDrawBBox()     { return is_draw_bbox_; }
	bool isDrawBoundary() { return is_draw_boundary_; }
	bool isDrawTexture()  { return is_draw_texture_; }
	
	int vertNum() { return ptr_mesh_->n_vertices(); }
	int edgeNum() { return ptr_mesh_->n_edges(); }
	int faceNum() { return ptr_mesh_->n_faces(); }

	
	void setDrawBBox() { is_draw_bbox_ = !is_draw_bbox_; }
	void setDrawBoundary() { is_draw_boundary_ = !is_draw_boundary_; }
	void setColor(const OpenMesh::Vec3f &c) { curr_color_ = c; }
	void setMeshRenderMode(MeshRenderMode mode) { mesh_render_mode_ = mode; }


	string  getMeshPath() { return mesh_path_; }
	string  getMeshName() { return mesh_name_; }
	string  getMeshFext() { return mesh_fext_; }
	QString getMeshInfo();
	QColor  getColor() { return QColor(curr_color_[0] * 255, curr_color_[1] * 255, curr_color_[2] * 255); }
	MeshRenderMode getMeshRenderMode() { return mesh_render_mode_; }


private:
	MyMesh                      *ptr_mesh_;
	MeshSelector                *ptr_selector_;

	OpenMesh::Vec3d				bbox_min_;    // min bbox corner
	OpenMesh::Vec3d				bbox_max_;    // max bbox corner
	OpenMesh::Vec3f             curr_color_;  // current color

	string						mesh_path_;   // mesh file path
	string						mesh_name_;   // mesh file name
	string						mesh_fext_;   // mesh file extension (begins with a dot(.)) (.obj, .stl, ...)

	MeshRenderMode				mesh_render_mode_ = MESH_WIREFRAME;

	// Texture
	GLuint						texture_[1];


	// Status info
	bool						is_load_mesh_		= false;
	bool						is_load_texture_	= false;

	bool						is_draw_axes_		= false;
	bool						is_draw_bbox_       = false;
	bool						is_draw_boundary_   = false;
	bool						is_draw_texture_    = false;
};

