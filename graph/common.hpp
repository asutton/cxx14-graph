// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_COMMON_HPP
#define GRAPH_COMMON_HPP

#include "utility.hpp"

#include <cstddef>


namespace origin {

// Used to indicate the absence of labels on graphs and edges.
struct empty { };

// Represents a vertex within a graph.
using vertex = std::size_t;

// Represents an edge within a graph.
using edge = std::size_t;

// A pair of vertices representing an edge.
using vertex_pair = std::pair<vertex, vertex>;

} // namespace origin
