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
using vertex_t = std::size_t;

// Represents an edge within a graph.
using edge_t = std::size_t;

// A list of incident edges.
using edge_list = std::vector<edge_t>;


// Construct a label over a vector.
template<typename T>
auto vertex_label(std::vector<T>& vec) {
  return [&vec](vertex_t v) -> T& { return vec[v]; };
}

// Construct a label over a vector.
template<typename T>
auto edge_label(std::vector<T>& vec) {
  return [&vec](edge_t e) -> T& { return vec[e]; };
}


} // namespace origin

#endif
