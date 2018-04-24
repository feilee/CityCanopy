#ifndef FIGURE_H
#define FIGURE_H

#include <QtGui/qopengl.h>
#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "datamanager.h"

class Figure
{

public:
    Figure();
    virtual ~Figure();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 9; }
    virtual void load(DataManager* dataManager,float width, float height) = 0;
    void initOpenGLObjects();
    void draw();

protected:
    void add(const QVector3D &v, const QVector3D &n, const QVector3D &c);
    QVector<GLfloat> m_data;
    QVector<QVector3D> m_normals;
    QOpenGLBuffer m_vbo;
    int m_count;
    float m_width;
    float m_height;
    float m_wScale;
    float m_hScale;
    float m_zScale;
    int m_ncols;
    int m_nrows;
    DataManager* m_dataManager;
};

#endif // FIGURE_H
