// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_UTILITY_HPP
#define GRAPH_UTILITY_HPP

#include <iterator>


namespace origin {

// An iterator whose values are increasing integer values.
//
// TODO: This should be a random access iterator.
template<typename T>
struct counted_iterator
{
  using value_type = T;
  using reference = T;
  using pointer = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  counted_iterator(T n)
    : count(n)
  { }

  T operator*() const { return count; }

  counted_iterator& operator++() { ++count; return *this; }
  counted_iterator& operator++(int) { auto x = *this; ++count; return x; }

  counted_iterator& operator--() { --count; return *this; }
  counted_iterator& operator--(int) { auto x = *this; --count; return x; }

  bool operator==(counted_iterator i) { return count == i.count; }
  bool operator!=(counted_iterator i) { return count != i.count; }

  T count;
};


// A range of counted iterators.
template<typename T>
struct counted_range
{
  counted_range(T b)
    : first(0), limit(b)
  { }

  counted_range(T a, T b)
    : first(a), limit(b)
  { }

  counted_iterator<T> begin() const { return first; }
  counted_iterator<T> end() const { return limit; }

  std::size_t size() const { return limit.count - first.count; }

  counted_iterator<T> first;
  counted_iterator<T> limit;
};


} // namespace origin
