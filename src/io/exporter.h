#ifndef EXPORTER_H
#define EXPORTER_H
#include "datamanager.h"

class Exporter
{
public:

    Exporter();
    ~Exporter();
    void exportData(DataManager* dataManager);

protected:
    QVector<float> m_data;

};

#endif // EXPORTER_H
