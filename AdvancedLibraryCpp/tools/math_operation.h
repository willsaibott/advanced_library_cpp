#pragma once
#include <algorithm>

namespace advanced {
namespace tools {

/** @test Todo */

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

  inline T operator()(const T& value) const {
    return value;
  }

  template<typename ...Args>
  inline T operator() (const T& lhs, const T& rhs, Args&&... other) const {
    return lhs + this->operator()(rhs, std::forward<Args>(other)...);
  } // LCOV_EXCL_LINE

  template<class A = T>
  typename std::enable_if<std::is_scalar<A>::value, T>::type
  null_value() const {
    return static_cast<T>(0);
  }

  template<class A = T>
  typename std::enable_if<std::is_class<A>::value, T>::type
  null_value() const {
    return T{};
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

  inline T operator()(const T& value) const {
    return value;
  }

  template<typename ...Args>
  inline T operator() (const T& lhs, const T& rhs, Args&&... other) const {
    return lhs * this->operator()(rhs, std::forward<Args>(other)...);
  }

  template<class A = T>
  typename std::enable_if<std::is_scalar<A>::value, T>::type
  null_value() const {
    return static_cast<T>(1);
  }

  template<class A = T>
  typename std::enable_if<std::is_class<A>::value, T>::type
  null_value() const {
    return T{};
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

  inline T operator()(const T& value) const {
    return value;
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

  template<typename ...Args>
  T operator()(const T& lhs, const T& rhs, Args&&... args) const {
    return std::min( std::initializer_list<T>{ lhs, rhs, std::forward<Args>(args)... });
  } // LCOV_EXCL_LINE
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

  template<typename ...Args>
  T operator()(const T& lhs, const T& rhs, Args&&... args) const {
    return std::max( std::initializer_list<T>{ lhs, rhs, std::forward<Args>(args)... });
  } // LCOV_EXCL_LINE
};

}
}
