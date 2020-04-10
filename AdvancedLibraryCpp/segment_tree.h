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
    join(const std::vector<T>& array,
         const range_value_t&  left,
         const range_value_t&  right) const = 0;

    /**
     * criteria boolean flag for stopping in the range, and not split the
     * interval query anymore
     */
    virtual bool
    criteria(const range_value_t& range) const {
      return true;
    }

    /**
     * function to make any necessary updates on lazy resolution operations
     */
    virtual void
    propagate(range_value_t& range, range_value_t& left, range_value_t& right) {

    }

    /**
     * transform operator for leaf nodes
     */
    virtual range_value_t
    build(const T& element, const size_t& position) const = 0;
  };

  template <class T, class compare_t = std::less<T>>
  struct index_compare_t : segment_node_t<T, size_t> {
    static const size_t not_found { static_cast<size_t>(-1) };

    inline virtual size_t
    join(const std::vector<T>& array,
         const size_t&         left,
         const size_t&         right) const override
    {
      static const compare_t compare;
      return compare(array[left], array[right]) ? left : right;
    }

    inline virtual size_t
    build(const T& element, const size_t& position) const override {
      return position;
    }
  };

  template <class T, class operation_t>
  struct range_operation_t : segment_node_t<T, T> {

    inline virtual T
    join(const std::vector<T>& array,
         const size_t&         left,
         const size_t&         right) const override
    {
      static const operation_t operation;
      return operation(left, right);
    }

    inline virtual T
    build(const T& element, const size_t& position) const override {
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
  template <typename T,
            typename range_value_t   = size_t,
            class    transform_t     = index_compare_t<T>>
  class segment_tree_t {
  public:

    static const size_t not_found { static_cast<size_t>(-1) };

    struct range_t {
      size_t start { not_found };
      size_t end   { not_found };
      size_t index;
      range_value_t value;

      range_t()                                     = default;
      range_t(const range_t&)                       = default;
      range_t(range_t&&) noexcept                   = default;
      inline range_t& operator=(const range_t&)     = default;
      inline range_t& operator=(range_t&&) noexcept = default;

      range_t(size_t s, size_t e, size_t idx=0) : start{s}, end{e}, index{idx} {}

      size_t center() const {
        return (start + end) / 2;
      }

      size_t length() const {
        return end - start;
      }
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
    segment_tree_t(Args&&... args) : _array( std::forward<Args>(args)... ) {
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
      size_t fixed_start{ std::min(std::min(start, end), size() - 1) };
      size_t fixed_end  { std::min(std::max(start, end), size() - 1)};
      return _range_query({ fixed_start, fixed_end } , head());
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
      return _heap[_heap.right_child_of(node.index)];
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: right child of the current node
     *    start: [(start+end)/2 + 1, end]
     *    end:   end
     *  }
     */
    range_t&
    upper_bound(const range_t& node) {
      return _heap[_heap.right_child_of(node.index)];
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
      return _heap[_heap.left_child_of(node.index)];
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: right child of the current node
     *    start: start
     *    end:   [(start+end)/2]
     *  }
     */
    range_t&
    lower_bound(const range_t& node) {
      return _heap[_heap.left_child_of(node.index)];
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

    range_t
    right_interval(const range_t& interval, size_t center) const {
      return { std::max(interval.start, center+1), interval.end };
    }

    range_t
    left_interval(const range_t& interval, size_t center) const {
      return { interval.start, std::min(interval.end, center) };
    }

    /**
     * @returns a new range object containing:
     *  {
     *    start: [(start+end)/2 + 1, end]
     *    end:   end
     *  }
     */
    range_t
    new_upper_bound(const range_t& node) const {
      return { (node.start+node.end) / 2 + 1, node.end, _heap.right_child_of(node.index) };
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
      return { node.start, (node.start + node.end) / 2, _heap.left_child_of(node.index) };
    }

    /**
     * It performs a range query for the array recursively
     * @return the index of the element which is the top of the range segment
     */
    range_value_t
    _range_query(const range_t& interval, const range_t& node_range) const {
      static const transform_t transform;
      range_value_t value;

      if (node_range.start == interval.start && interval.end == node_range.end) {
        // completely inside range
        if (transform.criteria(node_range.value)) {
          return value = node_range.value;
        }
      }
      if (node_range.length()) {
        const auto& lbound{ lower_bound(node_range) };
        const auto& rbound{ upper_bound(node_range) };
        const auto center = node_range.center();
        if (interval.start <= center && interval.end > center) {
          auto left  = _range_query(left_interval(interval, center),  lbound);
          auto right = _range_query(right_interval(interval, center), rbound);
          value      = transform.join(_array, left, right);
        }
        else if (interval.start <= center) {
          value      = _range_query(left_interval(interval, center), lbound);
        }
        else if (interval.end > center) {
          value      = _range_query(right_interval(interval, center), rbound);
        }
      }
      return value;
    }

    /**
     * It performs a range query for the array recursively
     * @return the index of the element which is the top of the range segment
     */
    range_value_t
    _range_update(const range_t& interval, range_t& node_range) {
      static const transform_t transform;

      if (node_range.start == interval.start && interval.end == node_range.end) {
        // completely inside range
        if (transform.criteria(node_range)) {
          return node_range.value = interval.value;
        }
      }
      if (node_range.length()) {
        const auto& lbound{ lower_bound(node_range) };
        const auto& rbound{ upper_bound(node_range) };
        const auto center = node_range.center();
        transform.propagate(node_range, lbound, rbound);

        if (interval.start <= center && interval.end > center) {
          auto left        = _range_update(left_interval(interval, center),  lbound);
          auto right       = _range_update(right_interval(interval, center), rbound);
          node_range.value = transform.join(_array, left, right);
        }
        else if (interval.start <= center) {
          node_range.value = _range_update(left_interval(interval, center), lbound);
        }
        else if (interval.end > center) {
          node_range.value= _range_update(right_interval(interval, center), rbound);
        }
      }
      return node_range.value;
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
        range.value = tranform.join(_array, left, right);
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
        _heap.resize(minimum_required_size_for_head(size()));
        _heap.set_root({ 0, size() - 1, 0 });
        (void)build(_heap.head());
        _built_heap = true;
      }
    }

    /**
     * It performs a range query updating the internal heap for the entire array
     * recursively.
     * @return the range segment
     */
    range_value_t
    build(range_t& range) {
      static const transform_t transform;
      range_value_t right, left;

      if (range.end > range.start) {
        _heap.set_left(range.index,  new_lower_bound(range));
        _heap.set_right(range.index, new_upper_bound(range));
        left        = build(lower_bound(range));
        right       = build(upper_bound(range));
        range.value = transform.join(_array, left, right);
      }
      else {
        range.value = transform.build(_array[range.start], range.start);
      }
      return range.value;
    }

    std::vector<T>          _array;
    heap_t<range_t>         _heap;
    bool                    _built_heap{ false };
  };
}
