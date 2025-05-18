#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <clocale>
#include "database/metrodatabase.h"
#include "model/station.h"
#include "algorithm/pathfinder.h"

bool isRussianUpper(const std::string& str, size_t pos) {
    if (pos + 1 >= str.size()) return false;

    const unsigned char c1 = str[pos];
    const unsigned char c2 = str[pos+1];

    return (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0x9F) ||
           (c1 == 0xD0 && c2 >= 0xA0 && c2 <= 0xAF) ||
           (c1 == 0xD1 && c2 >= 0x80 && c2 <= 0x8F);
}

void convertRussianUpperToLower(std::string& result, const std::string& str, size_t& pos) {
    if (pos + 1 >= str.size()) return;

    const unsigned char c1 = str[pos];
    const unsigned char c2 = str[pos+1];

    if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0x9F) {
        result += c1;
        result += static_cast<char>(c2 + 0x20);
        pos += 2;
    }
    else if (c1 == 0xD0 && c2 >= 0xA0 && c2 <= 0xAF) {
        result += 0xD1;
        result += static_cast<char>(c2 - 0x20);
        pos += 2;
    }
    else if (c1 == 0xD0 && c2 == 0x81) {
        result += 0xD1;
        result += 0x91;
        pos += 2;
    }
    else {
        result += str[pos++];
    }
}

std::string toLower(const std::string& str) {
    std::string result;
    result.reserve(str.size());

    for (size_t i = 0; i < str.size(); ) {
        unsigned char c = str[i];

        if ((c & 0x80) == 0) {
            if (c >= 'A' && c <= 'Z') {
                result += static_cast<char>(c + 32);
            } else {
                result += c;
            }
            i++;
        }
        else if (isRussianUpper(str, i)) {
            convertRussianUpperToLower(result, str, i);
        }
        else {
            size_t char_len = 1;
            if ((c & 0xE0) == 0xC0) char_len = 2;
            else if ((c & 0xF0) == 0xE0) char_len = 3;
            else if ((c & 0xF8) == 0xF0) char_len = 4;

            for (size_t j = 0; j < char_len && i < str.size(); j++) {
                result += str[i++];
            }
        }
    }

    return result;
}

int main() {
    std::setlocale(LC_ALL, "");

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

    std::string lowerStart = toLower(startName);
    std::string lowerEnd = toLower(endName);

    int startId = -1, endId = -1;
    for (const auto& station : stations) {
        std::string stationLower = toLower(station.getName());
        if (stationLower == lowerStart) startId = station.getId();
        if (stationLower == lowerEnd) endId = station.getId();
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
