#include <mutex>
#include <type_traits>

namespace advanced {
namespace concurrency {

template<class T, class mutex_t = std::mutex>
class lockable_t : public T, public mutex_t {
  public:

  template <typename = typename std::enable_if<
                  std::is_default_constructible<T>::value >
                ::type >
  lockable_t() { }

  template <typename ...Args>
  lockable_t(Args&&...args) : T(std::forward<Args>(args)...) { }

  template <typename = typename std::enable_if<
                  std::is_copy_constructible<T>::value >
                ::type >
  lockable_t(const T& other) : T(other) { }

  template <typename = typename std::enable_if<
                  std::is_move_constructible<T>::value >
                ::type >
  lockable_t(T&& other) : T(other) { }

  template <typename = typename std::enable_if<
                  std::is_move_assignable<T>::value >
                ::type >
  inline lockable_t&
  operator=(T&& other) {
    static_cast<T&>(*this) = std::move(other);
    return *this;
  }

  template <typename = typename std::enable_if<
                  std::is_copy_assignable<T>::value >
                ::type >
  inline lockable_t&
  operator=(const T& other) {
    static_cast<T&>(*this) = other;
    return *this;
  }
};

}
} // namespace advanced
