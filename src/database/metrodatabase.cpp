#include "../../include/database/metrodatabase.h"
#include <algorithm>

MetroDatabase::MetroDatabase() : db(nullptr) {}

MetroDatabase::~MetroDatabase() {
    if (db) sqlite3_close(db);
}

bool MetroDatabase::open(const std::string& dbPath) {
    return sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK;
}

std::vector<Station> MetroDatabase::loadStations() {
    std::vector<Station> stations;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name FROM stations;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            stations.emplace_back(id, name);
        }
        sqlite3_finalize(stmt);
    }
    return stations;
}

void MetroDatabase::loadConnections(std::vector<Station>& stations) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT station_from, station_to FROM connections;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int from = sqlite3_column_int(stmt, 0);
            int to = sqlite3_column_int(stmt, 1);

            auto itFrom = std::find_if(stations.begin(), stations.end(),
                [from](const Station& s) { return s.getId() == from; });

            auto itTo = std::find_if(stations.begin(), stations.end(),
                [to](const Station& s) { return s.getId() == to; });

            if (itFrom != stations.end() && itTo != stations.end()) {
                itFrom->addNeighbour(to);
                itTo->addNeighbour(from);
            }
        }
        sqlite3_finalize(stmt);
    }
}
