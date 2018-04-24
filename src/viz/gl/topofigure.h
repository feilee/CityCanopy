#ifndef TOPOFIGURE_H
#define TOPOFIGURE_H

#include "viz/gl/figure.h"

#include "datamanager.h"

class TopoFigure : public Figure
{
public:
    TopoFigure();
    ~TopoFigure();
    void load(DataManager* dataManager,float width, float height);
    float getZValue(float x, float y);

private:
    void finalNormals();
};

#endif // TOPOFIGURE_H
