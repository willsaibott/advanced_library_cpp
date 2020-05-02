#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <exception>
#include <type_traits>
#include "heap.h"
#include "../tools/math_operation.h"

namespace advanced {
namespace structures {

  class heap_not_built_exception_t : public std::runtime_error {
  public:

    heap_not_built_exception_t() :
      std::runtime_error{
        "Attempt to call a method that requires the heap is built"} { }
  };

  /**
   * struct no update applyable. Default structure for segment template class
   * "update_t".
   */
  struct empty_update_t {
    size_t start;
    size_t end;

    empty_update_t(size_t s, size_t e) : start{ s }, end(e) {}
    empty_update_t() : empty_update_t{0, 0} {}

    empty_update_t(const empty_update_t& other)               = default;
    empty_update_t(empty_update_t&& other) noexcept           = default;
    empty_update_t&operator=(const empty_update_t& other)     = default;
    empty_update_t&operator=(empty_update_t&& other) noexcept = default;

    virtual ~empty_update_t() {}

    virtual inline operator
    bool() const { return false; }
  };

  /**
   * Generic segment tree implementation using a std::vector<T>
   * Template arguments:
   * T:              type of the data_type of the original array
   * range_value_t:  range value type, i.e. what type the range node will store
   * transform_t:    transform struct responsible for all necessary node changes
   *                 (join, build, applying a range update, propagating lazy
   *                  changes, defining a criteria to keep splitting the query
   *                  or not)
   * update_t:       update struct containing all necessary information to
   *                 perform an update in a range. If you don't need updates,
   *                 do not pass it, the default empty_update will be enough
   */
  template <typename T,
            typename range_value_t,
            class    transform_t,
            typename update_t = empty_update_t>
  class segment_tree_t {

  public:

    static const size_t not_found { static_cast<size_t>(-1) };

    /**
     * Node of the segment tree, it stores the value in the field "value" and
     * any update in "lazy_update"
     */
    struct range_node_t {
      size_t start { not_found };
      size_t end   { not_found };
      size_t index;
      range_value_t value;
      update_t      lazy_update;

      range_node_t()                                          = default;
      range_node_t(const range_node_t&)                       = default;
      range_node_t(range_node_t&&) noexcept                   = default;
      inline range_node_t& operator=(const range_node_t&)     = default;
      inline range_node_t& operator=(range_node_t&&) noexcept = default;

      range_node_t(size_t s, size_t e, size_t idx = 0)
        : start{ s }, end{ e }, index{ idx } {}

      /**
       * center point of the range
       */
      inline size_t
      center() const {
        return (start + end) >> 1;
      }

      /**
       * length of the range
       */
      inline size_t
      length() const {
        return end - start;
      }

      /**
       * @return is this a leaf node?
       */
      inline bool
      leaf() const {
        return length() == 0;
      }

      /**
       * @return does it have lazy updates?
       */
      inline bool
      has_lazy_updates() const {
        return static_cast<bool>(lazy_update);
      }

      /**
       * @return is this a valid node?
       */
      inline operator
      bool() const {
        return start != not_found && end != not_found;
      }
    };

    /**
     * Default constructor. Nothing to do. The heap isn't built
     */
    segment_tree_t() = default;

    /**
     * It'll forward to std::vector<T> constructor any given argument
     * and build the entire heap. If you make any modifications in the original
     * array, you must rebuild it.
     */
    segment_tree_t(const std::vector<T>& input,
                   size_t number_of_elements = std::numeric_limits<size_t>::max()) {
      build(input, number_of_elements);
    }

    /**
     * It'll rebuild the internal heap, really costful operation
     */
    void
    rebuild(const std::vector<T>& input,
            size_t number_of_elements = std::numeric_limits<size_t>::max()) {
      build(input, number_of_elements);
    }

    /**
     * @return whether the heap is built or not
     */
    inline bool
    is_built() const {
      return _built_heap;
    }

    /**
     * @return the number of elements the original array have in the moment of
     * creation of the tree
     */
    size_t
    size() const {
      return _size;
    }

    /**
     * It performs a range query for the array.
     * query(1, 3) is equal to query(3, 1)
     *
     * @throw advanced::heap_not_built_exception_t if heap is not built
     * @note  The const modifier was dropped to support lazy updates
     */
    range_value_t
    query(size_t start, size_t end) {
      if (!is_built()) throw heap_not_built_exception_t{};
      static const update_t nothing;
      size_t fixed_start{ std::min(std::min(start, end), size() - 1) };
      size_t fixed_end  { std::min(std::max(start, end), size() - 1)};
      range_value_t result;
      _range_query(fixed_start, fixed_end, head(), result);
      return result;
    }

