#include "viz/gl/figure.h"
#include <QOpenGLFunctions>
#include <QDebug>

Figure::Figure() : m_count(0)
{

}

Figure::~Figure()
{

}

void Figure::initOpenGLObjects(){
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(constData(),count()*sizeof(GLfloat));
    m_vbo.release();
}
void Figure::draw(){
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    f->glDrawArrays(GL_TRIANGLES, 0, vertexCount());
    m_vbo.release();
}

void Figure::add(const QVector3D &v, const QVector3D &n, const QVector3D &c)
{
    m_data.append(v.x());
    m_data.append(v.y());
    m_data.append(v.z());
    m_data.append(n.x());
    m_data.append(n.y());
    m_data.append(n.z());
    m_data.append(c.x());
    m_data.append(c.y());
    m_data.append(c.z());
    m_count += 9;
}

