#pragma once
#include <algorithm>

namespace advanced {
namespace tools {

  template<class T> struct multiplication_t;
  template<class T> struct sum_t;
  template<class T> struct subtraction_t;
  template<class T> struct division_t;
  template<class T> struct minimum_t;
  template<class T> struct maximum_t;

  /**
   * @brief sum operation declared in template style
   * @see std::less<T>    https://en.cppreference.com/w/cpp/utility/functional/less
   * @see std::greater<T> https://en.cppreference.com/w/cpp/utility/functional/greater
   */
  template <class T>
  struct sum_t {
    using reverse_t = subtraction_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs + rhs;
    }
  };

  /**
   * @brief subtraction operation declared in template style
   * @see std::less<T>    https://en.cppreference.com/w/cpp/utility/functional/less
   * @see std::greater<T> https://en.cppreference.com/w/cpp/utility/functional/greater
   */
  template <class T>
  struct subtraction_t {
    using reverse_t = sum_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs - rhs;
    }
  };

  /**
   * @brief multiplication operation declared in template style
   * @see std::less<T>    https://en.cppreference.com/w/cpp/utility/functional/less
   * @see std::greater<T> https://en.cppreference.com/w/cpp/utility/functional/greater
   */
  template <class T>
  struct multiplication_t {
    using reverse_t = division_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs * rhs;
    }
  };

  /**
   * @brief division_t operation declared in template style
   * @see std::less<T>    https://en.cppreference.com/w/cpp/utility/functional/less
   * @see std::greater<T> https://en.cppreference.com/w/cpp/utility/functional/greater
   */
  template <class T>
  struct division_t {
    using reverse_t = multiplication_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return lhs / rhs;
    }
  };

  /**
   * @brief miminum operation declared in template style
   * @see std::less<T>    https://en.cppreference.com/w/cpp/utility/functional/less
   * @see std::greater<T> https://en.cppreference.com/w/cpp/utility/functional/greater
   */
  template <class T>
  struct minimum_t {
    using reverse_t = maximum_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return std::min(lhs, rhs);
    }
  };

  /**
   * @brief maximum operation declared in template style
   * @see std::less<T>    https://en.cppreference.com/w/cpp/utility/functional/less
   * @see std::greater<T> https://en.cppreference.com/w/cpp/utility/functional/greater
   */
  template <class T>
  struct maximum_t {
    using reverse_t = minimum_t<T>;
    inline T operator() (const T& lhs, const T& rhs) const {
      return std::max(lhs, rhs);
    }
  };

}
}
