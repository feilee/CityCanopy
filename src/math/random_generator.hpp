#pragma once

#include <QtGui/qopengl.h>
#include <QVector3D>
#include <QVector>

#include <QDebug>


struct position
{
    GLfloat x, y, z;

    position()
    {
        x = y = z = -1.0;
    }
    position(GLfloat _x, GLfloat _y, GLfloat _z):
        x(_x), y(_y), z(_z) {};
};

struct data_value
{
    position m_position;
    QVector3D* m_color;

    data_value()
    {
        m_position = position();
        m_color = new QVector3D();
    };

    data_value(position _position, QVector3D* _color):
        m_position(_position), m_color(_color)
    {};
} ;

class temp_values
{
public:
    temp_values
    (
        unsigned _rows,
        unsigned _cols,
        float _h_scale,
        float _w_scale
    ): h_scale(_h_scale), w_scale(_w_scale),
        m_data_matrix(_rows, std::vector<data_value*>(_cols)), n_pieces(20)
    {

    };

    void add(unsigned _row, unsigned _col, GLfloat _x, GLfloat _y, GLfloat _z, QVector3D* _color)
    {
        m_data_matrix[_row][_col] = new data_value( position(_x,_y,_z), _color );
    };

    QVector<QVector3D> generate_pieces()
    {
        for (unsigned row = 0; row < m_data_matrix.size(); row++)
            for (unsigned col = 0; col < m_data_matrix[0].size(); col++)
            {
                if (m_data_matrix[row][col] == 0) continue;
                //draw a cube
                GLfloat x1 = m_data_matrix[row][col]->m_position.x - this->w_scale/2.0f;
                GLfloat x2 = m_data_matrix[row][col]->m_position.x + this->w_scale/2.0f;
                GLfloat y1 = m_data_matrix[row][col]->m_position.y - this->h_scale/2.0f;
                GLfloat y2 = m_data_matrix[row][col]->m_position.y + this->h_scale/2.0f;

                // subdivide into p pieces to randomize building apparance
                // test in pieces < 49 = max_pieces on pc without cuda
                for ( unsigned p = 0; p < n_pieces; p++ )
                {
                    GLfloat x_step = ( x2 - x1 ) / n_pieces;
                    GLfloat y_step = ( y2 - y1 ) / n_pieces;

                    // generation of vertices
                    // x2 changed to x1 + p*x_step => analogue to y2
                    // we have to generate p*p pieces
                    for ( unsigned q = 0; q < n_pieces; q++ )
                    {
                        // with probability 80% we applicate new height
                        GLfloat z_step = ( std::rand() %100>80 ) ? 0.002f + ( 1.0 * std::rand() ) / ( 40.0 * RAND_MAX ) : 0.002f;
                        //qDebug() << "rand z" << z_step;

                        QVector3D v1 ( x1 + p*x_step           , y1 + q*y_step           , m_data_matrix[row][col]->m_position.z +z_step );
                        QVector3D v2 ( x1 + ( ( p+1 ) *x_step ), y1 + q*y_step           , m_data_matrix[row][col]->m_position.z +z_step );
                        QVector3D v3 ( x1 + ( ( p+1 ) *x_step ), y1 + ( ( q+1 ) *y_step ), m_data_matrix[row][col]->m_position.z +z_step );
                        QVector3D v4 ( x1 + p*x_step           , y1 + ( ( q+1 ) *y_step ), m_data_matrix[row][col]->m_position.z +z_step );
                        QVector3D v5 ( x1 + p*x_step           , y1 + q*y_step           , m_data_matrix[row][col]->m_position.z );
                        QVector3D v6 ( x1 + ( ( p+1 ) *x_step ), y1 + q*y_step           , m_data_matrix[row][col]->m_position.z );
                        QVector3D v7 ( x1 + ( ( p+1 ) *x_step ), y1 + ( ( q+1 ) *y_step ), m_data_matrix[row][col]->m_position.z );
                        QVector3D v8 ( x1 + p*x_step           , y1 + ( ( q+1 ) *y_step ), m_data_matrix[row][col]->m_position.z );

                        QVector3D new_color = ( z_step>0.0 ) ? *m_data_matrix[row][col]->m_color + QVector3D ( 0.0+ ( p*0.01 ) + ( q*0.01 ),0.0+ ( p*0.01 ) + ( q*0.01 ),0.0+ ( p*0.01 ) + ( q*0.01 ) ) : QVector3D ( 0.38,0.38,0.38 );

                        pieces.push_back(v1);
                        pieces.push_back(v2);
                        pieces.push_back(v3);
                        pieces.push_back(v4);
                        pieces.push_back(v5);
                        pieces.push_back(v6);
                        pieces.push_back(v7);
                        pieces.push_back(v8);
                        pieces.push_back(new_color);
                    }
                }
            }

        return pieces;
    };

private:
    float h_scale;
    float w_scale;
    std::vector<std::vector<data_value*>> m_data_matrix;
    QVector<QVector3D> pieces;
    unsigned n_pieces;

};
