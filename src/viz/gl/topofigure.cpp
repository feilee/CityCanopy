#include "viz/gl/topofigure.h"

#include <QVector3D>
#include <QDebug>
#include <math.h>

TopoFigure::TopoFigure()
{

}

TopoFigure::~TopoFigure()
{

}

void TopoFigure::load(DataManager *dataManager, float width, float height)
{
    m_dataManager = dataManager;
    m_ncols = dataManager->getColsSize();
    m_nrows = dataManager->getRowsSize();
    m_width = width;
    m_height = height;
    m_wScale = m_width/(m_ncols-1);
    m_hScale = m_height/(m_nrows-1);
    m_zScale = 1.0/3000.0;

    QVector3D color(0.58,0.44,0.09);

    // vertex i,j normal := m_normals[i*m_ncols+j]
    finalNormals();

    for(int i = 0; i < m_nrows-1; i++){
        for(int j = 0; j < m_ncols-1; j++){

            GLfloat x1 = m_wScale*(j - (m_ncols-1)/2.0f);
            GLfloat x2 = m_wScale*(j + 1 - (m_ncols-1)/2.0f);
            GLfloat y1 = m_hScale*((m_nrows-1)/2.0f - i);
            GLfloat y2 = m_hScale*((m_nrows-1)/2.0f - i - 1);

            GLfloat z11 = dataManager->getValue(i,j)*m_zScale;
            GLfloat z12 = dataManager->getValue((i+1),j)*m_zScale;
            GLfloat z22 = dataManager->getValue((i+1),(j+1))*m_zScale;
            GLfloat z21 = dataManager->getValue(i,(j+1))*m_zScale;

            QVector3D v1(x1,y1,z11);
            QVector3D v2(x1,y2,z12);
            QVector3D v3(x2,y2,z22);
            QVector3D v4(x2,y1,z21);

            add(v1,m_normals[i*m_ncols+j],color);
            add(v2,m_normals[(i+1)*m_ncols+j],color);
            add(v3,m_normals[(i+1)*m_ncols+j+1],color);

            add(v1,m_normals[i*m_ncols+j],color);
            add(v3,m_normals[(i+1)*m_ncols+j+1],color);
            add(v4,m_normals[i*m_ncols+j+1],color);
        }
    }
}

void TopoFigure::finalNormals()
{

    m_normals.resize(m_ncols*m_nrows);
    for( int k = 0; k < m_ncols*m_nrows ; ++k){
        m_normals[k] = QVector3D(0.0,0.0,0.0);
    }

    for(int i = 0; i < m_nrows-1; i++){
        for(int j = 0; j < m_ncols-1; j++){

            GLfloat x1 = m_wScale*(j - (m_ncols-1)/2.0f);
            GLfloat x2 = m_wScale*(j + 1 - (m_ncols-1)/2.0f);
            GLfloat y1 = m_hScale*((m_nrows-1)/2.0f - i);
            GLfloat y2 = m_hScale*((m_nrows-1)/2.0f - i - 1);

            GLfloat z11 = m_dataManager->getValue(i,j)*m_zScale;
            GLfloat z12 = m_dataManager->getValue((i+1),j)*m_zScale;
            GLfloat z22 = m_dataManager->getValue((i+1),(j+1))*m_zScale;
            GLfloat z21 = m_dataManager->getValue(i,(j+1))*m_zScale;

            QVector3D v1(x1,y1,z11);
            QVector3D v2(x1,y2,z12);
            QVector3D v3(x2,y2,z22);
            QVector3D v4(x2,y1,z21);

            QVector3D n1 = QVector3D::normal(v2-v1,v3-v1);
            QVector3D n2 = QVector3D::normal(v3-v1,v4-v1);

            m_normals[i*m_ncols + j] = n1+m_normals[i*m_ncols+j];
            m_normals[(i+1)*m_ncols+j] = n1+m_normals[(i+1)*m_ncols+j];
            m_normals[(i+1)*m_ncols+j+1] = n1+m_normals[(i+1)*m_ncols+j+1];

            m_normals[i*m_ncols+j] = n2+m_normals[i*m_ncols+j];
            m_normals[(i+1)*m_ncols+j+1] = n2+m_normals[(i+1)*m_ncols+j+1];
            m_normals[i*m_ncols+j+1] = n2+m_normals[i*m_ncols+j+1];
        }
    }
}


float TopoFigure::getZValue(float x, float y){
    int j,i;
    j = x/m_wScale + (m_ncols-1)/2.0f;
    float i_aux =  (m_nrows-1)/2.0f - y/m_hScale;
    //Looking for upper or lower triangle
    QVector3D v1,v2,v3;
    i = round(i_aux);

    //base case: point over an edge
    if( i_aux == i){
        // special case: point over a vertex
        if( j >= m_ncols-1)
            return m_dataManager->getValue(i,j)*m_zScale;
        else{
            float z1 = m_dataManager->getValue(i,j)*m_zScale;
            float z2 = m_dataManager->getValue(i,j+1)*m_zScale;
            float x1 = m_wScale*(j - (m_ncols-1)/2.0f);
            float x2 = m_wScale*(j+1 - (m_ncols-1)/2.0f);
            float m = (z2-z1)/(x2-x1);
            float z = m*(x-x1)+z1;
            return z;
        }
    //first case: point within lower triangle
    }else if( i_aux < i){
        v1 = QVector3D(j,i,0);
        v2 = QVector3D(j+1,i,0);
        v3 = QVector3D(j+1,i-1,0);
    //second case: point within upper triangle
    }else{
        v1 = QVector3D(j,i+1,0);
        v2 = QVector3D(j+1,i,0);
        v3 = QVector3D(j,i,0);
    }

    //if (i >= m_nrows-1 || j >= m_ncols-1) return 0.0f;

    v1.setZ(m_dataManager->getValue(v1.y(),v1.x())*m_zScale);
    v1.setX(m_wScale*(v1.x() - (m_ncols-1)/2.0f));
    v1.setY(m_hScale*((m_nrows-1)/2.0f - v1.y()));

    v2.setZ(m_dataManager->getValue(v2.y(),v2.x())*m_zScale);
    v2.setX(m_wScale*(v2.x() - (m_ncols-1)/2.0f));
    v2.setY(m_hScale*((m_nrows-1)/2.0f - v2.y()));

    v3.setZ(m_dataManager->getValue(v3.y(),v3.x())*m_zScale);
    v3.setX(m_wScale*(v3.x() - (m_ncols-1)/2.0f));
    v3.setY(m_hScale*((m_nrows-1)/2.0f - v3.y()));

    QVector3D n = QVector3D::normal(v2-v1,v3-v1);

    //Using v1 as pivot (v1 - v')*n = 0
    float zValue = v1.z() + ( (v1.x() - x) * n.x() + (v1.y() - y) * n.y() ) / n.z();
    return zValue;
}
