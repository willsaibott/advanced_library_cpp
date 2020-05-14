#pragma once
#include <vector>
#include <stdexcept>
#include <limits>
#include "../tools/math_operation.h"

namespace advanced {
namespace structures {

/** @test ../test/test_fenwick_tree.cpp */

class fenwick_not_built_exception_t : public std::runtime_error {
public:

  fenwick_not_built_exception_t() :
    std::runtime_error {
      "Attempt to call a method that requires the heap is built"
    } { }
};

/**
 * @brief Templated Fenwick Tree
 * It works for all commutative operations
 *
 * @tparam T            data type of the std::vector that contains a sequence
 * @tparam transform_t
 *
 * For more information about the expected behaviour:
 * @see https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
 */
template <class T, typename transform_t = tools::sum_t<T>>
class fenwick_tree_t {
public:

  /**
   * @brief Default constructor. Nothing to do. The internal heap isn't built
   */
  fenwick_tree_t() = default;

  /**
   * @brief It'll forward to std::vector<T> constructor any given argument
   * and build the entire heap
   */
  fenwick_tree_t(const std::vector<T>& input,
                 size_t number_of_elements = std::numeric_limits<size_t>::max())
  {
    _build(input, number_of_elements);
  }

  /**
   * @brief fenwick_tree_t Move constructor. It unbuilds the other tree
   * @param other
   */
  fenwick_tree_t(fenwick_tree_t&& other) noexcept {
    *this = std::move(other);
  }

  /**
   * @brief operator = Move assignment.  It unbuilds the other tree
   * @param other
   * @return reference to this
   */
  fenwick_tree_t& operator=(fenwick_tree_t&& other) noexcept {
    this->_built_heap = other._built_heap;
    other._built_heap = false;
    _processed = std::move(other._processed);
    return *this;
  }

  fenwick_tree_t(const fenwick_tree_t& other)             = default;
  fenwick_tree_t& operator=(const fenwick_tree_t& other)  = default;

  /**
   * @brief It'll rebuild the internal heap, really costful operation
   */
  void
  rebuild(const std::vector<T>& input,
          size_t number_of_elements = std::numeric_limits<size_t>::max() ) {
    _build(input, number_of_elements);
  }

  /**
   * @brief It updates a value in a position of the array
   * @param position position to update
   * @param value    value to replace
   * @throws advanced::fenwick_not_built_exception_t if heap is not built
   * @return true if the position is lower than the current size
   */
  bool
  update(size_t position, const T&value) {
    if (!is_built()) { throw fenwick_not_built_exception_t{}; }
    bool ok { position < size() };
    if (ok) {
      if (value){
        _update(position, value);
      }
    }
    return ok;
  } // LCOV_EXCL_LINE

  /**
   * @brief  the const reference to an element at a position
   * @return the const reference to an element at a position
   * @throws std::out_of_range exception if the position isn't lower than the
   * size of the internal array;
   */
  const T&
  operator[](size_t position) const {
    return _processed.at(position);
  }

  /**
   * @brief  the const reference to an element at a positions
   * @return the const reference to an element at a position
   * @throws std::out_of_range exception if the position isn't lower than the
   * size of the internal array;
   */
  const T&
  at(size_t position) const {
    return _processed.at(position);
  }

  /**
   * @return the size of the internal array;
   */
  size_t
  size() const {
    return _processed.size();
  }

  /**
   * @brief It downsizes the internal array. If the size is greater than the
   * current size, nothing is done.
   * @param size of the internal array
   */
  void
  resize(size_t size) {
    if (size < _processed.size()) {
      _processed.resize(size);
    }
  }

  /**
   * @brief reserve the necessary ammount of memory in the internal array
   * It reserves an ammount of memory in the internal array
   */
  void
  reserve(size_t size) {
    _processed.reserve(size);
  }

  /**
   * @brief is_built whether the heap is built or not
   * @return whether the heap is built or not
   */
  bool
  is_built() const {
    return _built_heap;
  }

  /**
   * @brief It performs a range query for the array.
   * @note query({ 1, 1.0 }, 3) is equal to query({ 3, 3.0 }, 1)
   * @throws advanced::fenwick_not_built_exception_t if heap is not built
   */
  T
  query(size_t start, size_t end) const {
    if (!is_built()) { throw fenwick_not_built_exception_t{}; }
    auto fixed_start{ std::min(std::min(start, end), size() - 1) };
    auto fixed_end  { std::min(std::max(start, end), size() - 1)};
    return _range_query(fixed_start, fixed_end);
  } // LCOV_EXCL_LINE

