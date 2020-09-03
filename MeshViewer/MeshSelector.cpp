#include "MeshSelector.h"

#ifdef _WIN32
#    include <gl/GL.h>
#    include <gl/GLU.h>
#else
#    include <GLUT/glut.h>  // Header File For The GLut Library
#    include <OpenGL/OpenGL.h>
#    include <OpenGL/gl.h>   // Header File For The OpenGL32 Library
#    include <OpenGL/glu.h>  // Header File For The GLu32 Library
#endif

MeshSelector::MeshSelector()
{
    ptr_mesh_ = new MyMesh;
    kdTree = NULL;
}

MeshSelector::~MeshSelector()
{
    if (kdTree)
        delete kdTree;
}

void MeshSelector::drawSelectedVert()
{
    if (!is_draw_selected_vert_) {
        return;
    }
    if (selectedVert_.size() > 0) {
        MyMesh::Point p;
        glColor3fv(vert_color_.data());
        glPointSize(vert_size_);
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < selectedVert_.size(); ++i) {
            p = ptr_mesh_->point(ptr_mesh_->vertex_handle(selectedVert_[i]));
            glVertex3dv(p.data());
        }
        glEnd();
        glPointSize(1);
    }
}

void MeshSelector::drawSelectedEdge()
{
    if (!is_draw_selected_edge_) {
        return;
    }
    if (selectedEdge_.size() > 0) {
        glColor3fv(edge_color_.data());
        glLineWidth(edge_width_);
        MyMesh::Point          p1;
        MyMesh::Point          p2;
        MyMesh::EdgeHandle     e_handle;
        MyMesh::HalfedgeHandle he_handle;
        for (unsigned int i = 0; i < selectedEdge_.size(); ++i) {
            e_handle = ptr_mesh_->edge_handle(selectedEdge_[i]);
            he_handle = ptr_mesh_->halfedge_handle(e_handle, 0);
            p1 = ptr_mesh_->point(ptr_mesh_->from_vertex_handle(he_handle));
            p2 = ptr_mesh_->point(ptr_mesh_->to_vertex_handle(he_handle));
            glBegin(GL_LINES);
            glVertex3dv(p1.data());
            glVertex3dv(p2.data());
            glEnd();
        }
        glLineWidth(1.0f);
    }
}

void MeshSelector::drawSelectedFace()
{
    if (!is_draw_selected_face_) {
        return;
    }
    if (selectedFace_.size() > 0) {
        glColor3fv(face_color_.data());
        MyMesh::Point               p;
        MyMesh::ConstFaceVertexIter fv_it;
        MyMesh::FaceHandle          f_handle;
        for (unsigned int i = 0; i < selectedFace_.size(); ++i) {
            f_handle = ptr_mesh_->face_handle(selectedFace_[i]);
            fv_it = ptr_mesh_->fv_iter(f_handle);
            glBegin(GL_POLYGON);
            for (; fv_it.is_valid(); ++fv_it) {
                glVertex3dv(&ptr_mesh_->point(*fv_it)[0]);
            }
            glEnd();
        }
    }
}

void MeshSelector::drawSelectedVertTag(QGLWidget* widget)
{
    if (is_tag_selected_vert_) {
        glColor3fv(vtag_color_.data());
        for (size_t i = 0; i < selectedVert_.size(); i++) {
            MyMesh::Point p = ptr_mesh_->point(ptr_mesh_->vertex_handle(selectedVert_[i]));
            widget->renderText(p[0], p[1], p[2], QString(" v%1").arg(selectedVert_[i]), QFont("Courier", tag_font_size_, QFont::Normal, false));  // QFont::Arial
        }
        widget->renderText(10, 65, QString("selected verts %1").arg(selectedVert_.size()), QFont("Courier", tag_font_size_, QFont::Normal, false));  // QFont::Arial
    }
}

void MeshSelector::drawSelectedEdgeTag(QGLWidget* widget)
{
    if (is_tag_selected_edge_) {
        glColor3fv(etag_color_.data());
        for (size_t i = 0; i < selectedEdge_.size(); i++) {
            HHandle         he = ptr_mesh_->halfedge_handle(ptr_mesh_->edge_handle(selectedEdge_[i]), 0);
            OpenMesh::Vec3d d = ptr_mesh_->calc_edge_vector(he);
            MyMesh::Point   p = ptr_mesh_->point(ptr_mesh_->from_vertex_handle(he));
            p = p + 0.5 * d;
            widget->renderText(p[0], p[1], p[2], QString("e%1").arg(selectedEdge_[i]), QFont("Courier", tag_font_size_, QFont::Normal, false));
        }
        widget->renderText(10, 80, QString("selected edges %1").arg(selectedEdge_.size()), QFont("Courier", tag_font_size_, QFont::Normal, false));
    }
}

