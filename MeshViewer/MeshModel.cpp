#include "MeshModel.h"

#include <QTextCodec>

#ifdef _WIN32
#    include <gl/GL.h>
#    include <gl/GLU.h>
#else
#    include <GLUT/glut.h>  // Header File For The GLut Library
#    include <OpenGL/OpenGL.h>
#    include <OpenGL/gl.h>   // Header File For The OpenGL32 Library
#    include <OpenGL/glu.h>  // Header File For The GLu32 Library
#endif

Model::Model()
{
    ptr_mesh_ = new MyMesh;
    ptr_selector_ = new MeshSelector;
    bbox_min_ = OpenMesh::Vec3d(-1.0, -1.0, -1.0);
    bbox_max_ = OpenMesh::Vec3d(1.0, 1.0, 1.0);
    curr_color_ = OpenMesh::Vec3f(0.0, 1.0, 1.0);
}

Model::~Model()
{
    SafeDelete(ptr_mesh_);
    SafeDelete(ptr_selector_);
}


bool Model::loadMesh(QString filename)
{
    if (filename.isEmpty()) {
        logE("empty mesh file path '%s'", filename.toStdString().c_str());
        return false;
    }
    // Support for path containing Chinese characters
    QTextCodec* code = QTextCodec::codecForName("gd18030");
    QTextCodec::setCodecForLocale(code);
    QByteArray            byfilename = filename.toLocal8Bit();
    string                sfilename = filename.toStdString();
    OpenMesh::IO::Options opt_read = OpenMesh::IO::Options::Default;
    opt_read += OpenMesh::IO::Options::VertexNormal;
    ptr_mesh_->request_vertex_normals();
    opt_read += OpenMesh::IO::Options::VertexTexCoord;
    ptr_mesh_->request_vertex_texcoords2D();
    ptr_mesh_->clear();
    if (!OpenMesh::IO::read_mesh(*ptr_mesh_, byfilename.data(), opt_read)) {
        logE("failed to load mesh from '%s'\n", filename.toStdString().c_str());
        return false;
    }
    is_load_mesh_ = true;
    size_t pos1 = sfilename.find_last_of("/");
    size_t pos2 = sfilename.find_last_of(".");
    mesh_path_ = sfilename.substr(0, pos1 + 1);
    mesh_name_ = sfilename.substr(pos1 + 1, pos2 - pos1 - 1);
    mesh_fext_ = sfilename.substr(pos2, sfilename.length() - pos2);

    initMesh();

    ptr_selector_->setMesh(ptr_mesh_);
    ptr_selector_->buildSearchTree();

    bool statusVN = opt_read.check(OpenMesh::IO::Options::VertexNormal);
    bool statusVT = opt_read.check(OpenMesh::IO::Options::VertexTexCoord);
    logI("succeed in loading a mesh with %s%s\n", (statusVN ? "[vertex normals]" : ""), (statusVT ? "[vertex uv coordinates]" : ""));
    return true;
}
bool Model::loadTexture(QString filename)
{
    if (filename.isEmpty()) {
        logE("empty texture image file path '%s'\n", filename.toStdString().c_str());
        return false;
    }
    QImage tex, buf;
    if (!buf.load(filename)) {
        logE("failed to load texture image from '%s'\n", filename.toStdString().c_str());
        return false;
    }
#if 0
	QImage dummy(128, 128, QImage::Format_ARGB32);
	dummy.fill(Qt::green);
	buf = dummy;
#endif
    if (is_load_texture_) {
        glDeleteTextures(1, &texture_[0]);
    }
    tex = QGLWidget::convertToGLFormat(buf);
    //创建纹理
    glGenTextures(1, &texture_[0]);
    //使用来自位图数据生成的典型纹理，将纹理名字texture[0]绑定到纹理目标上
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    is_load_texture_ = true;
    logI("succeed in loading a texture image from '%s'\n", filename.toStdString().c_str());
    return true;
}
void Model::initMesh()
{
    ptr_mesh_->request_vertex_status();
    ptr_mesh_->request_edge_status();
    ptr_mesh_->request_face_status();

    ptr_mesh_->request_face_normals();
    ptr_mesh_->request_vertex_normals();

    updateMeshBBox();
    unifyMesh(2.f);

    // updateMesh();
    updateMeshNormal();
    updateMeshBBox();
}

