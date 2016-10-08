// Copyright (c) 2016 Andrew Sutton
// All rights reserved

#ifndef GRAPH_UTILITY_HPP
#define GRAPH_UTILITY_HPP

#include <cassert>
#include <iterator>


namespace origin {

// An iterator whose values are increasing integer values.
template<typename T>
struct counted_iterator
{
  using value_type = T;
  using reference = T;
  using pointer = void;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  counted_iterator(T n)
    : num_(n)
  { }

  T operator*() const { return num_; }
  T operator[](std::ptrdiff_t n) const { return num_ + n; }

  void operator->() const = delete;
  
  counted_iterator& operator++() { ++num_; return *this; }
  counted_iterator& operator++(int) { auto x = *this; ++num_; return x; }

  counted_iterator& operator--() { --num_; return *this; }
  counted_iterator& operator--(int) { auto x = *this; --num_; return x; }

  counted_iterator& operator+=(std::ptrdiff_t n) { num_ += n; return *this; }
  counted_iterator& operator-=(std::ptrdiff_t n) { num_ += n; return *this; }

  bool operator==(counted_iterator i) { return num_ == i.num_; }
  bool operator!=(counted_iterator i) { return num_ != i.num_; }

  T num_;
};

template<typename T>
inline counted_iterator<T>
operator+(counted_iterator<T> i, std::ptrdiff_t n)
{
  return i += n;
}

template<typename T>
inline counted_iterator<T>
operator+(std::ptrdiff_t n, counted_iterator<T> i)
{
  return i += n;
}

template<typename T>
inline counted_iterator<T>
operator-(counted_iterator<T> i, std::ptrdiff_t n)
{
  return i -= n;
}

template<typename T>
inline std::ptrdiff_t
operator-(counted_iterator<T> i, counted_iterator<T> j)
{
  return i.num_ - j.num_;
}


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

#endif
