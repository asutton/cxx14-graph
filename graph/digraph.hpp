// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_DIRECTED_LIST_HPP
#define GRAPH_DIRECTED_LIST_HPP

#include "utility.hpp"
#include "common.hpp"

#include <vector>


namespace origin {

// A labeled vertex with source and target edges. This is specialized for the
// empty case, which associates no label.
template<typename T = empty>
struct directed_vertex
{
  directed_vertex() = default;

  directed_vertex(T const& t)
    : out_(), in_(), data(t)
  { }

  bool is_source() const { return in_degree() == 0; }
  bool is_sing() const { return out_degree() == 0; }

  edge_list const& out_edges() const { return out_; }
  edge_list const& in_edges() const { return in_; }

  std::size_t out_degree() const { return out_.size(); }
  std::size_t in_degree() const { return in_.size(); }
  std::size_t degree() const { return out_degree() + in_degree(); }

  edge_list out_;
  edge_list in_;
  T data;
};


// Edges

// An labeled edge with source and target vertexes. This is specialized for the
// empty case, which associates no label.
template<typename T = empty>
struct directed_edge
{
  // TODO: Value-initialize the data element or not? We currently do not.
  directed_edge(vertex_t u, vertex_t v)
    : ends_{u, v}
  { }
  
  directed_edge(vertex_t u, vertex_t v, T const& t)
    : ends_{u, v}, data(t)
  { }
  
  vertex_t source() const { return ends_[0]; }
  vertex_t target() const { return ends_[1]; }

  vertex_t ends_[2];
  T data;
};


// Graph

// A directed graph that is parameterized by the edge labels for vertexes (V)
// and edges (U). Both arguments default to empty, indicating that no labels
// are associated with U or V.
//
// TODO: Support vertex and edge removal by keeping a free-list of unused
// indexes and clever used of in-place allocation/deletion. Note that 
// supporting removal makes vertex traversal more complex (forward only).
//
// TODO: Implement const iterators and ranges.
template<typename V = empty, typename E = empty>
struct digraph
{
  using vertex_type = directed_vertex<V>;
  using vertex_set = std::vector<vertex_type>;
  using vertex_iterator = counted_iterator<vertex_t>;
  using vertex_range = counted_range<vertex_t>;

  using edge_type = directed_edge<E>;
  using edge_set = std::vector<edge_type>;
  using edge_iterator = counted_iterator<edge_t>;
  using edge_range = counted_range<edge_t>;

  // Vertex list
  bool is_null() const;
  std::size_t num_vertices() const;

  vertex_range vertices() const;
  vertex_iterator begin_vertices() const;
  vertex_iterator end_vertices() const;

  // Edge list
  bool is_empty() const;
  std::size_t num_edges() const;

  edge_range edges() const;
  edge_iterator begin_edges() const;
  edge_iterator end_edges() const;

  // Incidence list
  edge_list out_edges(vertex_t) const;
  edge_list in_edges(vertex_t) const;

  std::size_t out_degree(vertex_t) const;
  std::size_t in_degree(vertex_t) const;
  std::size_t degree(vertex_t) const;

  edge_iterator find_edge(vertex_t, vertex_t) const;
  bool has_edge(vertex_t, vertex_t) const;
  edge_t edge(vertex_t, vertex_t) const;

  vertex_t source(edge_t) const;
  vertex_t target(edge_t) const;

  // Incremental construction
  vertex_t add_vertex();
  vertex_t add_vertex(V const&);

  edge_t add_edge(vertex_t, vertex_t);
  edge_t add_edge(vertex_t, vertex_t, E const&);

