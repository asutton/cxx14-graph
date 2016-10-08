// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_UNDIRECTED_LIST_HPP
#define GRAPH_UNDIRECTED_LIST_HPP

#include "utility.hpp"
#include "common.hpp"

#include <vector>


namespace origin {

// A labeled vertex with source and target edges. This is specialized for the
// empty case, which associates no label.
template<typename T = empty>
struct undirected_vertex
{
  undirected_vertex() = default;

  undirected_vertex(T const& t)
    : edges_(), data(t)
  { }

  edge_list const& edges() const { return edges_; }

  std::size_t degree() const { return edges_.size(); }

  edge_list edges_;
  T data;
};


// Edges

// An labeled edge with source and target vertexes. This is specialized for the
// empty case, which associates no label.
template<typename T = empty>
struct undirected_edge
{
  // TODO: Value-initialize the data element or not? We currently do not.
  undirected_edge(vertex_t u, vertex_t v)
    : ends_{u, v}
  { }
  
  undirected_edge(vertex_t u, vertex_t v, T const& t)
    : ends_{u, v}, data(t)
  { }
  
  vertex_t first() const { return ends_[0]; }
  vertex_t second() const { return ends_[1]; }

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
struct graph
{
  using vertex_type = undirected_vertex<V>;
  using vertex_set = std::vector<vertex_type>;
  using vertex_iterator = counted_iterator<vertex_t>;
  using vertex_range = counted_range<vertex_t>;

  using edge_type = undirected_edge<E>;
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

  // Incidence
  edge_list edges(vertex_t v) const;
  std::size_t degree(vertex_t v) const;

  edge_iterator find_edge(vertex_t, vertex_t) const;
  bool has_edge(vertex_t, vertex_t) const;
  edge_t edge(vertex_t, vertex_t) const;

  vertex_t first(edge_t e) const;
  vertex_t second(edge_t e) const;
  vertex_t opposite(edge_t e, vertex_t v) const;
  
  // Incremental construction
  vertex_t add_vertex();
  vertex_t add_vertex(V const&);

  edge_t add_edge(vertex_t, vertex_t);
  edge_t add_edge(vertex_t, vertex_t, E const&);

  vertex_set verts_;
  edge_set edges_;
};

// Vertex list

// Returns true if this is the null graph (having no vertices).
template<typename V, typename E>
bool 
graph<V, E>::is_null() const 
{ 
  return num_vertices() == 0; 
}

// Returns the number of vertices in the graph.
template<typename V, typename E>
std::size_t 
graph<V, E>::num_vertices() const 
{ 
  return verts_.size(); 
}

// Returns the list of vertices in the graph.
template<typename V, typename E>
auto
graph<V, E>::vertices() const -> vertex_range
{ 
  return vertex_range(num_vertices()); 
}

// Returns an iterator for the first vertex in the graph.
template<typename V, typename E>
auto 
graph<V, E>::begin_vertices() const -> vertex_iterator 
{ 
  return vertex_iterator(0); 
}

// Returns an iterator past the last vertex in the graph.
template<typename V, typename E>
auto
graph<V, E>::end_vertices() const -> vertex_iterator
{ 
  return vertex_iterator(num_vertices()); 
}

// Edge list

// Returns true if the graph has empty (no edges).
template<typename V, typename E>
bool 
graph<V, E>::is_empty() const 
{ 
  return num_edges() == 0; 
}

// Returns the number of edges in the graph.
template<typename V, typename E>
std::size_t 
graph<V, E>::num_edges() const { return edges_.size(); }

// Returns the list of edges in the graph.
template<typename V, typename E>
auto 
graph<V, E>::edges() const -> edge_range
{ 
  return edge_range(num_edges()); 
}

template<typename V, typename E>
auto 
graph<V, E>::begin_edges() const -> edge_iterator 
{ 
  return edge_iterator(0); 
}

template<typename V, typename E>
auto 
graph<V, E>::end_edges() const -> edge_iterator 
{ 
  return edge_iterator(num_edges()); 
}

// Incidence

// Returns the list of edges incident to v.
template<typename V, typename E>
edge_list 
graph<V, E>::edges(vertex_t v) const 
{ 
  return verts_[v].edges(); 
}

// Returns the degree of v.
template<typename V, typename E>
std::size_t 
graph<V, E>::degree(vertex_t v) const 
{ 
  return verts_[v].degree(); 
}

// Returns an iterator referring to the edge {u, v} if such an edge
// exists. Returns end_edges() otherwise.
template<typename V, typename E>
auto
graph<V, E>::find_edge(vertex_t u, vertex_t v) const -> edge_iterator
{
  if (degree(u) < degree(v))
    std::swap(u, v);
  edge_list const& edges = verts_[u].edges_;
  for (edge_t e : edges) {
    if (opposite(e, u) == v)
      return begin_edges() + e;
  }
  return end_edges();
}

// Returns true if the edge {u, v} is in the graph.
template<typename V, typename E>
bool
graph<V, E>::has_edge(vertex_t u, vertex_t v) const
{
  return find_edge(u, v) != end_edges();
}

// Returns the edge {u, v}, assuming that it is in the graph.
template<typename V, typename E>
edge_t
graph<V, E>::edge(vertex_t u, vertex_t v) const
{
  assert(has_edge(u, v));
  return *find_edge(u, v);
}

// In the edge {u, v}, returns u.
//
// TODO: I don't like this function. An undirected edge doesn't really
// have positional ends. It would be better if this function didn't
// actually exist (or maybe the vertices were canonically ordered?).
template<typename V, typename E>
vertex_t 
graph<V, E>::first(edge_t e) const 
{ 
  return edges_[e].first(); 
}

// In the edge {u, v}, returns v.
//
// TODO: See comments above.
template<typename V, typename E>
vertex_t 
graph<V, E>::second(edge_t e) const 
{ 
  return edges_[e].second(); 
}

// Assuming v is an end of e, returns the opposite end.
template<typename V, typename E>
vertex_t
graph<V, E>::opposite(edge_t e, vertex_t v) const
{
  assert(v == first(e) || v == second(e));
  if (v == first(e))
    return second(e);
  else // v == second(e)
    return first(e);
}


// Incremental construction

template<typename V, typename E>
vertex_t
graph<V, E>::add_vertex()
{
  verts_.emplace_back();
  return verts_.size() - 1;
}

template<typename V, typename E>
vertex_t
graph<V, E>::add_vertex(V const& v)
{
  verts_.emplace_back(v);
  return verts_.size() - 1;
}

template<typename V, typename E>
edge_t
graph<V, E>::add_edge(vertex_t u, vertex_t v)
{
  assert(!has_edge(u, v));
  edges_.emplace_back(u, v);
  edge_t e = edges_.size() - 1;
  verts_[u].edges_.push_back(e);
  verts_[v].edges_.push_back(e);
  return e;
}

template<typename V, typename E>
edge_t
graph<V, E>::add_edge(vertex_t u, vertex_t v, E const& x)
{
  assert(!has_edge(u, v));
  edges_.emplace_back(u, v, x);
  edge_t e = edges_.size() - 1;
  verts_[u].edges_.push_back(e);
  verts_[v].edges_.push_back(e);
  return e;
}

} // namespace origin

#endif