    /**
     * It performs a range update query for the array.
     * query(1, 3) is equal to query(3, 1)
     *
     * @throw advanced::heap_not_built_exception_t if heap is not built
     */
    range_value_t
    query_update(size_t start, size_t end, const update_t& change) {
      if (!is_built()) throw heap_not_built_exception_t{};
      size_t fixed_start{ std::min(std::min(start, end), size() - 1) };
      size_t fixed_end  { std::min(std::max(start, end), size() - 1)};
      range_value_t result;
      _range_update(fixed_start, fixed_end, head(), change, result);
      return result;
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: index of the top of the segment for the entire array
     *    start: 0
     *    end:   size of the array minus 1 (size()-1)
     *  }
     */
    const range_node_t&
    head() const {
      return _heap.head();
    }

    /**
     * @returns a range object containing:
     *  {
     *    index: index of the top of the segment for the entire array
     *    start: 0
     *    end:   size of the array minus 1 (size()-1)
     *  }
     */
    range_node_t&
    head() {
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
    const range_node_t&
    upper_bound(const range_node_t& node) const {
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
    range_node_t&
    upper_bound(const range_node_t& node) {
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
    const range_node_t&
    lower_bound(const range_node_t& node) const {
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
    range_node_t&
    lower_bound(const range_node_t& node) {
      return _heap[_heap.left_child_of(node.index)];
    }

    /**
     * @return the minimum memory space required for the heap for a given array
     * size
     */
    inline static size_t
    minimum_required_size_for_head(size_t size) {
      return static_cast<size_t>(std::ceil(std::pow(2.0, std::log2(size) + 2)));
    }

  protected:

    range_node_t
    right_interval(const range_node_t& interval, size_t center) const {
      return { std::max(interval.start, center+1), interval.end };
    }

    range_node_t
    left_interval(const range_node_t& interval, size_t center) const {
      return { interval.start, std::min(interval.end, center) };
    }

    /**
     * @returns a new range object containing:
     *  {
     *    start: [(start+end)/2 + 1, end]
     *    end:   end
     *  }
     */
    range_node_t
    new_upper_bound(const range_node_t& node) const {
      return { node.center() + 1, node.end, _heap.right_child_of(node.index) };
    }

    /**
     * @returns a new range object containing:
     *  {
     *    start: start
     *    end:   [(start+end)/2]
     *  }
     */
    range_node_t
    new_lower_bound(const range_node_t& node) const {
      return { node.start, node.center(), _heap.left_child_of(node.index) };
    }

    /**
     * It performs a range query for the array recursively.
     * @note  The const modifier was dropped to support lazy updates
     * @param start      (in)  start of the interval of the query
     * @param end        (in)  end of the interval of the query
     * @param node_range (in)  current node in the recursion
     * @param value      (out) stored value to be returned
     */
    virtual void
    _range_query(size_t         start,
                 size_t         end,
                 range_node_t&  node_range,
                 range_value_t& value)
    {
      static const transform_t transform;

      if (node_range.start == start && end == node_range.end) {
        // completely inside range and matched the criteria
        if (node_range.leaf()) {
          value = node_range.value;
          return;
        }
        else if (!node_range.has_lazy_updates()) {
          value = node_range.value;
          return;
        }
      }
      if (!node_range.leaf()) {
        const auto center = node_range.center();
        auto& lbound{ lower_bound(node_range) };
        auto& rbound{ upper_bound(node_range) };

        if (node_range.has_lazy_updates()) {
          // Propagate them to children nodes:
          transform.propagate(node_range, lbound, rbound);
        }

        if (start <= center && end > center) {
          range_value_t left, right;
          _range_query(start, std::min(end, center),  lbound, left);
          _range_query(center+1, end, rbound, right);
          transform.join(value, left, right);
        }
        else if (start <= center) {
          _range_query(start, end, lbound, value);
        }
        else if (end > center) {
          _range_query(start, end, rbound, value);
        }
        // join the nodes back to keep consistency with changes in the recursion
        transform.join(node_range.value, lbound.value, rbound.value);
      }
    }

    /**
     * It performs a range update query for the array recursively.
     * @param start      (in)  start of the interval of the query
     * @param end        (in)  end of the interval of the query
     * @param node_range (in)  current node in the recursion
     * @param change     (in)  object representing a change in a range
     * @param value      (out) stored value to be returned
     */
    virtual void
    _range_update(size_t                start,
                  size_t                end,
                  range_node_t&         node_range,
                  const update_t&       change,
                  range_value_t&        value)
    {
      static const transform_t transform;

      if (node_range.start == start && end == node_range.end) {
        // Completely inside range and matched the criteria
        if (node_range.leaf()) {
          if (change) {
            transform.apply(node_range, change);
          }
          value = node_range.value;
          return;
        }
        else if (!node_range.has_lazy_updates() || transform.criteria(node_range)) {
          if (change) {
            transform.apply(node_range, change);
          }
          value = node_range.value;
          return;
        }
      }
      if (!node_range.leaf()) {
        const auto center = node_range.center();
        auto& lbound{ lower_bound(node_range) };
        auto& rbound{ upper_bound(node_range) };

        if (node_range.has_lazy_updates()) {
          // Propagate them to children nodes:
          transform.propagate(node_range, lbound, rbound);
        }

        if (start <= center && end > center) {
          range_value_t left, right;
          _range_update(start, center, lbound, change, left);
          _range_update(center+1, end, rbound, change, right);
          transform.join(value, left, right);
        }
        else if (start <= center) {
          _range_update(start, end, lbound, change, value);
        }
        else if (end > center) {
          _range_update(start, end, rbound, change, value);
        }
        // join the nodes back to keep consistency with changes in the recursion
        transform.join(node_range.value, lbound.value, rbound.value);
      }
    }

    /**
     * It'll reserve enough space in the internal heap and fill it with the
     * range nodes in order recursivily
     */
    void
    build(const std::vector<T>& input, size_t number_of_elements) {
      _size = number_of_elements < input.size() ? number_of_elements : input.size();
      if (size()) {
        auto new_size = minimum_required_size_for_head(size());
        if (_heap.size() < new_size) {
          _heap.resize(new_size);
        }
        _heap.set_root({ 0, size() - 1, 0 });
        build(input, _heap.head());
        _built_heap = true;
      }
    }

    /**
     * It performs a range query updating the internal heap for the entire array
     * recursively.
     * @return the range segment
     */
    void
    build(const std::vector<T>& input, range_node_t& range) {
      static const transform_t transform;

      if (range.end > range.start) {
        auto& lbound{ lower_bound(range) };
        auto& rbound{ upper_bound(range) };
        auto center  = range.center();
        lbound.start = range.start;
        lbound.end   = center;
        rbound.start = center+1;
        rbound.end   = range.end;
        lbound.index = _heap.left_child_of(range.index);
        rbound.index = _heap.right_child_of(range.index);
        lbound.lazy_update = update_t{};
        rbound.lazy_update = update_t{};
        build(input, lbound);
        build(input, rbound);
        transform.join(range.value, lbound.value, rbound.value);
      }
      else {
        transform.build(range.value, input[range.start], range.start);
      }
    }

    size_t                  _size{ 0 };
    size_t                  _changes_not_applied{ 0 };
    heap_t<range_node_t>    _heap;
    bool                    _built_heap{ false };
  };

  /**
   * Segment tree node transform Interface.
   * Please, override it in your class and implement the necessary methods
   * All the methods were transformed into inline versions to improve
   * performance.
   * Old exercises may not compile because of this. I'm sorry for the trouble
   */
  template <class T,
            class range_value_t,
            class parent_class,
            class update_t = empty_update_t>
  struct segment_node_t {

    using node_t = typename segment_tree_t<T, range_value_t, parent_class, update_t>::range_node_t;

    /**
     * method to be called when two nodes, left and right have their values
     * combined in the parent. Implement them as you wish.
     */
    virtual void
    join(range_value_t&       parent,
         const range_value_t& left,
         const range_value_t& right) const = 0;

    /**
     * On a range_update query, if the node isn't a leaf, but the it matched
     * perfectly a range in the tree, the criteria boolean method will be called.
     * If this method returns true, it won't split the query into the children
     * nodes and will the query methods will return the current value, otherwise
     * the search will continue until either this function has been satisfied or
     * the node is a leaf.
     */
    virtual bool
    criteria(const node_t& range) const {
      return true;
    }

    /**
     * Method called when the current node has lazy updates and must propagate
     * them to children. This is logic dependent
     */
    virtual void
    propagate(node_t& node, node_t&left, node_t&right) const {  }

    /**
     * Method called when an update is applied in the node. You may need to call
     * it in propagate.
     */
    virtual void
    apply(node_t& node, const update_t& update) const {  }

    /**
     * It Builds the leaf nodes
     */
    virtual void
    build(range_value_t& value, const T& element, const size_t& position) const = 0;
  };

  template <class T, class operation_t, typename update_t >
  struct range_operation_t : segment_node_t<T, T, range_operation_t<T, operation_t, update_t>, update_t> {

    inline virtual void
    join(T& parent, const size_t& left, const size_t& right) const override {
      static const operation_t operation;
      parent = operation(left, right);
    }

    inline virtual void
    build(T& value, const T& element, const size_t& position) const override {
      value = element;
    }

  };

  template<class T, typename update_t = empty_update_t>
  using range_sum_t = range_operation_t<T, sum_t<T>, update_t>;

  template<class T, typename update_t = empty_update_t>
  using range_multiplication_t = range_operation_t<T, multiplication_t<T>, update_t>;

  template<class T, typename update_t = empty_update_t>
  using range_min_t = range_operation_t<T, minimum_t<T>, update_t>;

  template<class T, typename update_t = empty_update_t>
  using range_max_t = range_operation_t<T, maximum_t<T>, update_t>;
}
}
