#include "../../include/model/station.h"

Station::Station(int id, std::string name)
    : id(id), name(name), visited(false) {}

void Station::addNeighbour(int neighbourId) {
    neighbours.push_back(neighbourId);
}

int Station::getId() const { return id; }
std::string Station::getName() const { return name; }
const std::vector<int>& Station::getNeighbours() const { return neighbours; }
bool Station::isVisited() const { return visited; }
void Station::setVisited(bool v) { visited = v; }
