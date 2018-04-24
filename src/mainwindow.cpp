#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viz/gl/glwidget.h"
#include "io/wrfexporter.h"
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
       QMainWindow(parent),
       ui(new Ui::MainWindow),
       urban_set(0),
       topo_set(0)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openTopoButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    if (fileName != ""){
        qDebug() << "Opening :" << fileName;
        topoDataManager = new TopoDataManager;
        topoDataManager->load(fileName);
        ui->openGLWidget->initFigure(topoDataManager);
        topo_set =1 ;
    }
}

void MainWindow::on_openUrbanButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    if(fileName != ""){
        qDebug() << "Opening :" << fileName;
        urbanDataManager = new UrbanDataManager;
        urbanDataManager->load(fileName);
        ui->openGLWidget->initFigure(urbanDataManager);
        urban_set = 1;
    }
}

void MainWindow::on_exportButton_clicked()
{
    if(osm_set > 0){
        QString pathToExport = QFileDialog::getExistingDirectory();
        if(pathToExport != "" ){
            QDir::setCurrent(pathToExport);
            WRFExporter wrfExporter;
            wrfExporter.exportData(osmDataManager);
        }
        /*
    if( urban_set > 0){
        QString pathToExport = QFileDialog::getExistingDirectory();
        if(pathToExport != "" ){
            QDir::setCurrent(pathToExport);
            WRFExporter wrfExporter;
            wrfExporter.exportData(urbanDataManager);
        }*/
    }else{
        QMessageBox::information(this,tr("Error exporting"),tr("No osm urban data loaded"));
    }
}

void MainWindow::on_openOSMButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    if(fileName != ""){
        qDebug() << "Opening :" << fileName;
        osmDataManager = new OsmDataManager;
        osmDataManager->load(fileName);
        ui->openGLWidget->initFigure(osmDataManager);
        osm_set = 1;
    }
}

void MainWindow::on_clearButton_clicked()
{
    ui->openGLWidget->clearFigure();
    if(osm_set)
        delete(osmDataManager);
    if(urban_set)
        delete(urbanDataManager);
    if(topo_set)
        delete(topoDataManager);
}
