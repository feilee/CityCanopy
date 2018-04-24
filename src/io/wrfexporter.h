#ifndef WRFEXPORTER_H
#define WRFEXPORTER_H
#include "io/exporter.h"
#include "datamanager.h"

class WRFExporter : public Exporter
{
public:
    WRFExporter();
    ~WRFExporter();
    void exportData(DataManager* dataManager);

private:
    void writeData(float *data, int ncols, int nrows);
    void writeIndexData(int ncols,int nrows,float lonMin,float latMin,float cellsize);
};

#endif // WRFEXPORTER_H
