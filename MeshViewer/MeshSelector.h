/*! 
 * \file MeshSelector.h
 * \brief   
 *    
 * Detailed explanation. 
 * \author Wei Li  GCL@USTC
 * \version 1.0
 * \date  2017/Dec/1 13:26:47
*/  
#pragma once
#include "Common.h"
//#include "MeshViewer.h"
#include "ANN\ANN.h"
#include <QtOpenGL/QGLWidget>
#include <gl/GL.h>
#include <gl/GLU.h>

class MeshViewer;

class MeshSelector
{
public:
	MeshSelector();
	~MeshSelector();

	void setMesh(MyMesh *mesh) { ptr_mesh_ = mesh; }

	void drawSelectedVert();
	void drawSelectedEdge();
	void drawSelectedFace();
	void drawSelectedVertTag(QGLWidget *widget);
	void drawSelectedEdgeTag(QGLWidget *widget);
	void drawSelectedFaceTag(QGLWidget *widget);

	void clearSelectedVert() { selectedVert_.clear(); }
	void clearSelectedEdge() { selectedEdge_.clear(); }
	void clearSelectedFace() { selectedFace_.clear(); }
	void clearSelectedAll() { selectedVert_.clear(); selectedEdge_.clear(); selectedFace_.clear(); }

	void findVertByPos(const MyMesh::Point &p);
	void findVertByIdx(int vi);
	void findEdgeByIdx(int ei);
	void findFaceByIdx(int fi);

	void pickVert(const MyMesh::Point &p);
	void pickEdge(const MyMesh::Point &p);
	void pickFace(const MyMesh::Point &p);

	
	int findVertByPoint(const MyMesh::Point &p);
	int findEdgeByPoint(const MyMesh::Point &p);
	int findFaceByPoint(const MyMesh::Point &p);


	void showSelectedVert(bool bv) { is_draw_selected_vert_ = bv; }
	void showSelectedEdge(bool bv) { is_draw_selected_edge_ = bv; }
	void showSelectedFace(bool bv) { is_draw_selected_face_ = bv; }
	void showSelectedVertTag(bool bv) { is_tag_selected_vert_ = bv; }
	void showSelectedEdgeTag(bool bv) { is_tag_selected_edge_ = bv; }
	void showSelectedFaceTag(bool bv) { is_tag_selected_face_ = bv; }

	void saveSelectedVert(string savePath);
	void saveSelectedEdge(string savePath);
	void saveSelectedFace(string savePath);

	void buildSearchTree();
	int  findClosestVertFromPoint(const MyMesh::Point &p, double &dist);
private:
	ANNkd_tree     *kdTree;
	MyMesh         *ptr_mesh_;
	vector<int>		selectedVert_;
	vector<int>		selectedEdge_;
	vector<int>		selectedFace_;

	bool			is_draw_selected_vert_ = true;
	bool			is_draw_selected_edge_ = true;
	bool			is_draw_selected_face_ = true;
	bool			is_tag_selected_vert_ = false;
	bool			is_tag_selected_edge_ = false;
	bool			is_tag_selected_face_ = false;

	float			point_dist_thres_ = 10;
	int				tag_font_size_ = 10;
	int				vert_size_  = 12;
	float			edge_width_ = 4.0f;
	OpenMesh::Vec3f vert_color_ = OpenMesh::Vec3f(1.0, 0.5, 0.0);
	OpenMesh::Vec3f edge_color_ = OpenMesh::Vec3f(1.0, 0.5, 1.0);
	OpenMesh::Vec3f face_color_ = OpenMesh::Vec3f(1.0, 0.5, 0.5);
	OpenMesh::Vec3f vtag_color_ = OpenMesh::Vec3f(1.0, 0.0, 0.0); // vert_color_;
	OpenMesh::Vec3f etag_color_ = OpenMesh::Vec3f(1.0, 0.0, 0.0); // edge_color_;
	OpenMesh::Vec3f ftag_color_ = OpenMesh::Vec3f(1.0, 0.0, 0.0); // ftag_color_;
};

