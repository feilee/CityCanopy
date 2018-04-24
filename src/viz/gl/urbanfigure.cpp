#include "math/cuda_code.cuh"
#include "math/random_generator.hpp"

#include "viz/gl/urbanfigure.h"


#include <QVector3D>
#include <QDebug>

UrbanFigure::UrbanFigure()
{

}

UrbanFigure::~UrbanFigure()
{

}

void UrbanFigure::load ( DataManager *dataManager, float width, float height )
{
    load ( dataManager,width,height, 0 );
}

/// Always width = height = 3 -> arbritrary w-h-scale ???

void UrbanFigure::load ( DataManager *dataManager, float width, float height, TopoFigure* topoFigure )
{
    m_dataManager = dataManager;
    m_ncols = dataManager->getColsSize();
    m_nrows = dataManager->getRowsSize();
    m_width = width;
    m_height = height;
    m_wScale = m_width/ ( m_ncols-1 );
    m_hScale = m_height/ ( m_nrows-1 );
    m_zScale = 1.0/1000.0;

    temp_values* values = new temp_values(m_nrows, m_ncols, m_hScale, m_wScale);

    for ( int i = 0; i < m_nrows; i++ ) {
        for ( int j = 0; j < m_ncols; j++ ) {

            GLfloat xc = m_wScale * ( j - ( m_ncols-1 ) /2.0f );
            GLfloat yc = m_hScale * ( ( m_nrows-1 ) /2.0f - i );
            GLfloat zc = dataManager->getValue ( i,j );

            if ( zc > 0.0f ) {
                QVector3D* color;
                if ( zc >= 10 ) {
                    color = new QVector3D ( 0.76,0.52,0.52 );
                } else if ( zc >= 7.5 ) {
                    color = new QVector3D ( 0.27,0.39,0.21 );
                } else {
                    color = new QVector3D ( 0.25,0.28,0.50 );
                }

                // zc = zc * m_zScale; // unused!

                // get base height of topography
                GLfloat z_h = ( topoFigure )? topoFigure->getZValue ( xc, yc ): 0.0f;

                values->add(i,j, xc, yc, z_h, color);

            } // if zc > 0
        } // for cols
    } // for rows

    QVector<QVector3D> pieces = values->generate_pieces();

    for (QVector<QVector3D>::iterator it = pieces.begin(); it != pieces.end(); it += 9)
    {
        //top face
        addQuad ( *(it)  , *(it+1), *(it+2), *(it+3), *(it+8) ); //v1,v2,v3,v4,new_color );
        //lateral faces
        addQuad ( *(it)  , *(it+4), *(it+5), *(it+1), *(it+8) ); //v1,v5,v6,v2,new_color );
        addQuad ( *(it+1), *(it+5), *(it+6), *(it+2), *(it+8) ); //v2,v6,v7,v3,new_color );
        addQuad ( *(it+2), *(it+6), *(it+7), *(it+3), *(it+8) ); //v3,v7,v8,v4,new_color );
        addQuad ( *(it+3), *(it+7), *(it+4), *(it)  , *(it+8) ); //v4,v8,v5,v1,new_color );

        addQuad ( *(it+7), *(it+6), *(it+5), *(it+4), *(it+8) ); //v8,v7,v6,v5,new_color );

    }




}

void UrbanFigure::addQuad ( QVector3D v1, QVector3D v2, QVector3D v3,QVector3D v4,QVector3D color )
{
    QVector3D n = QVector3D::normal ( v2-v1,v3-v1 );
    add ( v1,n,color );
    add ( v2,n,color );
    add ( v3,n,color );
    n = QVector3D::normal ( v3-v1,v4-v1 );
    add ( v1,n,color );
    add ( v3,n,color );
    add ( v4,n,color );
}

