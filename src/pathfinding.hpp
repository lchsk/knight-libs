#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <limits>
#include <queue>

#include "hex.hpp"

namespace K {

struct MapNode {
    Hex target;
    double weight;
    MapNode(const Hex &target, double weight)
        : target(target), weight(weight) {}
};

class PathFinder {
  public:
    void find_paths(const K::Hex &source);

    const std::list<K::Hex> get_shortest_path_to(const K::Hex &h);

    double get_min_distance(const K::Hex &hex) const;

    std::unordered_map<K::Hex, std::vector<K::MapNode>> nodes;

  private:
    constexpr static double max_weight =
        std::numeric_limits<double>::infinity();

    std::unordered_map<K::Hex, double> min_distance;
    std::unordered_map<K::Hex, K::Hex> previous;
};
}