void Model::unifyMesh(float size)
{
    float scaleX = bbox_max_[0] - bbox_min_[0];
    float scaleY = bbox_max_[1] - bbox_min_[1];
    float scaleZ = bbox_max_[2] - bbox_min_[2];

    float scaleMax;

    if (scaleX < scaleY) {
        scaleMax = scaleY;
    }
    else {
        scaleMax = scaleX;
    }
    if (scaleMax < scaleZ) {
        scaleMax = scaleZ;
    }
    float         scaleV = size / scaleMax;
    MyMesh::Point centerPos((bbox_max_[0] + bbox_min_[0]) / 2.f, (bbox_max_[1] + bbox_min_[1]) / 2.f, (bbox_max_[2] + bbox_min_[2]) / 2.f);

    for (auto it = ptr_mesh_->vertices_begin(); it != ptr_mesh_->vertices_end(); ++it) {
        ptr_mesh_->point(*it) = (ptr_mesh_->point(*it) - centerPos) * scaleV;
    }
}

bool Model::saveMesh(QString file)
{
    if (file.isEmpty()) {
        logE("empty file path '%s'\n", file.toStdString().c_str());
        return false;
    }
    OpenMesh::IO::Options opt_write = OpenMesh::IO::Options::Default;
    // opt_write += OpenMesh::IO::Options::VertexNormal;
    // opt_write += OpenMesh::IO::Options::VertexTexCoord;
    if (!OpenMesh::IO::write_mesh(*ptr_mesh_, file.toStdString(), opt_write, 6)) {
        logE("failed to save current mesh to '%s'\n", file.toStdString().c_str());
        return false;
    }
    return true;
}

void Model::updateMesh()
{
    updateMeshNormal();
    updateMeshBBox();
    updateMeshCenter();
}
void Model::updateMeshNormal()
{
    ptr_mesh_->update_face_normals();
    ptr_mesh_->update_vertex_normals();
}
void Model::updateMeshBBox()
{
    MyMesh::VertexIter vIt = ptr_mesh_->vertices_begin();
    MyMesh::VertexIter vEnd = ptr_mesh_->vertices_end();
    bbox_min_ = bbox_max_ = OpenMesh::vector_cast<OpenMesh::Vec3d>(ptr_mesh_->point(*vIt));

    size_t count = 0;
    for (; vIt != vEnd; ++vIt, ++count) {
        bbox_min_.minimize(OpenMesh::vector_cast<OpenMesh::Vec3d>(ptr_mesh_->point(*vIt)));
        bbox_max_.maximize(OpenMesh::vector_cast<OpenMesh::Vec3d>(ptr_mesh_->point(*vIt)));
    }
}
void Model::updateMeshCenter()
{
    MyMesh::EdgeIter e_it = ptr_mesh_->edges_begin();
    MyMesh::EdgeIter e_end = ptr_mesh_->edges_end();
    double           aveLen = 0.0;                                 // average edge length
    double           maxLen = 0.0;                                 // max edge length
    double           minLen = ptr_mesh_->calc_edge_length(*e_it);  // min edge length
    double           e_len = 0.0;
    for (; e_it != e_end; ++e_it) {
        e_len = ptr_mesh_->calc_edge_length(*e_it);
        if (e_len > maxLen) {
            maxLen = e_len;
        }
        else if (e_len < minLen) {
            minLen = e_len;
        }
        aveLen += e_len;
    }
    printf("Edge Length : Max : %f; Min : %f; AVG : %f\n", maxLen, minLen, aveLen / ptr_mesh_->n_edges());
    printMeshInfo();
}

void Model::printMeshInfo()
{
    printf("BoundingBox:\nX : [ %f , %f ]\n", bbox_min_[0], bbox_max_[0]);
    printf("Y : [ %f , %f ]\n", bbox_min_[1], bbox_max_[1]);
    printf("Z : [ %f , %f ]\n", bbox_min_[2], bbox_max_[2]);
    printf("Diag length of BBox : %f\n", (bbox_max_ - bbox_min_).norm());
}


