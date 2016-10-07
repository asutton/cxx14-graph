// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_COMMON_HPP
#define GRAPH_COMMON_HPP

#include "utility.hpp"

#include <cstddef>
#include <vector>


namespace origin {

// Used to indicate the absence of labels on graphs and edges.
struct empty { };

// Represents a vertex within a graph.
using vertex = std::size_t;

// Represents an edge within a graph.
using edge = std::size_t;

// A list of incident edges.
using edge_list = std::vector<edge>;


// Construct a label over a vector.
template<typename T>
auto vertex_label(std::vector<T>& vec) {
  return [&vec](vertex v) -> T& { return vec[v]; };
}


} // namespace origin

#endif
