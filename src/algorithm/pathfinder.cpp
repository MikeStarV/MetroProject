#include "../../include/algorithm/pathfinder.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

std::vector<std::string> PathFinder::findShortestPath(int startId, int endId,
                                                    std::vector<Station>& stations) {
    std::unordered_map<int, int> parent;
    std::queue<int> q;

    auto startIt = std::find_if(stations.begin(), stations.end(),
                             [startId](const Station& s) { return s.getId() == startId; });
    auto endIt = std::find_if(stations.begin(), stations.end(),
                            [endId](const Station& s) { return s.getId() == endId; });

    if (startIt == stations.end() || endIt == stations.end()) {
        return {};
    }

    for (auto& station : stations) {
        station.setVisited(false);
    }

    q.push(startId);
    startIt->setVisited(true);
    parent[startId] = -1;

    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        if (currentId == endId) break;

        auto currentIt = std::find_if(stations.begin(), stations.end(),
                                    [currentId](const Station& s) { return s.getId() == currentId; });

        if (currentIt != stations.end()) {
            for (int neighborId : currentIt->getNeighbours()) {
                auto neighborIt = std::find_if(stations.begin(), stations.end(),
                                            [neighborId](const Station& s) { return s.getId() == neighborId; });

                if (neighborIt != stations.end() && !neighborIt->isVisited()) {
                    neighborIt->setVisited(true);
                    parent[neighborId] = currentId;
                    q.push(neighborId);
                }
            }
        }
    }

    std::vector<std::string> path;
    if (parent.find(endId) != parent.end()) {
        std::vector<int> idPath;
        for (int at = endId; at != -1; at = parent[at]) {
            idPath.push_back(at);
        }
        std::reverse(idPath.begin(), idPath.end());

        for (int id : idPath) {
            auto it = std::find_if(stations.begin(), stations.end(),
                                [id](const Station& s) { return s.getId() == id; });
            if (it != stations.end()) {
                path.push_back(it->getName());
            }
        }
    }

    return path;
}
