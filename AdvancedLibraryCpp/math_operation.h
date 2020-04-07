#pragma once
#include <algorithm>

namespace advanced {
  template <class T>
  struct sum_t {
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs + rhs;
    }
  };

  template <class T>
  struct multiplication_t {
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs * rhs;
    }
  };

  template <class T>
  struct minimum_t {
    inline T operator() (const T& lhs, const T& rhs) const {
      return std::min(lhs, rhs);
    }
  };

  template <class T>
  struct maximum_t {
    inline T operator() (const T& lhs, const T& rhs) const {
      return std::max(lhs, rhs);
    }
  };


}
