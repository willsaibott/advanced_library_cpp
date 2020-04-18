#pragma once
#include <algorithm>

namespace advanced {
  template<class T> struct multiplication_t;
  template<class T> struct sum_t;
  template<class T> struct subtraction_t;
  template<class T> struct divisor_t;
  template<class T> struct minimum_t;
  template<class T> struct maximum_t;

  template <class T>
  struct sum_t {
    using reverse_t = subtraction_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs + rhs;
    }
  };

  template <class T>
  struct subtraction_t {
    using reverse_t = sum_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs - rhs;
    }
  };

  template <class T>
  struct multiplication_t {
    using reverse_t = divisor_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs * rhs;
    }
  };

  template <class T>
  struct divisor_t {
    using reverse_t = multiplication_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs / rhs;
    }
  };

  template <class T>
  struct minimum_t {
    using reverse_t = maximum_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return std::min(lhs, rhs);
    }
  };

  template <class T>
  struct maximum_t {
    using reverse_t = minimum_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return std::max(lhs, rhs);
    }
  };


}
