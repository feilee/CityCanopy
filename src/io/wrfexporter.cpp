#include "io/wrfexporter.h"
#include "io/write_geogrid.c"
#include <QDebug>

WRFExporter::WRFExporter()
{

}

WRFExporter::~WRFExporter()
{

}

void WRFExporter::exportData(DataManager* dataManager){
    int ncols = dataManager->getColsSize();
    int nrows = dataManager->getRowsSize();
    float lonMin = dataManager->getLonMin();
    float latMin = dataManager->getLatMin();
    float cellsize = dataManager->getCellSize();

    writeData(dataManager->getData().data(),ncols,nrows);
    writeIndexData(ncols,nrows,lonMin,latMin,cellsize);
}

void WRFExporter::writeIndexData(int ncols,int nrows,float lonMin,float latMin,float cellsize){
    QFile file;
    file.setFileName("index");
    if( file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out << "type=categorical" << endl;
        out << "category_min=31" << endl;
        out << "category_max=33" << endl;
        out << "projection=regular_ll" << endl;
        out << "dx=" << cellsize << endl;
        out << "dy=" << cellsize << endl;
        out << "known_x=1.0" << endl;
        out << "known_y=1.0" << endl;
        out << "known_lat=" << latMin << endl;
        out << "known_lon=" << lonMin << endl;
        out << "wordsize=1" << endl;
        out << "tile_x=" << ncols << endl;
        out << "tile_y=" << nrows << endl;
        out << "tile_z=1" << endl;
        out << "missing_value = 0" << endl;
        out << "units=\"category\"" << endl;
        out << "description=\"3-category urban LU\"" << endl;
        file.close();
    }
}

void WRFExporter::writeData(float *data, int ncols, int nrows){
    int nz = 1;
    int isigned = 0;
    int endian = 1;
    float scalefactor = 1.0;
    int wordsize = 1;

    m_data.resize(ncols * nrows);
    float* p = m_data.data();

    for (int i = nrows-1 ; i >= 0 ; i--){
        for (int j = 0; j < ncols ; j++){
            float value = data[i*ncols + j];
            if(value == 33 || value == 32 || value == 31)
                *p++ = value;
            else
                *p++ = 0;
            /*
            if(value > 0)
                *p++ = 31;
            else
                *p++ = 0;

            
            if(value >= 10.0)
                *p++ = 33;
            else if(value >= 7.5)
                *p++ = 32;
            else if(value > 0)
                *p++ = 31;
            else
                *p++ = 0;
            */
        }
    }
    write_geogrid(m_data.data(),&ncols,&nrows,&nz,&isigned,&endian,&scalefactor,&wordsize);

}

