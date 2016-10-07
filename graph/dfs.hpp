// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_DFS_HPP
#define GRAPH_DFS_HPP

#include "common.hpp"


namespace origin {

// A basic DFS implementation for directed graphs.
template<typename G>
struct directed_dfs
{
  directed_dfs(G& g)
    : graph(g), 
      colors(graph.num_vertices(), 0),
      pre_times(graph.num_vertices(), -1),
      post_times(graph.num_vertices(), -1),
      parents(graph.num_vertices()),
      clock(0)
  { }


  void operator()()
  {
    auto color = vertex_label(colors);
    auto pre = vertex_label(pre_times);
    auto post = vertex_label(post_times);
    auto parent = vertex_label(parents);

    // Extra initialization.
    for (vertex v : graph.vertices())
      parent(v) = v;
    
    search(color, pre, post, parent);
  }


  template<typename L1, typename L2>
  void search(L1 color, L1 pre, L1 post, L2 parent)
  {
    for (vertex v : graph.vertices()) {
      if (color(v) == 0)
        explore(v, color, pre, post, parent);
    }
  }

  template<typename L1, typename L2>
  void explore(vertex u, L1 color, L1 pre, L1 post, L2 parent)
  {
    color(u) = 1;     // color u gray (on stack)
    pre(u) = clock++;

    for (edge e : graph.out_edges(u)) {
      vertex v = graph.target(e);
      if (color(v) == 0) {
        // (u, v) is a tree edge
        parent(v) = u;
        explore(v, color, pre, post, parent);
      }
      else if (color(v) == 1) {
        // (u, v) is a back edge
      }
      else {
        // (u, v) is a cross or forward edge
      }
    }

    post(u) = clock++;
    color(u) = 2; // color u black (done).
  }

  G& graph;
  std::vector<int> colors;
  std::vector<int> pre_times;
  std::vector<int> post_times;
  std::vector<vertex> parents;
  int clock;
};


// A basic DFS implementation for undirected graphs.
template<typename G>
struct undirected_dfs
{
  undirected_dfs(G& g)
    : graph(g), 
      colors(graph.num_vertices(), 0),
      pre_times(graph.num_vertices(), -1),
      post_times(graph.num_vertices(), -1),
      parents(graph.num_vertices()),
      clock(0)
  { }


  void operator()()
  {
    auto color = vertex_label(colors);
    auto pre = vertex_label(pre_times);
    auto post = vertex_label(post_times);
    auto parent = vertex_label(parents);

    // Extra initialization.
    for (vertex v : graph.vertics())
      parent(v) = v;
    
    search(color, pre, post, parent);
  }


  template<typename L1, typename L2>
  void search(L1 color, L1 pre, L1 post, L2 parent)
  {
    for (vertex v : graph.vertices()) {
      if (color(v) == 0)
        explore(v, color, pre, post, parent);
    }
  }

  template<typename L1, typename L2>
  void explore(vertex u, L1 color, L1 pre, L1 post, L2 parent)
  {
    color(u) = 1;     // color u gray (on stack)
    pre(u) = clock++;

    for (edge e : graph.edges(u)) {
      vertex v = graph.opposite(e, u);
      if (color(v) == 0) {
        // (u, v) is a tree edge
        parent(v) = u;
        explore(v, color, pre, post, parent);
      }
      else if (color(v) == 1) {
        // (u, v) is a back edge
      }
      else {
        // (u, v) is a cross or forward edge
      }
    }

    post(u) = clock++;
    color(u) = 2; // color u black (done)
  }

  G& graph;
  std::vector<int> colors;
  std::vector<int> pre_times;
  std::vector<int> post_times;
  std::vector<vertex> parents;
  int clock;
};



} // namespace origin

#endif