void MeshSelector::drawSelectedFaceTag(QGLWidget* widget)
{
    if (is_tag_selected_face_) {
        glColor3fv(ftag_color_.data());
        for (size_t i = 0; i < selectedFace_.size(); i++) {
            MyMesh::Point p = ptr_mesh_->calc_face_centroid(ptr_mesh_->face_handle(selectedFace_[i]));
            widget->renderText(p[0], p[1], p[2], QString("f%1").arg(selectedFace_[i]), QFont("Courier", tag_font_size_, QFont::Normal, false));
        }
        widget->renderText(10, 95, QString("selected faces %1").arg(selectedFace_.size()), QFont("Courier", tag_font_size_, QFont::Normal, false));
    }
}

void MeshSelector::findVertByPos(const MyMesh::Point& p)
{
    double  dist = 0.0;
    int     vi = findClosestVertFromPoint(p, dist);
    QString pStr = QString("(%1,%2,%3)").arg(double(p[0]), 0, 'g', 4).arg(double(p[1]), 0, 'g', 4).arg(double(p[2]), 0, 'g', 4);
    logI("find closest vertex %d from point %s with distance %f\n", vi, pStr.toStdString().c_str(), dist);
    findVertByIdx(vi);
}

void MeshSelector::findVertByIdx(int vi)
{
    vector<int>::iterator it;
    if ((it = find(selectedVert_.begin(), selectedVert_.end(), vi)) == selectedVert_.end()) {
        selectedVert_.push_back(vi);
        logI("select vertex %d\n", vi);
    }
    else {
        selectedVert_.erase(it);
        logI("unselect vertex %d\n", vi);
    }
}

void MeshSelector::findEdgeByIdx(int ei)
{
    vector<int>::iterator it;
    if ((it = find(selectedEdge_.begin(), selectedEdge_.end(), ei)) == selectedEdge_.end()) {
        selectedEdge_.push_back(ei);
        logI("select edge %d\n", ei);
    }
    else {
        selectedEdge_.erase(it);
        logI("unselect edge %d\n", ei);
    }
}

void MeshSelector::findFaceByIdx(int fi)
{
    vector<int>::iterator it;
    if ((it = find(selectedFace_.begin(), selectedFace_.end(), fi)) == selectedFace_.end()) {
        selectedFace_.push_back(fi);
        logI("select face %d\n", fi);
    }
    else {
        selectedFace_.erase(it);
        logI("unselect face %d\n", fi);
    }
}


void MeshSelector::pickVert(const MyMesh::Point& p)
{
    int vi = findVertByPoint(p);
    if (vi < 0 || vi > (int)ptr_mesh_->n_vertices()) {
        return;
    }
    findVertByIdx(vi);
}

void MeshSelector::pickEdge(const MyMesh::Point& p)
{
    int ei = findEdgeByPoint(p);
    if (ei < 0 || ei > (int)ptr_mesh_->n_edges()) {
        return;
    }
    findEdgeByIdx(ei);
}

void MeshSelector::pickFace(const MyMesh::Point& p)
{
    int fi = findFaceByPoint(p);
    if (fi < 0 || fi > (int)ptr_mesh_->n_faces()) {
        return;
    }
    findFaceByIdx(fi);
}

void MeshSelector::buildSearchTree()
{
    if (ptr_mesh_->n_vertices() == 0)
        return;

    MyMesh::VertexIter v_it(ptr_mesh_->vertices_begin());
    MyMesh::VertexIter v_end(ptr_mesh_->vertices_end());
    MyMesh::Point      p;
    int                nv = (int)ptr_mesh_->n_vertices();
    ANNpointArray      dataPts = annAllocPts(nv, 3);
    int                count = 0;
    for (; v_it != v_end; ++v_it) {
        p = ptr_mesh_->point(*v_it);
        dataPts[count][0] = p[0];
        dataPts[count][1] = p[1];
        dataPts[count][2] = p[2];
        ++count;
    }

    // if (kdTree) delete kdTree;
    kdTree = new ANNkd_tree(dataPts, nv, 3);
}

int MeshSelector::findClosestVertFromPoint(const MyMesh::Point& p, double& dist)
{
    ANNpoint tp = annAllocPt(3);
    tp[0] = p[0];
    tp[1] = p[1];
    tp[2] = p[2];
    ANNidxArray  nnIdx = new ANNidx[1];
    ANNdistArray dists = new ANNdist[1];
    kdTree->annkSearch(tp, 1, nnIdx, dists);
    dist = dists[0];
    return nnIdx[0];
}

int MeshSelector::findVertByPoint(const MyMesh::Point& p)
{
    ANNpoint tp = annAllocPt(3);
    tp[0] = p[0];
    tp[1] = p[1];
    tp[2] = p[2];
    ANNidxArray  nnIdx = new ANNidx[1];
    ANNdistArray dists = new ANNdist[1];
    kdTree->annkSearch(tp, 1, nnIdx, dists);
    // printf("close vertex %d with distance %f\n", nnIdx[0], dists[0]);
    if (dists[0] > point_dist_thres_) {
        return -1;
    }
    return nnIdx[0];
}

