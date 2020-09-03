//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/8 20:30:04
// Last Change  : 2017/Jan/8 20:30:07
// Summary      :
//===============================================================
#include "Common.h"

Eigen::Vector3f colorTable[10] = {
    Eigen::Vector3f(0.9f, 0.2f, 0.2f),  //  1: Red
    Eigen::Vector3f(0.2f, 0.2f, 0.9f),  //  2: Blue
    Eigen::Vector3f(0.9f, 0.9f, 0.5f),  //  3: Yellow
    Eigen::Vector3f(0.9f, 0.5f, 0.2f),  //  4: Orange
    Eigen::Vector3f(0.7f, 0.2f, 0.9f),  //  5: Purple
    Eigen::Vector3f(0.3f, 0.8f, 0.8f),  //  6: Cyan
    Eigen::Vector3f(0.5f, 0.3f, 0.2f),  //  7: Brown
    Eigen::Vector3f(0.9f, 0.2f, 0.6f),  //  8: Pink
    Eigen::Vector3f(0.6f, 0.6f, 0.7f),  //  9: Gray
    Eigen::Vector3f(0.9f, 0.6f, 0.5f)   // 10: LightSalmon
};

void save2DTexMesh(const MyMesh* mesh, string file)
{
    if (!mesh->has_vertex_texcoords2D()) {
        logE("no texture coordinates for mesh to write.\n");
        return;
    }
    ofstream outMesh;
    outMesh = ofstream(file);
    outMesh << "# vertices " << mesh->n_vertices() << endl;
    outMesh << "# faces  " << mesh->n_faces() << endl;
    for (auto vit = mesh->vertices_begin(); vit != mesh->vertices_end(); vit++) {
        // auto            P = mesh->point(*vit);
        OpenMesh::Vec2f tex = mesh->texcoord2D(*vit);
        outMesh << "v " << tex[0] << " " << tex[1] << " " << 0.0 << endl;
    }
    for (auto fit = mesh->faces_begin(); fit != mesh->faces_end(); fit++) {
        outMesh << "f ";
        auto fvit = mesh->cfv_begin(*fit);
        while (true) {
            outMesh << fvit->idx() + 1;
            ++fvit;
            if (fvit != mesh->cfv_end(*fit)) {
                outMesh << " ";
            }
            else {
                outMesh << endl;
                break;
            }
        }
    }
    outMesh.close();
}

void meshReader(std::string meshFile)
{
    using std::string;
    using std::vector;
    using namespace Eigen;

    std::ifstream inMesh;
    inMesh.open(meshFile);
    assert(inMesh.is_open());

    std::stringstream lineInput;
    string            line, dump;
    Vector3d          pos;
    Vector3i          tri;
    vector<Vector3d>  vertVec;
    vector<Vector3i>  faceVec;
    while (getline(inMesh, line)) {
        if (line.substr(0, 2) == "v ") {
            lineInput.str("");
            lineInput.clear();
            lineInput << line;
            lineInput >> dump >> pos(0) >> pos(1) >> pos(2);
            vertVec.push_back(pos);
        }
        if (line.substr(0, 2) == "f ") {
            lineInput.str("");
            lineInput.clear();
            lineInput << line;
            lineInput >> dump >> tri(0) >> tri(1) >> tri(2);
            faceVec.push_back(tri - Vector3i::Ones(3));
        }
    }
    inMesh.close();

    // store data in Eigen matrix
    MatrixXd meshVert_ = MatrixXd::Zero(vertVec.size(), 3);
    MatrixXi meshFace_ = MatrixXi::Zero(faceVec.size(), 3);
    for (size_t i = 0; i < vertVec.size(); i++) {
        meshVert_.row(i) = vertVec.at(i);
    }
    for (size_t i = 0; i < faceVec.size(); i++) {
        meshFace_.row(i) = faceVec.at(i);
    }
}

OpenMesh::Vec3d nearestPtOnSegment(const OpenMesh::Vec3d& p, const OpenMesh::Vec3d& p0, const OpenMesh::Vec3d& p1)
{
    double lambda = (p - p0) | (p1 - p0) / (p1 - p0).sqrnorm();
    if (lambda > 0 && lambda < 1) {
        return p0 + lambda * (p1 - p0);
    }
    return (p - p0).sqrnorm() < (p1 - p).sqrnorm() ? p0 : p1;
}


bool ray_triangle_intersection(const OpenMesh::Vec3d& src, const OpenMesh::Vec3d& dir, const OpenMesh::Vec3d& triP0, const OpenMesh::Vec3d& triP1, const OpenMesh::Vec3d& triP2, OpenMesh::Vec3d& baryC,
                               double& t)
{
    double area = ((triP1 - triP0) % (triP2 - triP0)).norm();
    if (abs(area) < IS_ZERO) {
        cout << "error in CMeshProjection::ray_triangle_intersection [degenerate triangle]" << endl;
        return false;
    }

    OpenMesh::Vec3d triN = (triP1 - triP0) % (triP2 - triP0);
    triN.normalize();
    if (abs(triN | dir) < IS_ZERO) {
        cout << "warning in CMeshProjection::ray_triangle_intersection [ray is parallel to triangle]" << endl;
        return false;
    }

    t = (triN | (triP0 - src)) / (triN | dir);
    OpenMesh::Vec3d P = src + t * dir;

    double area0 = ((triP1 - P) % (triP2 - P)).norm();
    double area1 = ((triP2 - P) % (triP0 - P)).norm();
    double area2 = ((triP0 - P) % (triP1 - P)).norm();
    if (abs(area0 + area1 + area2 - area) < IS_ZERO) {
        baryC = OpenMesh::Vec3d(area0 / area, area1 / area, area2 / area);
        return true;
    }
    else {
        return false;
    }
    return false;
}

bool isPointOnTriangle(const std::vector<OpenMesh::Vec3d>& tri, const OpenMesh::Vec3d& p)
{
    OpenMesh::Vec3d v1 = tri[1] - tri[0];
    OpenMesh::Vec3d v2 = tri[2] - tri[0];
    OpenMesh::Vec3d n = OpenMesh::cross(v1, v2);
    double          face_area = n.norm();
    n.normalize();
    double all_area = 0;
    for (unsigned i = 0; i < tri.size(); ++i) {
        unsigned next_i = (i + 1) % tri.size();
        unsigned prev_i = (i + tri.size() - 1) % tri.size();
        v1 = tri[next_i] - p;
        v2 = tri[prev_i] - p;
        double area = OpenMesh::dot(OpenMesh::cross(v1, v2), n);
        all_area += area;
        if (area < 0) {
            return false;
        }
    }
    if (std::abs(all_area - face_area) < 1e-8) {
        return true;
    }
    else {
        return false;
    }
}
