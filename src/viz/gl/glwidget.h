#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "datamanager.h"
#include "io/topodatamanager.h"
#include "io/urbandatamanager.h"
#include "io/osmdatamanager.h"

#include "viz/gl/figure.h"
#include "viz/gl/topofigure.h"
#include "viz/gl/urbanfigure.h"
#include "viz/gl/osmfigure.h"


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void initFigure(QSharedPointer<Figure> figure);
    void initFigure(TopoDataManager* dataManager);
    void initFigure(UrbanDataManager* dataManager);
    void initFigure(OsmDataManager* dataManager);
    void clearFigure();
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:

    bool m_core;
    float m_scale;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    int m_nfigures;
    float m_cameraPosZ;
    QPoint m_lastPos;
    QVector< QSharedPointer<Figure> > m_figures_list;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    bool m_transparent;
    UrbanFigure* m_urbanFigure;
    TopoFigure* m_topoFigure;
    OSMFigure* m_osmFigure;
};

#endif
