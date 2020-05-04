#include <mutex>
#include <type_traits>

namespace advanced {
namespace concurrency {

/**
 * class lockable_t is a wrapper for an object that makes it lockable because
 * it inherits the original object class and also the mutex_t passed as template
 * argument
 *
 * @see TestLockable at test_lockable for examples of usage.
 */
template<class T, class mutex_t = std::mutex>
class lockable_t : public T, public mutex_t {
  public:

  /**
   * Enable the default destructor if the original type is default constructible
   */
  template <typename = typename std::enable_if<
                  std::is_default_constructible<T>::value >
                ::type >
  lockable_t() { }

  /**
   * It creates any constructor the original class had
   */
  template <typename ...Args>
  lockable_t(Args&&...args) : T(std::forward<Args>(args)...) { }

  /**
   * Enable the copy constructor if the original type was copy constructible
   */
  template <typename = typename std::enable_if<
                  std::is_copy_constructible<T>::value >
                ::type >
  lockable_t(const T& other) : T(other) { }

  /**
   * Enable the move constructor if the original type was move constructible
   * It originally was noexcept, but some std types like std::queue isn't
   * noexcept, so, this condition was relaxed to allow extend this types
   */
  template <typename = typename std::enable_if<
                  std::is_move_constructible<T>::value >
                ::type >
  lockable_t(T&& other) : T(other) { }

  /**
   * Enable the move assignment operator if the original type was too.
   * It originally was noexcept, but some std types like std::queue isn't
   * noexcept, so, this condition was relaxed to allow extend this types
   */
  template <typename = typename std::enable_if<
                  std::is_move_assignable<T>::value >
                ::type >
  inline lockable_t&
  operator=(T&& other) {
    static_cast<T&>(*this) = std::move(other);
    return *this;
  }

  /**
   * Enable the copy assignment if the original type was copy constructible
   */
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