  /**
   * @brief set_value  it enforces a value into a position using the typename
   * reverse_t defined in the transform template structure
   *
   * @param position  position to set
   * @param value     value to set
   * @throws advanced::fenwick_not_built_exception_t if heap is not built
   * @return          whether the position was valid or not
   */
  bool
  set_value(size_t position, const T& value) {
    static const typename transform_t::reverse_t reverse;
    if (!is_built()) { throw fenwick_not_built_exception_t{}; }
    bool ok { position < _processed.size() && position > 0};
    if (ok) {
      const auto value_at_position{ _range_query(position, position) };
      const auto diff{ reverse(value, value_at_position) };
      if (diff) {
        _update(position, diff);
      }
    }
    return ok;
  } // LCOV_EXCL_LINE

private:

  /**
   * @brief lsb    lowest significant bit
   * @param index  index of the element
   * @return       lowest significant bit
   */
  inline static size_t
  lsb(const size_t& index) {
    return index & (-index);
  }

  /**
   * @brief next_query next index that depends on this index for queries
   * @param index      current index
   * @return           next index that depends on this index for queries
   */
  inline static size_t
  next_query(const size_t& index) {
    return index - lsb(index);
  }

  /**
   * @brief next_update next index that depends on this index for updates
   * @param index       current index
   * @return            next index that depends on this index for updates
   */
  inline static size_t
  next_update(const size_t& index) {
    return index + lsb(index);
  }

  /**
   * @brief _build  build the internal binary indexed vector from a input vector
   * @param input              input vector
   * @param number_of_elements number of elements from the vector to build the
   *                           binary indexed vector
   */
  void
  _build(const std::vector<T>& input, size_t number_of_elements) {
    static const transform_t transform;
    size_t size = number_of_elements < input.size() ?
                  number_of_elements + 1 :
                  input.size() + 1;

    if (size > _processed.size()) {
      _processed.assign(size, transform.null_value());
    }
    else if (is_built()) {
      _processed.resize(size);
      std::fill_n(_processed.begin(), size, transform.null_value());
    }

    for (size_t ii = 1; ii < size; ++ii) {
      _update(ii, input[ii-1]);
    }

    _built_heap = true;
  }

  /**
   * @brief _update   updates a position, and it's dependents indexes
   * @param position  position to be updated
   * @param value     value to apply the transform operation in the given
   *                  position
   */
  void
  _update(const size_t& position, const T& value) {
    static const transform_t transform;
    if (position && position < size()) {
      _processed[position] = transform(_processed[position], value);
      _update(next_update(position), value);
    }
  }

  /**
   * @brief _range_query  range query operation, it combines all binary indexes
   *                      to answer as a unique value representing the range
   * @param position
   * @return the value after a transform operation{ sum, multiplication... }
   */
  T
  _range_query (const size_t& position) const {
    static const transform_t transform;
    size_t index = position;
    T value = _processed[index];
    while((index = next_query(index))) {
      value = transform(_processed[index], value);
    }
    return value;
  }

  /**
   * @brief _range_query  range query operation, it combines all binary indexes
   *                      to answer as a unique value representing the range
   * @param position
   *
   * @note Class version
   * @return the value after a transform operation{ sum, multiplication... }
   */
  template<class A = T>
  typename std::enable_if<std::is_class<A>::value, T>::type
  _range_query (const size_t& start, const size_t& end) const {
    static const typename transform_t::reverse_t discard;
    auto discarded{ (start <= 1 ? A{} : _range_query(start - 1)) };
    auto value { _range_query(end) };
    return discarded ? discard(value, discarded) : value;
  }

  /**
   * @brief _range_query  range query operation, it combines all binary indexes
   *                      to answer as a unique value representing the range
   * @param position
   * @note  scalar version
   * @return the value after a transform operation{ sum, multiplication... }
   */
  template<class A = T>
  typename std::enable_if<std::is_scalar<A>::value, T>::type
  _range_query (const size_t& start, const size_t& end) const {
    static const typename transform_t::reverse_t discard;
    auto discarded{ (start <= 1 ? static_cast<T>(0) : _range_query(start - 1)) };
    auto value { _range_query(end) };
    return discarded ? discard(value, discarded) : value;
  }

  std::vector<T>   _processed;
  bool             _built_heap{ false };
};

}
}
