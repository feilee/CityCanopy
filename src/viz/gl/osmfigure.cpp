#include "viz/gl/osmfigure.h"

#include <QVector3D>
#include <QDebug>
#define COMMERCIAL 33
#define HIGHRESIDENTIAL 32
#define LOWRESIDENTIAL 31
#define NONE 0

OSMFigure::OSMFigure()
{

}

OSMFigure::~OSMFigure()
{

}

void OSMFigure::load(DataManager *dataManager, float width, float height){
   load(dataManager,width,height, 0);
}

void OSMFigure::load(DataManager *dataManager, float width, float height, TopoFigure* topoFigure)
{
    m_dataManager = dataManager;
    m_ncols = dataManager->getColsSize();
    m_nrows = dataManager->getRowsSize();
    m_width = width;
    m_height = height;
    m_wScale = m_width/(m_ncols-1);
    m_hScale = m_height/(m_nrows-1);
    m_zScale = 1.0/1000.0;

    for(int i = 0; i < m_nrows; i++){
        for(int j = 0; j < m_ncols; j++){

            GLfloat xc = m_wScale*(j - (m_ncols-1)/2.0f);
            GLfloat yc = m_hScale*((m_nrows-1)/2.0f - i);
            GLfloat zc = dataManager->getValue(i,j);

            if(zc > 0.0f)
            {
                QVector3D color;
                if (zc == COMMERCIAL){
                    color = QVector3D(1.0,0.0,0.0);
                }else if(zc == HIGHRESIDENTIAL){
                    color = QVector3D(0.0,1.0,0.0);
                }else if(zc == LOWRESIDENTIAL){
                    color = QVector3D(0.0,0.0,1.0);
                }

                zc = m_zScale*10;
                float zb = 0.0f;
                if(topoFigure){
                    zb = topoFigure->getZValue(xc,yc);
                }
                //draw a cube
                GLfloat x1 = xc - m_wScale/2.0f;
                GLfloat x2 = xc + m_wScale/2.0f;
                GLfloat y1 = yc - m_hScale/2.0f;
                GLfloat y2 = yc + m_hScale/2.0f;

                //vertices
                QVector3D v1(x1,y1,zc+zb);
                QVector3D v2(x2,y1,zc+zb);
                QVector3D v3(x2,y2,zc+zb);
                QVector3D v4(x1,y2,zc+zb);
                QVector3D v5(x1,y1,zb);
                QVector3D v6(x2,y1,zb);
                QVector3D v7(x2,y2,zb);
                QVector3D v8(x1,y2,zb);

                //top face
                addQuad(v1,v2,v3,v4,color);
                //lateral faces
                addQuad(v1,v5,v6,v2,color);
                addQuad(v2,v6,v7,v3,color);
                addQuad(v3,v7,v8,v4,color);
                addQuad(v4,v8,v5,v1,color);
                //bottom face
                addQuad(v8,v7,v6,v5,color);

            }
        }
    }
}

void OSMFigure::addQuad(QVector3D v1, QVector3D v2, QVector3D v3,QVector3D v4,QVector3D color)
{
    QVector3D n = QVector3D::normal(v2-v1,v3-v1);
    add(v1,n,color);
    add(v2,n,color);
    add(v3,n,color);
    n = QVector3D::normal(v3-v1,v4-v1);
    add(v1,n,color);
    add(v3,n,color);
    add(v4,n,color);
}
