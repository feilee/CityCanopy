#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "io/topodatamanager.h"
#include "io/urbandatamanager.h"
#include "io/osmdatamanager.h"

#include "viz/gl/topofigure.h"
#include "viz/gl/urbanfigure.h"

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TopoDataManager* topoDataManager;
    UrbanDataManager* urbanDataManager;
    OsmDataManager* osmDataManager;
    int urban_set;
    int topo_set;
    int osm_set;

private slots:
    void on_openTopoButton_clicked();
    void on_openUrbanButton_clicked();
    void on_exportButton_clicked();
    void on_openOSMButton_clicked();
    void on_clearButton_clicked();
};

#endif // MAINWINDOW_H
