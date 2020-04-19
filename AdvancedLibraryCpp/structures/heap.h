#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <cmath>

namespace advanced {

  template <class T = long long>
  class heap_t : protected std::vector<T> {

  public:

    heap_t() = default;

    template<typename ...Args>
    heap_t(Args...args) : std::vector<T>(std::forward<Args>(args)...) { }

    inline size_t
    left_child_of(size_t pos) const {
      return pos ? (pos << 1ull) + 1ull : 1ull;
    }

    inline size_t
    right_child_of(size_t pos) const {
      return pos ? (pos << 1ull) + 2ull : 2ull;
    }

    inline size_t
    parent_of(size_t pos) const {
      return pos ? (pos - 1) / 2ull : pos;
    }

    inline size_t
    height_of(size_t pos) const {
      return std::log2(pos) + 1;
    }

    /**
     * @param pos position of the node
     * It swaps an element with its parent node
     */
    void
    swap(size_t pos) {
      size_t parent { parent_of(pos) };
      if (pos != parent) {
        T temp           = std::move(this->at(pos));
        this->at(pos)    = std::move(this->at(parent));
        this->at(parent) = std::move(temp);
      }
    }

    /**
     * @param pos position of the node
     * It swaps an element with its left child node
     */
    void
    swap_with_left_child(size_t pos) {
      size_t left { left_child_of(pos) };
      if (left < this->size()) {
        swap(left);
      }
    }

    using std::vector<T>::size;
    using std::vector<T>::assign;
    using std::vector<T>::at;
    using std::vector<T>::operator[];
    using std::vector<T>::reserve;
    using std::vector<T>::resize;
    using std::vector<T>::clear;

    void
    set_left(size_t pos, T&& value) {
      size_t left { left_child_of(pos) };
      if (left > size()) {
        resize(right_child_of(left));
      }
      this->at(left) = std::forward<T>(value);
    }

    void
    set_right(size_t pos, T&& value) {
      size_t right { right_child_of(pos) };
      if (right > size()) {
        resize(right_child_of(right));
      }
      this->at(right) = std::forward<T>(value);
    }

    void
    set_root(T&& value) {
      this->at(0) = std::forward<T>(value);
    }

    /**
     * @param pos position of the node
     * It swaps an element with its right child node
     */
    void
    swap_with_right_child(size_t pos) {
      size_t right { right_child_of(pos) };
      if (right < this->size()) {
        swap(right);
      }
    }

    /**
     * @return the head of the heap, i.e., the front of the vector
     */
    T&
    head() {
      return std::vector<T>::at(0);
    }

    /**
     * const version
     * @return the head of the heap, i.e., the front of the vector
     */
    const T&
    head() const {
      return std::vector<T>::front();
    }

    /**
     * @return the tail of the heap, i.e., the back of the vector
     */
    T&
    tail() {
      return std::vector<T>::back();
    }

    /**
     * @return the tail of the heap, i.e., the back of the vector
     */
    const T&
    tail() const {
      return std::vector<T>::back();
    }
  };
}