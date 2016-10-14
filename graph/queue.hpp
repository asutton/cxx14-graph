// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_QUEUE_HPP
#define GRAPH_QUEUE_HPP

#include "common.hpp"

#include <algorithm>
#include <queue>
#include <iostream>


namespace origin
{

// A function object used to compare the label values of vertices. This
// is a relation on the value of the label.
template<typename L, typename C>
struct compare_vertex_label
{
  compare_vertex_label(L label)
    : label(label), comp()
  { }

  compare_vertex_label(L label, C comp)
    : label(label), comp(comp)
  { }

  bool operator()(vertex_t u, vertex_t v) const 
  {
    return comp(label(u), label(v));
  }

  L label; // Value associated with vertices.
  C comp;  // Comparison function for vertices.
};


// A priority queue whose values are sorted on insertion. The order of
// elements in the key is determined by a a comparison parameter.
//
// Note that insertion is linear in the size of the queue.
template<typename T, typename C = std::less<T>>
struct insertion_queue : std::queue<T>
{
  using container_type = typename std::queue<T>::container_type;

  insertion_queue();
  insertion_queue(C comp);

  container_type const& data() const { return this->c; }

  void push(T const&);
  void push(T&&);

  C comp;
};

template<typename T, typename C>
insertion_queue<T, C>::insertion_queue()
  : std::queue<T>(), comp()
{ }

template<typename T, typename C>
insertion_queue<T, C>::insertion_queue(C comp)
  : std::queue<T>(), comp(comp)
{ }

template<typename T, typename C>
void
insertion_queue<T, C>::push(T const& key)
{
  auto iter = this->c.begin();
  auto limit = this->c.end();
  while (iter != limit && comp(key, *iter))
    ++iter;
  this->c.insert(iter, key);
}

template<typename T, typename C>
void
insertion_queue<T, C>::push(T&& key)
{
  auto iter = this->c.begin();
  auto limit = this->c.end();
  while (iter != limit && comp(key, *iter))
    ++iter;
  this->c.insert(iter, std::move(key));
}


#if 0
// TODO: Currently we assume that T is an integer type whose values
// are dense (0..n). In full generality, that won't hold water. We
// should really parameterize the queue by the index to better lookup.
template<typename T, typename L, typename C = std::less<>>
struct mutable_binary_heap
{
  using std::vector<std::size_t> index_map;
  using std::vector<T> key_list;

  bool is_empty() const;
  int size() const;

  T const& top();
  void push(T const&);
  void pop();

  template<typename U>
  void update(T const&, U const&);

  index_map index;
  key_list order;
  L pri;
  C comp;
};


// Returns true if the binary heap is emmpty.
template<typename T, typename L, typename C>
bool 
mutable_binary_heap<T, L, C>::is_empty() const 
{ 
  return order.empty(); 
}

// Returns the number of elements in the heap.
template<typename T, typename L, typename C>
int 
mutable_binary_heap<T, L, C>::size() const 
{ 
  return order.size(); 
}

// Returns the element at the top of the heap.
template<typename T, typename L, typename C>
T const&
mutable_binary_heap<T, L, C>::top() const
{

}

// Insert a new element into the heap.
template<typename T, typename L, typename C>
void
mutable_binary_heap<T, L, C>::push(T const& k)
{

}

// Remove the top element from the heap.
template<typename T, typename L, typename C>
void
mutable_binary_heap<T, L, C>::pop()
{

}

// Update the position of k within the heap as a result of changing its
// priority to the given value. 
//
// NOTE: This function is only defined if the key is mutable.
template<typename T, typename L, typename C>
template<typename U>
void
mutable_binary_heap<T, L, C>::update(T const& key, U const& value)
{

}
#endif


} // namespace origin


#endif
