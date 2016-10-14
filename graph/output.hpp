// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_OUTPUT_HPP
#define GRAPH_OUTPUT_HPP

#include "common.hpp"

#include <iostream>


namespace origin
{

template<typename G>
struct print_digraph
{
  print_digraph(std::ostream& os, G const& g)
    : os(os), graph(g)
  { }

  void operator()() {
    os << "digraph {\n";

    // TODO: Factor vertex printing into a template parameter.
    for (vertex_t v : graph.vertices())
      print_vertex(v);

    // TODO: Factor edge printing into a template parameter.
    for (edge_t e : graph.edges())
      print_edge(e);

    os << "}\n";
  }

  void print_vertex(vertex_t v) {
    os << v << '\n';
  }

  void print_edge(edge_t e) {
    os << graph.source(e) << " -> " << graph.target(e) << '\n';
  }

  std::ostream& os;
  G const& graph;
};


template<typename G>
struct print_graph
{
  print_graph(std::ostream& os, G const& g)
    : os(os), graph(g)
  { }

  void operator()() {
    os << "graph {\n";

    // TODO: Factor vertex printing into a template parameter.
    for (vertex_t v : graph.vertices())
      print_vertex(v);

    // TODO: Factor edge printing into a template parameter.
    for (edge_t e : graph.edges())
      print_edge(e);

    os << "}\n";
  }

  void print_vertex(vertex_t v) {
    os << v << '\n';
  }

  void print_edge(edge_t e) {
    os << graph.first(e) << " -- " << graph.second(e) << '\n';
  }

  std::ostream& os;
  G const& graph;
};


} // namespace origin


#endif
