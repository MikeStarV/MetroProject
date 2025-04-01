#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <string>
#include "../model/station.h"

class PathFinder {
public:
    static std::vector<std::string> findShortestPath(int startId, int endId,
                                                   std::vector<Station>& stations);
};

#endif // PATHFINDER_H
