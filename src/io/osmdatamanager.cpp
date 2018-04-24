
#include "io/osmdatamanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QtMath>
#define COMMERCIAL 33
#define HIGHRESIDENTIAL 32
#define LOWRESIDENTIAL 31
#define NONE 0

OsmDataManager::OsmDataManager()
{
    cellsize = 0.008333;
    NODATA_value = -9999;
    ncols = 0;
    nrows = 0;
}

OsmDataManager::~OsmDataManager()
{

}

void OsmDataManager::load(const QString filename){
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QXmlStreamReader xml(&file);

    if(xml.readNextStartElement()){
        if(xml.name() == "osm"){
            while(xml.readNextStartElement()){
                QString name = xml.name().toString();
                if(name == "bounds"){
                    xllcorner = xml.attributes().value("minlon").toFloat();
                    yllcorner = xml.attributes().value("minlat").toFloat();
                    xurcorner = xml.attributes().value("maxlon").toFloat();
                    yurcorner = xml.attributes().value("maxlat").toFloat();
                    xml.skipCurrentElement();
                }
                else if(name == "node"){
                    addNode(xml);
                }
                else if(name == "way"){
                    addWay(xml);
                }
                else{
                    xml.skipCurrentElement();
                }
            }
        }
    }
    xml.clear();
    file.close();

    generateGrid();
}

void OsmDataManager::addNode(QXmlStreamReader& xml){
    QXmlStreamAttributes attr = xml.attributes();
    QString id = attr.value("id").toString();
    float lat = attr.value("lat").toString().toFloat();
    float lon = attr.value("lon").toString().toFloat();
    QList<float> nodes;
    nodes.append(lat);
    nodes.append(lon);
    m_nodes[id] = nodes;
    xml.skipCurrentElement();
}

void OsmDataManager::addWay(QXmlStreamReader &xml){
    QList< QString > polygon;
    int landuse = NONE;
    while(xml.readNextStartElement()){
        QString name = xml.name().toString();
        if(name == "nd"){
            QString id = xml.attributes().value("ref").toString();
            polygon.append(id);
            xml.skipCurrentElement();
        }
        else if(name == "tag"){
            QString kAttr = xml.attributes().value("k").toString();
            QString vAttr = xml.attributes().value("v").toString();
            if(kAttr == "building"){
                if(vAttr == "commercial")
                    landuse = COMMERCIAL;
                else if(vAttr == "residential")
                    landuse = HIGHRESIDENTIAL;
                else
                    landuse = LOWRESIDENTIAL;
            }
            xml.skipCurrentElement();
        }
        else{
            xml.skipCurrentElement();
        }
    }
    //Calculate centroid 2d
    if(landuse != NONE){
        float avg_lat = 0.0;
        float avg_lon = 0.0;
        float total = (float)polygon.length();
        //polygon[0]=polygon[total-1] so skip the last node in the list
        for(int k = 0; k < total-1 ; k++){
            QString ref = polygon[k];
            QList<float> aux = m_nodes[ref];
            avg_lat += aux[0];
            avg_lon += aux[1];
        }
        avg_lat = avg_lat/(total-1);
        avg_lon = avg_lon/(total-1);
        UrbElement element(landuse,avg_lat,avg_lon);
        m_urbelements.append(element);
    }
}

void OsmDataManager::generateGrid(){
    float width = xurcorner-xllcorner;
    float height = yurcorner-yllcorner;
    ncols = qRound(width/cellsize) + 1;
    nrows = qRound(height/cellsize) + 1;
    int length = ncols*nrows;
    m_data.resize(length);
    QVector<int> countLowRes;
    QVector<int> countHiRes;
    QVector<int> countComm;
    countLowRes.resize(length);
    countHiRes.resize(length);
    countComm.resize(length);

    foreach(UrbElement element, m_urbelements){
        float lat = element.getLat();
        float lon = element.getLon();
        int landuse = element.getLanduse();
        QList< int > indexes = getIndexesByCoord(lat,lon);
        if(landuse == COMMERCIAL)
            countComm[ncols*indexes[0] + indexes[1]] +=1;
        else if(landuse == HIGHRESIDENTIAL)
            countHiRes[ncols*indexes[0] + indexes[1]] +=1;
        else if(landuse == LOWRESIDENTIAL)
            countLowRes[ncols*indexes[0] + indexes[1]] +=1;
    }

    for(int k = 0 ; k < length ; k++){
        int numComm = countComm[k];
        int numHiRes = countHiRes[k];
        int numLowRes = countLowRes[k];
        int total = numComm + numHiRes + numLowRes;

        //qDebug() << "Comm: " << numComm << " Hires: " << numHiRes << " LowRes:" << numLowRes << " Total: " << total;


        if(numComm > 0)
            m_data[k] = COMMERCIAL;
        else if(numHiRes > 0)
            m_data[k] = HIGHRESIDENTIAL;
        else if(total > 0)
            m_data[k] = LOWRESIDENTIAL;
        else
            m_data[k] = NONE;

    }

}

QList<int> OsmDataManager::getIndexesByCoord(float lat, float lon){
    int index_lat = qRound((yurcorner - lat)/cellsize);
    int index_lon = qRound((lon - xllcorner)/cellsize);
    if(index_lat < 0) index_lat = 0;
    if(index_lon < 0) index_lon = 0;
    QList<int> indexes;
    indexes.append(index_lat);
    indexes.append(index_lon);
    return indexes;
}


