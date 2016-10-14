// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include "../digraph.hpp"
#include "../output.hpp"

#include <cassert>
#include <iostream>


using namespace origin;


int
main()
{
  using G = digraph<char, int>;
  G g;
  vertex_t v[] {
    g.add_vertex('a'), // 0
    g.add_vertex('b'), // 1
    g.add_vertex('c'), // 2
    g.add_vertex('d'), // 3
    g.add_vertex('e'), // 4
    g.add_vertex('f')  // 5
  };
  edge_t e[] {
    g.add_edge(v[0], v[1], 0), // a -> b
    g.add_edge(v[0], v[3], 1), // a -> d
    g.add_edge(v[1], v[2], 2), // b -> c
    g.add_edge(v[1], v[3], 3), // b -> d
    g.add_edge(v[2], v[0], 4), // c -> a
    g.add_edge(v[3], v[2], 5), // d -> c
    g.add_edge(v[4], v[5], 6), // e -> f
  };

  assert(g.num_vertices() == 6);
  assert(g.num_edges() == 7);

  assert(g.out_degree(0) == 2);
  assert(g.in_degree(0) == 1);

  assert(g.out_degree(1) == 2);
  assert(g.in_degree(1) == 1);

  assert(g.out_degree(2) == 1);
  assert(g.in_degree(2) == 2);

  assert(g.out_degree(3) == 1);
  assert(g.in_degree(3) == 2);

  assert(g.out_degree(4) == 1);
  assert(g.in_degree(4) == 0);

  assert(g.out_degree(5) == 0);
  assert(g.in_degree(5) == 1);

  // TODO: Verify out- and in-edge lists for the graph.

  assert(g.has_edge(0, 1));
  assert(g.source(0) == 0);
  assert(g.target(0) == 1);
  
  assert(g.has_edge(0, 3));
  assert(g.source(1) == 0);
  assert(g.target(1) == 3);
  
  assert(g.has_edge(1, 2));
  assert(g.source(2) == 1);
  assert(g.target(2) == 2);
  
  assert(g.has_edge(1, 3));
  assert(g.source(3) == 1);
  assert(g.target(3) == 3);
  
  assert(g.has_edge(2, 0));
  assert(g.source(4) == 2);
  assert(g.target(4) == 0);
  
  assert(g.has_edge(3, 2));
  assert(g.source(5) == 3);
  assert(g.target(5) == 2);
  
  assert(g.has_edge(4, 5));
  assert(g.source(6) == 4);
  assert(g.target(6) == 5);

  print_digraph<G> print(std::cout, g);
  print();
}
