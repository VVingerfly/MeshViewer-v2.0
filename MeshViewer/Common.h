//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/8 20:29:44
// Last Change  : 2017/Jan/8 20:29:47
// Summary      :
//===============================================================

#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::string;
using std::cout;
using std::endl;

/// ***************************** STL ************************************
#include <vector>
#include <list>
using std::vector;
using std::list;

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

/// ***************************** OpenMesh ************************************
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Handles.hh>
#include <OpenMesh/Core/Mesh/Traits.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#ifdef _DEBUG
#pragma comment(lib, "OpenMeshCored.lib")
#pragma comment(lib, "OpenMeshToolsd.lib")
#else
#pragma comment(lib, "OpenMeshCore.lib")
#pragma comment(lib, "OpenMeshTools.lib")
#endif // DEBUG


struct TriTraits : public OpenMesh::DefaultTraits
{
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
	typedef OpenMesh::Vec4f Color;
};
typedef OpenMesh::TriMesh_ArrayKernelT<TriTraits>    MyMesh;
typedef MyMesh::VertexHandle   VHandle;
typedef MyMesh::EdgeHandle     EHandle;
typedef MyMesh::HalfedgeHandle HHandle;
typedef MyMesh::FaceHandle     FHandle;
typedef MyMesh::VertexIter     VIter;
typedef MyMesh::EdgeIter     EIter;
typedef MyMesh::FaceIter     FIter;

/// ***************************** Eigen ************************************
#include <Eigen/Dense>
#include <Eigen/Sparse>
typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;


/// ***************************** macro ************************************
#ifndef IS_ZERO
#define IS_ZERO 0.00000001
#endif
#ifndef INF
#define INF     10000000
#endif
#ifndef PI
#define PI 3.141592653
#endif
//#define MYDEBUG

#ifdef MYDEBUG
#define DbgPrintf printf
#else
#define DbgPrintf /\
/DbgPrintf
#endif


#define logInfo
#ifdef logInfo
#define logI (printf("--info-- in [%d@%s] ",__LINE__, __FUNCTION__), printf)  
#else
#define logI /\
/logI
#endif

#define logW (printf("--warn-- in [%d@%s] ",__LINE__, __FUNCTION__), printf)  
#define logE (printf("--error- in [%d@%s] ",__LINE__, __FUNCTION__), printf)  






inline double triangleArea(const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, const Eigen::Vector3d &p3)
{
	Eigen::Vector3d p1p2 = p2 - p1;
	Eigen::Vector3d p1p3 = p3 - p1;
	return 0.5 *(p1p2.cross(p1p3).norm());
}


void save2DTexMesh(const MyMesh *mesh, string file);


template< typename MeshT >
MeshT* meshDeepCopy(const MeshT* mesh)
{
	MeshT *mesh_copy = new MeshT();
	vector<MeshT::VertexHandle>  vHandleVec;
	for (auto vit = mesh->vertices_begin(); vit != mesh->vertices_end(); vit++)
	{
		auto p = mesh->point(vit.handle());
		vHandleVec.push_back(mesh_copy->add_vertex(p));
	}
	vector<MeshT::VertexHandle>  face_vhandles;
	for (auto fit = mesh->faces_begin(); fit != mesh->faces_end(); fit++)
	{
		auto fh = fit.handle();
		face_vhandles.clear();
		for (auto fvit = mesh->cfv_begin(fh); fvit != mesh->cfv_end(fh); fvit++)
		{
			auto vi = fvit.handle().idx();
			face_vhandles.push_back(vHandleVec.at(vi));
		}
		mesh_copy->add_face(face_vhandles);
	}
	return mesh_copy;
}

void meshReader(std::string meshFile);

template< typename MeshT >
void meshWriter(const MeshT* mesh, string file, double scale)
{
	ofstream outMesh;
	outMesh = ofstream(file);
	outMesh << "# vertices " << mesh->n_vertices() << endl;
	outMesh << "# faces  " << mesh->n_faces() << endl;
	outMesh << "# scale " << scale << endl;
	for (auto vit = mesh->vertices_begin(); vit != mesh->vertices_end(); vit++)
	{
		auto P = mesh->point(*vit);
		outMesh << "v " << P[0]*scale << " " << P[1]*scale << " " << P[2]*scale << endl;
	}
	for (auto fit = mesh->faces_begin(); fit != mesh->faces_end(); fit++)
	{
		outMesh << "f ";
		auto fvit = mesh->cfv_begin(*fit);
		while (true)
		{
			outMesh << fvit->idx() + 1;
			++fvit;
			if (fvit != mesh->cfv_end(*fit))
			{
				outMesh << " ";
			}
			else
			{
				outMesh << endl;
				break;
			}
		}
	}
	outMesh.close();
}

/// ******************** Functions with OpenMesh ************************
OpenMesh::Vec3d nearestPtOnSegment(const OpenMesh::Vec3d &p, const OpenMesh::Vec3d &p0, const OpenMesh::Vec3d &p1);

bool ray_triangle_intersection(
	const OpenMesh::Vec3d &src, const OpenMesh::Vec3d &dir,
	const OpenMesh::Vec3d &triP0, const OpenMesh::Vec3d &triP1, const OpenMesh::Vec3d &triP2,
	OpenMesh::Vec3d &baryC, double &t);

bool isPointOnTriangle(const std::vector<OpenMesh::Vec3d>& tri, const OpenMesh::Vec3d& p);

#endif // COMMON_H