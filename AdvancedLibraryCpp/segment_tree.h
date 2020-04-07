#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <exception>
#include "heap.h"

namespace advanced {

  class heap_not_built_exception_t : public std::runtime_error {
  public:

    heap_not_built_exception_t() :
      std::runtime_error{
        "Attempt to call a method that requires the heap is built"} { }
  };

  /**
   * Generic segment tree implementation using std::vector<T = long long>
   * The template argument "class compare_structure = std::less<T>" is
   * the compare structure that implements the call operator "()" for comparing
   * two nodes in the sequence
   */
  template <class T = long long,
            class compare_structure_t = std::less<T>>
  class segment_tree_t {
    std::vector<T>        array;
    heap_t<size_t>        heap;
    bool                  built_heap{ false };

    static const size_t not_found { static_cast<size_t>(-1) };

  public:

    struct range_t {
      size_t index;
      size_t start;
      size_t end;
    };

    /**
     * Default constructor. Nothing to do. The heap isn' built
     */
    segment_tree_t() = default;

    /**
     * It'll forward to std::vector<T> constructor any given argument
     * and build the entire heap
     */
    template<typename ...Args>
    segment_tree_t(Args... args) : array(std::forward<Args>(args)...) {
      build();
    }

    /**
     * It'll rebuild the internal heap, really costful operation
     */
    void
    rebuild(const std::vector<T>& input) {
      array = input;
      build();
    }

    /**
     * @param position position to update
     * @param value    value to replace
     * It updates a value in a position of the array
     * @return true if the position is lower than the current size
     */
    bool
    update(size_t position, const T&value) {
      bool ok { position < array.size() };
      if (ok) {
        array[position] = value;
        if (is_built()){
          update_heap(position, head());
        }
        else {
          build();
        }
      }
      return ok;
    }

    /**
     * @param position position to update
     * @param value    value to replace (std::move)
     * It updates a value in a position of the array
     * @return true if the position is lower than the current size
     */
    bool
    update(size_t position, T&&value) {
      bool ok { position < array.size() };
      if (ok) {
        array[position] = std::move(value);
        if (is_built()){
          update_heap(position, head());
        }
        else {
          build();
        }
      }
      return ok;
    }

    /**
     * @param value    value to inserted
     * It inserts a value in the last position of the array
     * @return the index of the inserted element
     */
    size_t
    push_back(const T&value) {
      const auto minimum_size { minimum_required_size_for_head(size() + 1) };
      const auto position     { size() };
      array.push_back(value);
      if (heap.size() < minimum_size) {
        heap.resize(2 * minimum_size);
      }
      if (is_built()){
        update_heap(position, head());
      }
      else {
        build();
      }
      return position;
    }

    /**
     * @param value    value to inserted using std::move operation
     * It inserts a value in the last position of the array
     * @return the index of the inserted element
     */
    size_t
    emplace_back(T&&value) {
      const auto minimum_size { minimum_required_size_for_head(size() + 1) };
      const auto position     { size() };
      array.emplace_back(std::move(value));
      if (heap.size() < minimum_size) {
        heap.resize(2 * minimum_size);
      }
      if (is_built()){
        update_heap(position, head());
      }
      else {
        build();
      }
      return position;
    }

    /**
     * @return the const reference to an element at a position
     */
    const T&
    operator[](size_t position) const {
      return array[position];
    }

    /**
     * @return the const reference to an element at a position
     * @throws std::out_of_range exception if the position isn't lower than the
     * size of the internal array;
     */
    const T&
    at(size_t position) const {
      return array.at(position);
    }

    /**
     * @return the size of the internal array;
     */
    size_t
    size() const {
      return array.size();
    }

    /**
     * It resizes the array if, and only if, the new size is greater than
     * the current
     */
    void
    resize(size_t size) {
      if (size > array.size()) {
        array.resize(size);
      }
    }

    /**
     * It reserves an ammount of memory in the internal array
     */
    void
    reserve(size_t size) {
      array.reserve(size);
    }

