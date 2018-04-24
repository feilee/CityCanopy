#ifndef OSMDATAMANAGER_H
#define OSMDATAMANAGER_H
#include "datamanager.h"
#include <QMap>
#include <QXmlStreamReader>
#include <QDebug>

class UrbElement{
public:
    UrbElement(){}
    UrbElement(int l,float lat,float lon){
        m_landuse = l;
        m_lat = lat;
        m_lon = lon;
    }
    ~UrbElement(){}
    int getLanduse(){return m_landuse;}
    float getLat(){return m_lat;}
    float getLon(){return m_lon;}
    void setLanduse(int l){m_landuse = l;}
    void setLat(float lat){m_lat = lat;}
    void setLon(float lon){m_lon = lon;}
private:
    int m_landuse;
    float m_lat;
    float m_lon;
};

class OsmDataManager : public DataManager
{    
public:
    OsmDataManager();
    ~OsmDataManager();
    void load(const QString filename);
private:
    float xurcorner;
    float yurcorner;
    QMap< QString,QList<float> > m_nodes;
    QVector< UrbElement > m_urbelements;
    void addNode(QXmlStreamReader& xml);
    void addWay(QXmlStreamReader& xml);
    void generateGrid();
    QList<int> getIndexesByCoord(float lat,float lon);
};



#endif // OSMDATAMANAGER_H
