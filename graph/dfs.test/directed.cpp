// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#include "../digraph.hpp"
#include "../dfs.hpp"

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

  directed_dfs<G> dfs(g);
  dfs();

  // TODO: Actually verify properties of the search.
  for (vertex_t v : g.vertices())
    std::cout << v << ": " << dfs.parents[v] << '\n';
}
