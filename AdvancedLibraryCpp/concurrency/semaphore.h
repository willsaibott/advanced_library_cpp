#pragma once
#include <cstddef>
#include <chrono>
#include <condition_variable>
#include <shared_mutex>

namespace advanced {
namespace concurrency {

/** @test TestSemaphore in test/test_semaphore(.h|.cpp) */

/**
 * Simple semaphore implementation
 */
class semaphore_t {
  public:

  class out_of_resources_exception : public std::runtime_error {
  public:
    out_of_resources_exception(const std::string& msg)
      : std::runtime_error{ msg } { }
  };

  semaphore_t(semaphore_t &&)                 = default; // movable
  semaphore_t(const semaphore_t &)            = delete;  // non-copyable
  semaphore_t &operator=(semaphore_t &&)      = default; // movable
  semaphore_t &operator=(const semaphore_t &) = delete;  // non-copyable
  virtual ~semaphore_t() {}

  /**
   * Constructor
   * @param available_resources   number of allowed simultaneuos locks
   * @param timeout_ms            timeout_ms of condition variable
   */
  semaphore_t(size_t available_resources, size_t timeout_ms = 100)
      : _available_resources(available_resources), _timeout_ms(timeout_ms)
  { }

  /**
   * wait for an available resource to be allocated
   * if the total of resources allocated is greater than or equal to the
   * max counter, it will sleep while the resources aren't available
   * @throws out_of_resources_exception if available resources is zero
   */
  inline semaphore_t&
  lock() {
    std::unique_lock<std::mutex> guard(_mtx);
    if (!_available_resources) {
      guard.unlock();
      throw out_of_resources_exception{ "Available resource is zero" };
    }

    while (_count >= _available_resources) {
      _cv.wait_for(guard, _timeout_ms);
    }
    _count++;

    return *this;
  }

  /**
   * If there's at least one resource available, allocate it and return true,
   * if not, return false without acquiring any resources
   */
  inline bool
  try_lock() noexcept {
    std::unique_lock<std::mutex> guard(_mtx);
    bool lock_acquired{ false };
    if ((lock_acquired = (_count < _available_resources))) {
      _count++;
    }
    return lock_acquired;
  }

  /**
   * Free a previous allocated resource
   */
  inline semaphore_t&
  unlock() {
    if (decrecement()) {
      _cv.notify_one();
    }
    return *this;
  }

  /**
   * It changes the max available resources, if the new max_counter is greater
   * than the previous one, it'll notify the internal conditional variable
   */
  inline semaphore_t&
  set_available_resources(size_t available_resources) {
    if (set_max_counter_locked(available_resources)) {
      _cv.notify_one();
    }
    return *this;
  }

  /**
   * @returns Total of allocated resources
   */
  inline size_t
  count() const noexcept {
    std::unique_lock<std::mutex> guard(_mtx);
    return _count;
  }

  /**
   * @returns Max available resources
   */
  inline size_t
  max_count() const noexcept {
    return _available_resources;
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
  inline bool
  decrecement() {
    std::unique_lock<std::mutex> guard(_mtx);
    bool ok{ false };
    if (_count) {
      _count--;
      ok = true;
    }
    return ok;
  }

  /**
   * set max counter atomically
   */
  inline bool
  set_max_counter_locked(size_t available_resources) {
    std::unique_lock<std::mutex> guard(_mtx);
    bool is_greater_than{ available_resources > _available_resources };
    _available_resources = available_resources;
    return is_greater_than;
  }

  mutable std::mutex              _mtx;
  std::condition_variable         _cv;
  size_t                        _count{ 0ull };
  size_t                        _available_resources;
  const std::chrono::milliseconds _timeout_ms;
};

}
}
