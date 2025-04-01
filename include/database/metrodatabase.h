#ifndef METRODATABASE_H
#define METRODATABASE_H

#include <sqlite3.h>
#include <vector>
#include "../model/station.h"

class MetroDatabase {
private:
    sqlite3* db;

public:
    MetroDatabase();
    ~MetroDatabase();

    bool open(const std::string& dbPath);
    std::vector<Station> loadStations();
    void loadConnections(std::vector<Station>& stations);
};

#endif // METRODATABASE_H