  vertex_set verts_;
  edge_set edges_;
};

// Vertex list

// Returns true if the graph has no vertices.
template<typename V, typename E>
bool 
digraph<V, E>::is_null() const 
{ 
  return num_vertices() == 0; 
}

// Returns the number of vertices in the graph.
template<typename V, typename E>
std::size_t 
digraph<V, E>::num_vertices() const 
{ 
  return verts_.size(); 
}

// Returns the list of vertices in the graph.
template<typename V, typename E>
auto 
digraph<V, E>::vertices() const -> vertex_range 
{ 
  return vertex_range(num_vertices()); 
}

// Returns an iterator to the first vertex in the graph.
template<typename V, typename E>
auto 
digraph<V, E>::begin_vertices() const -> vertex_iterator 
{ 
  return vertex_iterator(0); 
}

// Returns an iterator past the last vertex in the graph.
template<typename V, typename E>
auto 
digraph<V, E>::end_vertices() const -> vertex_iterator 
{ 
  return vertex_iterator(num_vertices()); 
}

// Edge list

// Returns true if the graph has no edges.
template<typename V, typename E>
bool 
digraph<V, E>::is_empty() const 
{ 
  return num_edges() == 0; 
}

// Returns the number of edges in the graph.
template<typename V, typename E>
std::size_t 
digraph<V, E>::num_edges() const 
{ 
  return edges_.size(); 
}

// Returns the list of edges in the graph.
template<typename V, typename E>
auto 
digraph<V, E>::edges() const -> edge_range 
{ 
  return edge_range(num_edges()); 
}

// Returns an iterator to the first edge in the graph.
template<typename V, typename E>
auto 
digraph<V, E>::begin_edges() const -> edge_iterator 
{ 
  return edge_iterator(0); 
}

// Returns an iterator past the last edge in the graph.
template<typename V, typename E>
auto 
digraph<V, E>::end_edges() const -> edge_iterator 
{ 
  return edge_iterator(num_edges()); 
}

// Incidence

// Returns the list of outgoing edges for v.
template<typename V, typename E>
edge_list 
digraph<V, E>::out_edges(vertex_t v) const 
{ 
  return verts_[v].out_edges(); 
}

// Returns the list incoming edges to v.
template<typename V, typename E>
edge_list 
digraph<V, E>::in_edges(vertex_t v) const 
{ 
  return verts_[v].in_edges(); 
}

// Returns the out degree of v.
template<typename V, typename E>
std::size_t 
digraph<V, E>::out_degree(vertex_t v) const 
{ 
  return verts_[v].out_degree(); 
}

// Returns the in degree of v.
template<typename V, typename E>
std::size_t 
digraph<V, E>::in_degree(vertex_t v) const 
{ 
  return verts_[v].in_degree(); 
}

// Returns the (total) degree of v.
template<typename V, typename E>
std::size_t 
digraph<V, E>::degree(vertex_t v) const 
{ 
  return verts_[v].degree(); 
}

// Returns an iterator to the edge (u, v) if it exists. Otherwise, returns
// end_edges().
template<typename V, typename E>
auto
digraph<V, E>::find_edge(vertex_t u, vertex_t v) const -> edge_iterator
{
  // Search the shortest list for the corresponding edge.
  if (out_degree(u) < in_degree(v)) {
    edge_list const& out = verts_[u].out_;
    for (edge_t e : out) {
      if (target(e) == v)
        return begin_edges() + e;
    }
  }
  else {
    edge_list const& in = verts_[v].in_;
    for (edge_t e : in) {
      if (source(e) == u)
        return begin_edges() + e;
    }
  }
  return end_edges();
}

// Returns true if the edge (u, v) exists.
template<typename V, typename E>
bool
digraph<V, E>::has_edge(vertex_t u, vertex_t v) const
{
  return find_edge(u, v) != end_edges();
}

// Assuming (u, v) exists, returns that edge.
template<typename V, typename E>
edge_t
digraph<V, E>::edge(vertex_t u, vertex_t v) const
{
  assert(has_edge(u, v));
  return *find_edge(u, v);
}

// In the edge (u, v), returns u.
template<typename V, typename E>
vertex_t 
digraph<V, E>::source(edge_t e) const 
{ 
  return edges_[e].source(); 
}

// In the edge (u, v), returns v.
template<typename V, typename E>
vertex_t 
digraph<V, E>::target(edge_t e) const 
{ 
  return edges_[e].target(); 
}

// Incremental construction

template<typename V, typename E>
vertex_t
digraph<V, E>::add_vertex()
{
  verts_.emplace_back();
  return verts_.size() - 1;
}

template<typename V, typename E>
vertex_t
digraph<V, E>::add_vertex(V const& v)
{
  verts_.emplace_back(v);
  return verts_.size() - 1;
}

template<typename V, typename E>
edge_t
digraph<V, E>::add_edge(vertex_t u, vertex_t v)
{
  assert(!has_edge(u, v));
  edges_.emplace_back(u, v);
  edge_t e = edges_.size() - 1;
  verts_[u].out_.push_back(e);
  verts_[v].in_.push_back(e);
  return e;
}

template<typename V, typename E>
edge_t
digraph<V, E>::add_edge(vertex_t u, vertex_t v, E const& x)
{
  assert(!has_edge(u, v));
  edges_.emplace_back(u, v, x);
  edge_t e = edges_.size() - 1;
  verts_[u].out_.push_back(e);
  verts_[v].in_.push_back(e);
  return e;
}

} // namespace origin

#endif
