#include "pathfinding.hpp"

namespace K {
void PathFinder::find_paths(const K::Hex &source) {
    min_distance.clear();
    previous.clear();

    for (const auto &node_item : nodes) {
        for (const auto &node : node_item.second) {
            min_distance[node.target] = max_weight;
        }
    }

    min_distance[source] = 0;

    std::queue<std::pair<double, K::Hex>> queue;

    queue.push({min_distance[source], source});

    while (!queue.empty()) {
        auto node_item = queue.front();
        double dist = node_item.first;
        K::Hex hex = node_item.second;
        queue.pop();

        if (dist > min_distance[hex])
            continue;

        const std::vector<K::MapNode> &neighbors = nodes[hex];

        for (auto neighbor_iter = neighbors.cbegin();
             neighbor_iter != neighbors.cend(); neighbor_iter++) {
            auto target = neighbor_iter->target;
            double weight = neighbor_iter->weight;
            double distance_through_u = dist + weight;

            if (distance_through_u < min_distance[target]) {
                min_distance[target] = distance_through_u;
                previous[target] = hex;
                queue.push({min_distance[target], target});
            }
        }
    }
}

const std::list<K::Hex> PathFinder::get_shortest_path_to(const K::Hex &h) {
    K::Hex hex = h;
    std::list<K::Hex> path;

    path.push_front(hex);

    while (previous.find(hex) != previous.end()) {
        hex = previous.at(hex);
        path.push_front(hex);
    }

    return path;
}

double PathFinder::get_min_distance(const K::Hex &hex) const {
    if (min_distance.find(hex) == min_distance.end()) {
        return -1;
    }

    return min_distance.at(hex);
}
}