void Model::drawMesh()
{
    if (ptr_mesh_->n_vertices() == 0) {
        return;
    }

    switch (mesh_render_mode_) {
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
            // draw_meshpointset();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case MESH_HIDDEN_LINES:
            glDisable(GL_LIGHTING);
            drawMeshHiddenLines();
            break;
        case MESH_FLAT_LINES:
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.5f, 2.0f);
            glEnable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            drawMeshSolidFlat();
            glDisable(GL_POLYGON_OFFSET_FILL);
            // draw_meshpointset();
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            drawMeshWireFrame();
            // draw_meshpointset();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case MESH_SOLID_FLAT:
            glEnable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            drawMeshSolidFlat();
            // draw_meshpointset();
            glDisable(GL_LIGHTING);
            break;
        case MESH_SOLID_SMOOTH:
            glEnable(GL_LIGHTING);
            glShadeModel(GL_SMOOTH);
            drawMeshSolidSmooth();
            glDisable(GL_LIGHTING);
            break;
        default:
            break;
    }
}
void Model::drawMeshPointSet()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);

    // glColor3f(0.0, 1.0, 1.0);
    glColor3fv(curr_color_.data());
    glPointSize(8);
    MyMesh::VertexIter v_it = ptr_mesh_->vertices_begin();
    glBegin(GL_POINTS);
    for (; v_it != ptr_mesh_->vertices_end(); ++v_it) {
        glVertex3dv(ptr_mesh_->point(*v_it).data());
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void Model::drawMeshWireFrame()
{
    glLineWidth(1);
    // glColor3f(0.753, 0.753, 0.753);
    // glColor3f(0.0, 0.0, 0.0);
    // glColor3f(0.0, 0.0, 0.25);
    glColor3fv(curr_color_.data());
    // if(meshMode() != TRIANGLE && meshMode() != QUAD)
    {
        MyMesh::ConstFaceIter       fIt(ptr_mesh_->faces_begin()), fEnd(ptr_mesh_->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt != fEnd; ++fIt) {
            // GL::glNormal(dualMesh.normal(f_it));
            fvIt = ptr_mesh_->cfv_iter(*fIt);
            glBegin(GL_POLYGON);
            for (; fvIt.is_valid(); ++fvIt) {
                glVertex3dv(ptr_mesh_->point(*fvIt).data());
            }
            glEnd();
        }
    }
}
void Model::drawMeshHiddenLines()
{
    MyMesh::ConstFaceIter       f_it(ptr_mesh_->faces_begin());
    MyMesh::ConstFaceIter       f_end(ptr_mesh_->faces_end());
    MyMesh::ConstFaceVertexIter fv_it;
    glLineWidth(2.0);
    // glColor3f(0.0, 1.0, 1.0);
    glColor3fv(curr_color_.data());

    glDrawBuffer(GL_NONE);
    glDepthRange(0.01, 1.0);
    glBegin(GL_TRIANGLES);
    for (; f_it != f_end; ++f_it) {
        fv_it = ptr_mesh_->cfv_iter(*f_it);
        for (; fv_it.is_valid(); ++fv_it) {
            glVertex3dv(&ptr_mesh_->point(*fv_it)[0]);
        }
    }
    glEnd();


    glDrawBuffer(GL_BACK);
    glDepthRange(0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDepthFunc(GL_LEQUAL);

    glBegin(GL_TRIANGLES);
    for (f_it = ptr_mesh_->faces_begin(); f_it != f_end; ++f_it) {
        fv_it = ptr_mesh_->cfv_iter(*f_it);
        for (; fv_it.is_valid(); ++fv_it) {
            glVertex3dv(&ptr_mesh_->point(*fv_it)[0]);
        }
    }
    glEnd();

    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void Model::drawMeshSolidFlat()
{
    MyMesh::ConstFaceIter       fIt(ptr_mesh_->faces_begin()), fEnd(ptr_mesh_->faces_end());
    MyMesh::ConstFaceVertexIter fvIt;

    GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    // GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
    GLfloat mat_d[] = { curr_color_[0], curr_color_[1], curr_color_[2], 1.0f };
    GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat shine[] = { 120.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

    glBegin(GL_TRIANGLES);
    for (; fIt != fEnd; ++fIt) {
        glNormal3dv(ptr_mesh_->normal(*fIt).data());
        fvIt = ptr_mesh_->cfv_iter(*fIt);
        for (; fvIt.is_valid(); ++fvIt) {
            glVertex3dv(ptr_mesh_->point(*fvIt).data());
        }
    }
    glEnd();
    // logI("draw mesh solid flat called\n");
}
void Model::drawMeshSolidSmooth()
{
#if 1
    GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    // GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
    GLfloat mat_d[] = { curr_color_[0], curr_color_[1], curr_color_[2], 1.0f };
    GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat shine[] = { 120.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
#endif
#if 0
    glShadeModel(GL_SMOOTH);
    glColor3fv(curr_color_.data());
    MyMesh::ConstFaceIter       fIt(ptr_mesh_->faces_begin()), fEnd(ptr_mesh_->faces_end());
    MyMesh::ConstFaceVertexIter fvIt;
    glBegin(GL_TRIANGLES);
    for (fIt; fIt != fEnd; ++fIt) {
        glNormal3dv(ptr_mesh_->normal(fIt).data());
        fvIt = ptr_mesh_->cfv_iter(fIt.handle());
        for (fvIt; fvIt; ++fvIt) {
            glVertex3dv(ptr_mesh_->point(fvIt).data());
        }
    }
    glEnd();

#else

    // glLoadName(ptr_mesh_->n_vertices());

    MyMesh::ConstFaceIter       fIt(ptr_mesh_->faces_begin()), fEnd(ptr_mesh_->faces_end());
    MyMesh::ConstFaceVertexIter fvIt;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, ptr_mesh_->points());
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_DOUBLE, 0, ptr_mesh_->vertex_normals());

    for (; fIt != fEnd; ++fIt) {
        glBegin(GL_POLYGON);
        fvIt = ptr_mesh_->cfv_iter(*fIt);
        for (; fvIt.is_valid(); fvIt++) {
            glArrayElement(fvIt->idx());
        }
        glEnd();
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
#endif
}

void Model::drawAxes()
{
    if (!is_draw_axes_)
        return;

    // x axis
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

    // y axis
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

    // z axis
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

    // renderTextStr(len + 0.1, 0, 0, "X", 1.0, 0.0, 0.0, 16);
    // renderTextStr(0, len + 0.1, 0, "Y", 0.0, 1.0, 0.0, 16);
    // renderTextStr(0, 0, len + 0.1, "Z", 0.0, 0.0, 1.0, 16);
}
void Model::drawBBox()
{
    if (!is_draw_bbox_)
        return;

    OpenMesh::Vec3d temp0 = bbox_min_;
    OpenMesh::Vec3d temp1;
    glLineWidth(2.0);
    // glColor3f(1.0, 1.0, 0.0);
    glColor3fv(curr_color_.data());
    glBegin(GL_LINES);
    temp1 = bbox_min_;
    temp1[0] = bbox_max_[0];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());
    temp1 = bbox_min_;
    temp1[1] = bbox_max_[1];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());
    temp1 = bbox_min_;
    temp1[2] = bbox_max_[2];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_min_;
    temp0[0] = bbox_max_[0];
    temp1 = bbox_max_;
    temp1[1] = bbox_min_[1];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_min_;
    temp0[0] = bbox_max_[0];
    temp1 = bbox_max_;
    temp1[2] = bbox_min_[2];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_min_;
    temp0[1] = bbox_max_[1];
    temp1 = bbox_max_;
    temp1[2] = bbox_min_[2];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_min_;
    temp0[1] = bbox_max_[1];
    temp1 = bbox_max_;
    temp1[0] = bbox_min_[0];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_min_;
    temp0[2] = bbox_max_[2];
    temp1 = bbox_max_;
    temp1[1] = bbox_min_[1];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_min_;
    temp0[2] = bbox_max_[2];
    temp1 = bbox_max_;
    temp1[0] = bbox_min_[0];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());

    temp0 = bbox_max_;
    temp1 = bbox_max_;
    temp1[0] = bbox_min_[0];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());
    temp1 = bbox_max_;
    temp1[1] = bbox_min_[1];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());
    temp1 = bbox_max_;
    temp1[2] = bbox_min_[2];
    glVertex3dv(temp0.data());
    glVertex3dv(temp1.data());
    glEnd();


    // int prec = 4;
    // int width = 4;
    // QString texMin = QString("min:  %1  %2  %3").arg(double(bbox_min_[0]), width, 'g', prec, QChar(' ')).arg(double(bbox_min_[1]), width, 'g', prec, QChar(' ')).arg(double(bbox_min_[2]), width,
    // 'g', prec, QChar(' ')); QString texMax = QString("max:  %1  %2  %3").arg(double(bbox_max_[0]), width, 'g', prec, QChar(' ')).arg(double(bbox_max_[1]), width, 'g', prec, QChar('
    // ')).arg(double(bbox_max_[2]), width, 'g', prec, QChar(' ')); QString texSize = QString("size: %1  %2  %3").arg(double(bbox_max_[0] - bbox_min_[0]), width, 'g', prec, QChar('
    // ')).arg(double(bbox_max_[1] - bbox_min_[1]), width, 'g', prec, QChar(' ')).arg(double(bbox_max_[2] - bbox_min_[2]), width, 'g', prec, QChar(' ')); renderTextStr(10, 20, texMin);
    // renderTextStr(10, 35, texMax);
    // renderTextStr(10, 50, texSize);
}
void Model::drawBoundary()
{
    if (!is_draw_boundary_)
        return;

    glLineWidth(6.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (MyMesh::EdgeIter e_it = ptr_mesh_->edges_begin(); e_it != ptr_mesh_->edges_end(); ++e_it) {
        if (ptr_mesh_->is_boundary(*e_it)) {
            MyMesh::HalfedgeHandle heh0 = ptr_mesh_->halfedge_handle(*e_it, 0);
            glVertex3dv(ptr_mesh_->point(ptr_mesh_->to_vertex_handle(heh0)).data());
            glVertex3dv(ptr_mesh_->point(ptr_mesh_->from_vertex_handle(heh0)).data());
        }
    }
    glEnd();
}
void Model::drawTexture()
{
    if (!is_draw_texture_)
        return;
        // if (ptr_mesh_->n_faces() == 0 || !is_load_texture_) return;
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
    for (auto face = ptr_mesh_->faces_begin(); face != ptr_mesh_->faces_end(); ++face) {
        for (auto it = ptr_mesh_->fv_begin(*face); it != ptr_mesh_->fv_end(*face); ++it) {
            OpenMesh::Vec2f tex = ptr_mesh_->texcoord2D(*it);
            glTexCoord2f(tex[0], tex[1]);
            MyMesh::Normal normal = ptr_mesh_->normal(*it);
            glNormal3dv(normal.data());
            auto point = ptr_mesh_->point(*it);
            glVertex3dv(point.data());
            // printf("tex %d = (%f, %f)\n", vertex.idx(), tex[0], tex[1]);
        }
    }
    glEnd();
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

void Model::drawSelection()
{
    ptr_selector_->drawSelectedVert();
    ptr_selector_->drawSelectedEdge();
    ptr_selector_->drawSelectedFace();
}
void Model::drawSelectionTag(QGLWidget* widget)
{
    ptr_selector_->drawSelectedVertTag(widget);
    ptr_selector_->drawSelectedEdgeTag(widget);
    ptr_selector_->drawSelectedFaceTag(widget);
}

void Model::drawOthers()
{
    // drawAxes();
    drawBBox();
    drawBoundary();
    drawTexture();
}

bool Model::emptyMesh()
{
    if (ptr_mesh_->n_vertices() == 0)
        return true;
    return false;
}

QString Model::getMeshInfo()
{
    return QString("Vertices  %1 \n"
                   "Edges     %2 \n"
                   "Faces     %3 \n"
                   "Path      %4 \n"
                   "Name      %5%6 ")
        .arg(ptr_mesh_->n_vertices())
        .arg(ptr_mesh_->n_edges())
        .arg(ptr_mesh_->n_faces())
        .arg(QString::fromStdString(mesh_path_))
        .arg(QString::fromStdString(mesh_name_))
        .arg(QString::fromStdString(mesh_fext_));
}
