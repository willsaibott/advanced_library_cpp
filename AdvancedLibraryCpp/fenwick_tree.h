#pragma once
#include <vector>
#include <stdexcept>
#include "math_operation.h"

namespace advanced {

  class fenwick_not_built_exception_t : public std::runtime_error {
  public:

    fenwick_not_built_exception_t() :
      std::runtime_error{
        "Attempt to call a method that requires the heap is built"
      } { }
  };

  template <class T>
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
    template<typename ...Args>
    fenwick_tree_t(Args... args) : _array(std::forward<Args>(args)...) {
      _build();
    }

    /**
     * It'll rebuild the internal heap, really costful operation
     */
    void
    rebuild(const std::vector<T>& input) {
      _array = input;
      _build();
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
        if (is_built()){
          _update(position, _array[position] - value);
        }
        else {
          _array[position] = value;
          _build();
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
     * @throws advanced::fenwick_not_built_exception_t if heap is not built
     */
    T
    query(size_t start, size_t end) const {
      if (!is_built()) throw fenwick_not_built_exception_t{};
      auto fixed_start{ std::min(std::min(start, end), size() - 1) };
      auto fixed_end  { std::min(std::max(start, end), size() - 1)};
      return _range_query(fixed_start, fixed_end);
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

    void _build() {
      _processed.resize(_array.size(), static_cast<T>(0));

      for (size_t ii = 0; ii < _array.size(); ++ii) {
        _update(ii, _array[ii]);
      }

      _built_heap = true;
    }

    void _update(const size_t& position, const T& value) {
      if (position < size()) {
        _processed[position] += value;
        _update(next_update(position), value);
      }
    }

    T _range_query (const size_t& start, const size_t& end) {
      if (!start) {
        size_t index = end;
        T value = _processed[index];
        while((index = next_query(index))) {
          value += _processed[index];
        }
        return value;
      }
      return _range_query(0, end) - _range_query(0, start - 1);
    }

    std::vector<T>   _array;
    std::vector<T>   _processed;
    bool             _built_heap{ false };
  };
}
