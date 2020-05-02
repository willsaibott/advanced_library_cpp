#pragma once
#include <vector>
#include <stdexcept>
#include <limits>
#include "math_operation.h"

namespace advanced {
namespace structures {

  class fenwick_not_built_exception_t : public std::runtime_error {
  public:

    fenwick_not_built_exception_t() :
      std::runtime_error{
        "Attempt to call a method that requires the heap is built"
      } { }
  };

  template <class T, typename transform_t = sum_t<T>>
  class fenwick_tree_t {
  public:

    /**
     * Default constructor. Nothing to do. The heap isn' built
     */
    fenwick_tree_t() = default;

    /**
     * It'll forward to std::vector<T> constructor any given argument
     * and build the entire heap
     */
    fenwick_tree_t(const std::vector<T>& input,
                   size_t number_of_elements = std::numeric_limits<size_t>::max() )
    {
      _build(input, number_of_elements);
    }

    /**
     * It'll rebuild the internal heap, really costful operation
     */
    void
    rebuild(const std::vector<T>& input,
            size_t number_of_elements = std::numeric_limits<size_t>::max() ) {
      _build(input, number_of_elements);
    }

    /**
     * @param position position to update
     * @param value    value to replace
     * It updates a value in a position of the array
     * @return true if the position is lower than the current size
     */
    bool
    update(size_t position, const T&value) {
      bool ok { position < size() };
      if (ok) {
        if (is_built() && value){
          _update(position, value);
        }
        else {
          throw fenwick_not_built_exception_t{};
        }
      }
      return ok;
    }

    /**
     * @return the const reference to an element at a position
     */
    const T&
    operator[](size_t position) const {
      return _processed[position];
    }

    /**
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
     * It resizes the array if, and only if, the new size is greater than
     * the current
     */
    void
    resize(size_t size) {
      if (size > _processed.size()) {
        _processed.resize(size);
      }
    }

    /**
     * It reserves an ammount of memory in the internal array
     */
    void
    reserve(size_t size) {
      _processed.reserve(size);
    }

    /**
     * @return whether the heap is built or not
     */
    bool
    is_built() const {
      return _built_heap;
    }

    /**
     * It performs a range query for the array.
     * query(1, 3) is equal to query(3, 1)
     *
     * @throws advanced::fenwick_not_built_exception_t if heap is not built
     */
    T
    query(size_t start, size_t end) const {
      if (!is_built()) throw fenwick_not_built_exception_t{};
      auto fixed_start{ std::min(std::min(start, end), size() - 1) };
      auto fixed_end  { std::min(std::max(start, end), size() - 1)};
      return _range_query(fixed_start, fixed_end);
    }

    bool
    set_value(size_t position, const T& value) {
      static const typename transform_t::reverse_t transform;
      bool ok { position < _processed.size() };
      if (ok) {
        auto diff = transform(value, _processed[position]);
        if (diff) {
          _update(position, diff);
        }
      }
      return ok;
    }

  private:

    inline static size_t
    lsb(const size_t& index) {
      return index & (-index);
    }

    inline static size_t
    next_query(const size_t& index) {
      return index - lsb(index);
    }

    inline static size_t
    next_update(const size_t& index) {
      return index + lsb(index);
    }

    void
    _build(const std::vector<T>& input, size_t number_of_elements) {
      size_t size = number_of_elements < input.size() ?
                    number_of_elements + 1:
                    input.size() + 1;

      if (size > _processed.size()) {
        _processed.assign(size, static_cast<T>(0));
      }
      else if (is_built()){
        std::fill_n(_processed.begin(), size, static_cast<T>(0));
      }

      for (size_t ii = 1; ii < size; ++ii) {
        _update(ii, input[ii-1]);
      }

      _built_heap = true;
    }

    void
    _update(const size_t& position, const T& value) {
      static const transform_t transform;
      if (position && position < size()) {
        _processed[position] = transform(_processed[position], value);
        _update(next_update(position), value);
      }
    }

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

    T
    _range_query (const size_t& start, const size_t& end) const {
      return _range_query(end) - (start <= 1 ? 0 : _range_query(start - 1));
    }

    std::vector<T>   _processed;
    bool             _built_heap{ false };
  };
}
}
