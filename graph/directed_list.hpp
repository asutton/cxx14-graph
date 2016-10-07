// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_DIRECTED_LIST_HPP
#define GRAPH_DIRECTED_LIST_HPP

#include "utility.hpp"
#include "common.hpp"

#include <vector>


namespace origin {


// The base class of all vertices in a directed adjancency list. This 
// contains only the linking structure of the vertex: the in and out
// edges.
struct directed_vertex_base
{
  directed_vertex_base()
    : out_(), in_()
  { }

  edge_list const& out_edges() const { return out_; }
  edge_list const& in_edges() const { return in_; }

  std::size_t out_degree() const { return out_.size(); }
  std::size_t in_degree() const { return in_.size(); }
  std::size_t degree() const { return out_degree() + in_degree(); }
  
  edge_list out_;
  edge_list in_;
};

// A labeled vertex with source and target edges. This is specialized for the
// empty case, which associates no label.
template<typename T = empty>
struct directed_vertex : directed_vertex_base
{
  directed_vertex(T const& t)
    : directed_vertex_base(), data(t)
  { }
  
  T data;
};

template<>
struct directed_vertex<empty> : directed_vertex
{
  using directed_vertex::directed_vertex;
};


// Edges

// The base class of all edges in a directed graph. This contains only
// the source and target of the graph.
struct directed_edge_base
{
  directed_edge_base(vertex u, vertex v)
    : verts(u, v)
  { }

  vertex source() const { return verts.first; }
  vertex target() const { return verts.second; }

  edge_pair verts;
};


// An labeled edge with source and target vertexes. This is specialized for the
// empty case, which associates no label.
template<typename T = empty>
struct directed_edge : directed_edge_base
{
  // TODO: Value-initialize the data element or not? We currently do not.
  directed_edge(vertex u, vertex v)
    : directed_edge_base(u, v)
  { }
  
  directed_edge(vertex u, vertex v, T const& t)
    : directed_edge_base(u, v), data(t)
  { }
  
  T data;
};

template<>
struct directed_edge<empty> : directed_edge_base
{
  using directed_edge_base::directed_edge_base;
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
  using vertex_iterator = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_range = counted_range<vertex>;

  using edge_type = directed_edge<E>;
  using edge_set = std::vector<edge_type>;
  using edge_iterator = typename edge_set::iterator;
  using const_edge_iterator = typename edge_set::const_iterator;
  using edge_range = counted_range<edge>;

  // Vertexes
  std::size_t num_vertices() const { return verts_.size(); }

  edge_list out_edges(vertex v) const { return verts_[v].out_edges(); }
  edge_list in_edges(vertex v) const { return verts_[v].in_edges(); }

  std::size_t out_degree() const { return verts_[v].out_degree(); }
  std::size_t in_degree() const { return verts_[v].in_degree(); }
  std::size_t degree() const { return verts_[v].degree(); }
  
  vertex add_vertex();
  vertex add_vertex(V const&);

  // Edges
  std::size_t num_edges() const { return edges_.size(); }

  vertex source(edge e) const { return get_edge(e).source(); }
  vertex target(edge e) const { return get_edge(e).target(); }

  edge add_edge(vertex, vertex);
  edge add_edge(vertex, vertex, E const&);

  // Iterators
  vertex_range vertices() { return vertex_range(num_vertices()); }
  vertex_iterator begin_vertices() { return verts_.begin(); }
  vertex_iterator end_vertices() { return verts_.end(); }


  // Iterators
  edge_range edges() { return edge_range(num_edges()); }
  edge_iterator begin_edges() { return edges_.begin(); }
  edge_iterator end_edges() { return edges_.end(); }

  vertex_set verts_;
  edge_set edges_;
};


// TODO: Requires Default_constructible<V>()
template<typename V, typename E>
vertex
digraph<V, E>::add_vertex()
{
  verts_.emplace_back();
  return verts_.size() - 1;
}

template<typename V, typename E>
vertex
digraph<V, E>::add_vertex(V const& v)
{
  verts_.emplace_back(v);
  return verts_.size() - 1;
}

// TODO: This is a simple graph. Verify that (u, v) does not already exist.
template<typename V, typename E>
edge
digraph<V, E>::add_edge(vertex u, vertex v)
{
  edges_.emplace_back(u, v);
  edge e = edges_.size() - 1;
  verts_[u].out_.push_back(e);
  verts_[v].in_.push_back(e);
  return e;
}

// TODO: This is a simple graph. Verify that (u, v) does not already exist.
template<typename V, typename E>
edge
digraph<V, E>::add_edge(vertex u, vertex v, E const& x)
{
  edges_.emplace_back(u, v, x);
  edge e = edges_.size() - 1;
  verts_[u].out.push_back(e);
  verts_[v].in_.push_back(e);
  return e;
}

} // namespace origin