    /**
     * @return whether the heap is built or not
     */
    bool is_built() const {
      return built_heap;
    }

    /**
     * It performs a range query for the array.
     * query(1, 3) is equal to query(3, 1)
     *
     * @throws std::out_of_range if the given range is out of the array
     *         advanced::heap_not_built_exception_t if heap is not built
     */
    const T&
    query(size_t start, size_t end) const {
      if (!is_built()) throw heap_not_built_exception_t{};
      const auto index {
        range_query(std::min(start, end), std::max(start, end), head())
      };
      return array.at(index);
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: index of the top of the segment for the entire array
     *    start: 0
     *    end:   size of the array minus 1 (size()-1)
     *  }
     */
    range_t
    head() const {
      return { heap.head(), 0, size() - 1 };
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: right child of the current node
     *    start: [(start+end)/2 + 1, end]
     *    end:   end
     *  }
     */
    range_t
    upper_bound(const range_t& node) const {
      return { heap.right_child_of(node.index),
               (node.start + node.end) / 2 + 1,
               node.end };
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: right child of the current node
     *    start: start
     *    end:   [(start+end)/2]
     *  }
     */
    range_t
    lower_bound(const range_t& node) const {
      return { heap.left_child_of(node.index),
               node.start,
               (node.start + node.end) / 2 };
    }

    /**
     * @return the minimum memory space required for the heap for a given array
     * size
     */
    inline static size_t
    minimum_required_size_for_head(size_t size) {
      return static_cast<size_t>(
                std::ceil(2.0 * std::pow(2.0, std::log2(size) + 1.0)));
    }

  private:

    /**
     * It performs a range query for the array recursively
     * @return the index of the element which is the top of the range segment
     */
    size_t
    range_query(const range_t& interval, const range_t& node_range) const {
      static const compare_structure_t compare;
      size_t index{ not_found };

      if (node_range.start <= interval.end && node_range.end >= interval.start) {
        if (interval.start >= node_range.start && interval.end >= node_range.end) {
          // completely inside range
          index = node_range.index;
        }
        else {
          size_t left_index  = range_query(interval, lower_bound(node_range));
          size_t right_index = range_query(interval, upper_bound(node_range));

          if (left_index == not_found) {
            index = right_index;
          }
          else if (right_index == not_found) {
            index = left_index;
          }
          else {
            index = compare(array[left_index], array[right_index]) ?
                    left_index :
                    right_index;
          }
        }
      }
      return index;
    }

    /**
     * It performs a range query updating the internal heap for the array
     * recursively.
     * @return the index of the element which is the top of the range segment
     */
    size_t
    update_heap(size_t position, const range_t& range) {
      static const compare_structure_t compare;
      size_t index { not_found }, right, left;
      if (range.end == range.start) {
        index = heap[range.index];
      }
      else if (range.start <= position && range.end >= position){
        left  = update_heap(position, lower_bound(range));
        right = update_heap(position, upper_bound(range));
        if (left == not_found) {
          index = right;
        }
        else if (right == not_found) {
          index = left;
        }
        else {
          index = compare(array[left], array[right]) ? left : right;
        }
        heap[range.index] = index;
      }
      return index;
    }

    /**
     * It'll fill the heap with not_found indexes and call build(const range_t&)
     */
    void
    build() {
      heap.assign(heap.size(), not_found);
      (void)build(head());
      built_heap = true;
    }

    /**
     * It performs a range query updating the internal heap for the entire array
     * recursively.
     * @return the index of the element which is the top of the range segment
     */
    size_t
    build(const range_t& range) {
      static const compare_structure_t compare;
      size_t index { not_found }, right, left;
      if (range.end == range.start) {
        index = heap[range.index];
      }
      else {
        left  = build(lower_bound(range));
        right = build(upper_bound(range));
        index = compare(array[left], array[right]) ? left : right;
        heap[range.index] = index;
      }
      return index;
    }
  };
}
