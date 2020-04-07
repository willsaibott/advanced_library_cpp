#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <exception>
#include <type_traits>
#include "heap.h"
#include "math_operation.h"

namespace advanced {

  class heap_not_built_exception_t : public std::runtime_error {
  public:

    heap_not_built_exception_t() :
      std::runtime_error{
        "Attempt to call a method that requires the heap is built"} { }
  };

  template <class T, class range_value_t>
  struct segment_node_t {
    /**
     * transform operator for non-leaf nodes
     */
    virtual range_value_t
    operator()(const std::vector<T>& array,
               const range_value_t&  left,
               const range_value_t&  right) const = 0;

    /**
     * transform operator for leaf nodes
     */
    virtual range_value_t
    operator()(const T& element, const size_t& position) const = 0;
  };

  template <class T, class compare_t = std::less<T>>
  struct index_compare_t : segment_node_t<T, size_t> {
    static const size_t not_found { static_cast<size_t>(-1) };

    inline virtual size_t
    operator ()(const std::vector<T>& array,
                const size_t&         left,
                const size_t&         right) const override
    {
      static const compare_t compare;
      return compare(array[left], array[right]) ? left : right;
    }

    inline virtual size_t
    operator()(const T& element, const size_t& position) const override {
      return position;
    }
  };

  template <class T, class operation_t>
  struct range_operation_t : segment_node_t<T, T> {

    inline virtual T
    operator ()(const std::vector<T>& array,
                const size_t&         left,
                const size_t&         right) const override
    {
      static const operation_t operation;
      return operation(array[left], array[right]);
    }

    inline virtual T
    operator()(const T& element, const size_t& position) const override {
      return element;
    }
  };

  template<class T>
  using range_sum_t = range_operation_t<T, sum_t<T>>;

  template<class T>
  using range_multiplication_t = range_operation_t<T, multiplication_t<T>>;

  template<class T>
  using range_min_t = range_operation_t<T, minimum_t<T>>;

  template<class T>
  using range_max_t = range_operation_t<T, maximum_t<T>>;


  /**
   * Generic segment tree implementation using std::vector<T = long long>
   * The template argument "class compare_structure = std::less<T>" is
   * the compare structure that implements the call operator "()" for comparing
   * two nodes in the sequence
   */
  template <class T,
            class range_value_t   = size_t,
            class transform_t     = index_compare_t<T>>
  class segment_tree_t {
  public:

    static const size_t not_found { static_cast<size_t>(-1) };

    struct range_t {
      size_t start { not_found };
      size_t end   { not_found };
      range_value_t value;
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
    segment_tree_t(Args... args) : _array(std::forward<Args>(args)...) {
      build();
    }

    /**
     * It'll rebuild the internal heap, really costful operation
     */
    void
    rebuild(const std::vector<T>& input) {
      _array = input;
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
      bool ok { position < _array.size() };
      if (ok) {
        _array[position] = value;
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
      bool ok { position < _array.size() };
      if (ok) {
        _array[position] = std::move(value);
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
     * @return the const reference to an element at a position
     */
    const T&
    operator[](size_t position) const {
      return _array[position];
    }

    /**
     * @return the const reference to an element at a position
     * @throws std::out_of_range exception if the position isn't lower than the
     * size of the internal array;
     */
    const T&
    at(size_t position) const {
      return _array.at(position);
    }

    /**
     * @return the size of the internal array;
     */
    size_t
    size() const {
      return _array.size();
    }

    /**
     * It resizes the array if, and only if, the new size is greater than
     * the current
     */
    void
    resize(size_t size) {
      if (size > _array.size()) {
        _array.resize(size);
      }
    }

    /**
     * It reserves an ammount of memory in the internal array
     */
    void
    reserve(size_t size) {
      _array.reserve(size);
    }

    /**
     * @return whether the heap is built or not
     */
    bool is_built() const {
      return _built_heap;
    }

    /**
     * It performs a range query for the array.
     * query(1, 3) is equal to query(3, 1)
     *
     * @throws advanced::heap_not_built_exception_t if heap is not built
     */
    range_value_t
    query(size_t start, size_t end) const {
      if (!is_built()) throw heap_not_built_exception_t{};
      auto fixed_start{ std::min(std::min(start, end), size() - 1) };
      auto fixed_end  { std::min(std::max(start, end), size() - 1)};
      return _range_query(fixed_start, fixed_end, head());
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: index of the top of the segment for the entire array
     *    start: 0
     *    end:   size of the array minus 1 (size()-1)
     *  }
     */
    const range_t&
    head() const {
      return _heap.head();
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: right child of the current node
     *    start: [(start+end)/2 + 1, end]
     *    end:   end
     *  }
     */
    const range_t&
    upper_bound(const range_t& node) const {
      return _heap.right_child_of(node.index);
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: right child of the current node
     *    start: start
     *    end:   [(start+end)/2]
     *  }
     */
    const range_t&
    lower_bound(const range_t& node) const {
      return _heap.left_child_of(node.index);
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
     * @returns a new range object containing:
     *  {
     *    start: [(start+end)/2 + 1, end]
     *    end:   end
     *  }
     */
    range_t
    new_upper_bound(const range_t& node) const {
      return { (node.start+node.end) / 2 + 1, node.end };
    }

    /**
     * @returns a new range object containing:
     *  {
     *    start: start
     *    end:   [(start+end)/2]
     *  }
     */
    range_t
    new_lower_bound(const range_t& node) const {
      return { node.start, (node.start + node.end) / 2 };
    }

    /**
     * It performs a range query for the array recursively
     * @return the index of the element which is the top of the range segment
     */
    range_value_t
    _range_query(const range_t& interval, const range_t& node_range) const {
      static const transform_t transform;
      range_value_t value;

      if (interval.start >= node_range.start && interval.end <= node_range.end) {
        // completely inside range
        value = node_range.value;
      }
      else {
        auto left  = _range_query(interval, lower_bound(node_range));
        auto right = _range_query(interval, upper_bound(node_range));
        value      = transform(_array, left, right);
      }
      return value;
    }

    /**
     * It performs a range query updating the internal heap for the array
     * recursively.
     * @return the index of the element which is the top of the range segment
     */
    range_value_t
    update_heap(size_t position, range_value_t new_value, range_t& range) {
      static const transform_t tranform;
      range_value_t right, left;
      if (range.end == range.start && range.start == position) {
        range.value = new_value;
      }
      else if (range.start <= position && range.end >= position){
        left        = update_heap(position, new_value, lower_bound(range));
        right       = update_heap(position, new_value, upper_bound(range));
        range.value = tranform(_array, left, right);
      }
      return range.value;
    }

    /**
     * It'll reserve enough space in the internal heap and fill it with the
     * range nodes in order recursivily
     */
    void
    build() {
      if (size()) {
        _heap.reserve(minimum_required_size_for_head(size()));

        (void)build({ 0, size() - 1 });
        _built_heap = true;
      }
    }

    /**
     * It performs a range query updating the internal heap for the entire array
     * recursively.
     * @return the range segment
     */
    range_value_t
    build(const range_t& range) {
      static const transform_t transform;
      range_value_t value, right, left;
      _heap.push_back({ range });

      range_value_t& node{ _heap.tail() };

      if (range.end > range.start) {
        left       = build(new_lower_bound(range));
        right      = build(new_upper_bound(range));
        node.value = transform(_array, left, right);
      }
      else {
        node.value = transform(_array[range.start], range.start);
      }
      return node.value;
    }

    std::vector<T>          _array;
    heap_t<range_t>         _heap;
    bool                    _built_heap{ false };
  };
}
