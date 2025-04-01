#ifndef STATION_H
#define STATION_H

#include <string>
#include <vector>

class Station {
private:
    int id;
    std::string name;
    std::vector<int> neighbours;
    bool visited;

public:
    Station(int id = -1, std::string name = "");

    void addNeighbour(int neighbourId);
    int getId() const;
    std::string getName() const;
    const std::vector<int>& getNeighbours() const;
    bool isVisited() const;
    void setVisited(bool v);
};

#endif // STATION_H
