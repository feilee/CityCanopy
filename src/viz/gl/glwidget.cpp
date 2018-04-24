
#include "viz/gl/glwidget.h"

#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

#include "ui_mainwindow.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_scale(1.0),
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_nfigures(0),
      m_program(0),
      m_urbanFigure(0),
      m_topoFigure(0)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
    if (m_transparent)
        setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{

}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        update();
    }
}

void GLWidget::initFigure(TopoDataManager* dataManager){
    m_topoFigure = new TopoFigure;
    m_topoFigure->load(dataManager,3,3);
    initFigure(QSharedPointer<Figure>(m_topoFigure));
}

void GLWidget::initFigure(UrbanDataManager* dataManager){
    m_urbanFigure = new UrbanFigure;
    if(m_topoFigure){
        m_urbanFigure->load(dataManager,3,3,m_topoFigure);
    }else{
        m_urbanFigure->load(dataManager,3,3);
    }
    initFigure(QSharedPointer<Figure>(m_urbanFigure));
}

void GLWidget::initFigure(OsmDataManager* dataManager){
    m_osmFigure = new OSMFigure;
    if(m_topoFigure){
        m_osmFigure->load(dataManager,3,3,m_topoFigure);
    }else{
        m_osmFigure->load(dataManager,3,3);
    }
    initFigure(QSharedPointer<Figure>(m_osmFigure));
}

void GLWidget::initFigure(QSharedPointer<Figure> figure){
    figure->initOpenGLObjects();
    m_figures_list.append(figure);
    m_nfigures++;
    update();
}

void GLWidget::clearFigure(){
    m_nfigures=0;
    m_figures_list.clear();
    update();
}

void GLWidget::initializeGL()
{

    initializeOpenGLFunctions();
    glClearColor(0.2, 0.2, 0.2, 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/glsl/vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/glsl/fragment.fsh");
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->bindAttributeLocation("color", 2);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    m_cameraPosZ = 5.0f;

    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 30));
    m_program->release();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.scale(m_scale, m_scale, m_scale);
    m_world.rotate(m_xRot / 16.0f, 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(0,0,m_cameraPosZ),QVector3D(0,0,0),QVector3D(0,1,0));

    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    for(int k = 0; k < m_nfigures ; ++k){
        m_figures_list[k]->draw();
    }
    m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 50.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_P )
        {
            m_cameraPosZ -= 1.0f;
            update();
        }
    else if( event->key() == Qt::Key_O )
        {
            m_cameraPosZ += 1.0f;
            update();
        }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
 QPoint numDegrees = event->angleDelta();
 if (numDegrees.y() < 0)  m_scale = m_scale/1.1;
 if (numDegrees.y() > 0)  m_scale = m_scale*1.1;

  update(); // call paintGL()
}
