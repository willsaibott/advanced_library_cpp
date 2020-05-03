#pragma once
#include <cstddef>
#include <chrono>
#include <condition_variable>
#include <shared_mutex>

namespace advanced {
namespace concurrency {

/**
 * Simple semaphore implementation
 */
class semaphore_t {
  public:

  semaphore_t(semaphore_t &&)                 = default; // movable
  semaphore_t(const semaphore_t &)            = delete;  // non-copyable
  semaphore_t &operator=(semaphore_t &&)      = default; // movable
  semaphore_t &operator=(const semaphore_t &) = delete;  // non-copyable
  virtual ~semaphore_t() {}

  /**
   * Constructor
   * @param max_counter   number of allowed locks simultaneuos
   * @param timeout_ms    timeout_ms of condition variable
   */
  semaphore_t(size_t max_counter, size_t timeout_ms = 100)
      : _max_counter(max_counter), _timeout_ms(timeout_ms)
  { }

  /**
   * wait for an available resource to be allocated
   * if the total of resources allocated is greater than or equal to the
   * max counter, it will sleep while the resources aren't available
   */
  inline void
  lock() {
    std::unique_lock guard{ _mtx };
    while (_count >= _max_counter) {
      _cv.wait_for(guard, _timeout_ms);
    }
    _count++;
  }

  /**
   * If there's at least one resource available, allocate it and return true,
   * if not, return false without acquiring any resources
   */
  inline bool
  try_lock() {
    std::unique_lock guard{ _mtx };
    bool lock_acquired{ false };
    if ((lock_acquired = (_count < _max_counter))) {
      _count++;
    }
    return lock_acquired;
  }

  /**
   * Free a previous allocated resource
   */
  inline void
  unlock() {
    decrecement();
    _cv.notify_one();
  }

  /**
   * It changes the max available resources, if the new max_counter is greater
   * than the previous one, it'll notify the internal conditional variable
   */
  inline void
  set_max_counter(size_t max_counter) {
    if (set_max_counter_locked(max_counter)) {
      _cv.notify_one();
    }
  }

  /**
   * @returns Total of allocated resources
   */
  inline size_t
  count() const noexcept {
    std::unique_lock guard{ _mtx };
    return _count;
  }

  /**
   * @returns Max available resources
   */
  inline size_t
  max_count() const noexcept {
    return _max_counter;
  }

  /**
   * @returns Total of allocated resources
   */
  inline operator
  size_t() const noexcept {
    return count();
  }

  private:

  /**
   * decrement the counter atomically
   */
  inline void
  decrecement() {
    std::unique_lock guard{ _mtx };
    _count--;
  }

  /**
   * set max counter atomically
   */
  inline bool
  set_max_counter_locked(size_t max_counter) {
    std::unique_lock guard{ _mtx };
    bool is_greater_than{ max_counter > _max_counter };
    _max_counter = max_counter;
    return is_greater_than;
  }

  mutable std::mutex              _mtx;
  std::condition_variable         _cv;
  size_t                        _count{ 0ull };
  size_t                        _max_counter;
  const std::chrono::milliseconds _timeout_ms;
};

}
}
