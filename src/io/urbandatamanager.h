#ifndef URBANDATAMANAGER_H
#define URBANDATAMANAGER_H
#include "datamanager.h"

class UrbanDataManager : public DataManager
{
public:
    UrbanDataManager();
    ~UrbanDataManager();
    void load(const QString filename);

};

#endif // URBANDATAMANAGER_H