int MeshSelector::findEdgeByPoint(const MyMesh::Point& p)
{
    int desiredFace = findFaceByPoint(p);
    if (desiredFace < 0)
        return -1;

    double min_len = 1e30;
    int    desiredEdge = -1;

    MyMesh::FaceHandle fh = ptr_mesh_->face_handle(desiredFace);
    OpenMesh::Vec3d    resultP(p[0], p[1], p[2]);
    for (MyMesh::FaceHalfedgeIter fhe_it = ptr_mesh_->fh_iter(fh); fhe_it.is_valid(); ++fhe_it) {
        OpenMesh::Vec3d s = ptr_mesh_->point(ptr_mesh_->from_vertex_handle(*fhe_it));
        OpenMesh::Vec3d e = ptr_mesh_->point(ptr_mesh_->to_vertex_handle(*fhe_it));
        double          dis = (OpenMesh::cross(resultP - s, resultP - e)).norm() / (s - e).norm();
        if (dis < min_len) {
            min_len = dis;
            desiredEdge = ptr_mesh_->edge_handle(*fhe_it).idx();
        }
    }

    return desiredEdge;
}

int MeshSelector::findFaceByPoint(const MyMesh::Point& p)
{
    int rv = findVertByPoint(p);
    if (rv == -1)
        return -1;

    MyMesh::VertexFaceIter       vf_it = ptr_mesh_->vf_iter(ptr_mesh_->vertex_handle(rv));
    int                          desiredFace = -1;  // double minLen = 10*radius();
    std::vector<OpenMesh::Vec3d> tri_p(3);
    int                          tri_count = 0;
    MyMesh::Point                resultP(p[0], p[1], p[2]);
    for (; vf_it.is_valid(); ++vf_it) {
        tri_count = 0;
        for (MyMesh::FaceVertexIter fv_it = ptr_mesh_->fv_iter(*vf_it); fv_it.is_valid(); ++fv_it) {
            tri_p[tri_count] = ptr_mesh_->point(*fv_it);
            ++tri_count;
        }
        if (isPointOnTriangle(tri_p, resultP)) {
            desiredFace = vf_it->idx();
            break;
        }
    }
    if (desiredFace < 0) {
        for (MyMesh::FaceIter f_it = ptr_mesh_->faces_begin(); f_it != ptr_mesh_->faces_end(); ++f_it) {
            tri_count = 0;
            for (MyMesh::FaceVertexIter fv_it = ptr_mesh_->fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
                tri_p[tri_count] = ptr_mesh_->point(*fv_it);
                ++tri_count;
            }
            if (isPointOnTriangle(tri_p, resultP)) {
                desiredFace = f_it->idx();
                break;
            }
        }
    }

    return desiredFace;
}


void MeshSelector::saveSelectedVert(string savePath)
{
    std::ofstream out(savePath);
    out << "Selected Vertices " << selectedVert_.size() << endl;
    for (size_t i = 0; i < selectedVert_.size(); i++) {
        MyMesh::Point p = ptr_mesh_->point(ptr_mesh_->vertex_handle(selectedVert_[i]));
        out << i << " V" << selectedVert_[i];
        out << "  VP(" << p[0] << "," << p[1] << "," << p[2] << ")";
        out << endl;
    }
    out.close();
    logI("save selected vertices to '%s'\n", savePath.c_str());
}

void MeshSelector::saveSelectedEdge(string savePath)
{
    std::ofstream out(savePath);
    out << "Selected Edges " << selectedEdge_.size() << endl;
    for (size_t i = 0; i < selectedEdge_.size(); i++) {
        HHandle       he = ptr_mesh_->halfedge_handle(ptr_mesh_->edge_handle(selectedEdge_[i]), 1);
        VHandle       v0 = ptr_mesh_->from_vertex_handle(he);
        VHandle       v1 = ptr_mesh_->to_vertex_handle(he);
        MyMesh::Point p0 = ptr_mesh_->point(v0);
        MyMesh::Point p1 = ptr_mesh_->point(v1);

        out << i << " E" << selectedEdge_[i];
        out << "  EV[" << v0.idx() << "|" << v1.idx() << "]";
        out << "  P0(" << p0[0] << "," << p0[1] << "," << p0[2] << ")";
        out << "  P1(" << p1[0] << "," << p1[1] << "," << p1[2] << ")";
        out << endl;
    }
    out.close();
    logI("save selected edges to '%s'\n", savePath.c_str());
}

void MeshSelector::saveSelectedFace(string savePath)
{
    std::ofstream out(savePath);
    out << "Selected Faces " << selectedFace_.size() << endl;
    for (size_t i = 0; i < selectedFace_.size(); i++) {
        FHandle fh = ptr_mesh_->face_handle(selectedFace_[i]);
        out << i << " F" << selectedFace_[i];
        vector<VHandle> fv;
        for (MyMesh::FaceVertexIter it = ptr_mesh_->fv_begin(fh); it != ptr_mesh_->fv_end(fh); ++it) {
            fv.push_back(*it);
        }
        out << "  FV[" << fv[0].idx() << "|" << fv[1].idx() << "|" << fv[2].idx() << "]";
        out << endl;
    }
    out.close();
    logI("save selected faces to '%s'\n", savePath.c_str());
}
