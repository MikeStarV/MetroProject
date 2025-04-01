#include <iostream>
#include <vector>
#include "database/metrodatabase.h"
#include "model/station.h"
#include "algorithm/pathfinder.h"

int main() {
    MetroDatabase metroDB;
    if (!metroDB.open("metro.db")) {
        std::cerr << "Не удалось открыть базу данных" << std::endl;
        return 1;
    }

    auto stations = metroDB.loadStations();
    metroDB.loadConnections(stations);

    std::string startName, endName;
    std::cout << "Введите начальную станцию: ";
    std::getline(std::cin, startName);
    std::cout << "Введите конечную станцию: ";
    std::getline(std::cin, endName);

    int startId = -1, endId = -1;
    for (const auto& station : stations) {
        if (station.getName() == startName) startId = station.getId();
        if (station.getName() == endName) endId = station.getId();
    }

    if (startId == -1 || endId == -1) {
        std::cerr << "Одна из станций не найдена" << std::endl;
        return 1;
    }

    auto path = PathFinder::findShortestPath(startId, endId, stations);

    if (path.empty()) {
        std::cout << "Путь не найден!" << std::endl;
    } else {
        std::cout << "Кратчайший маршрут (" << path.size()-1 << " переходов): ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i != path.size()-1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
