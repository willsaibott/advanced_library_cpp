#pragma once

#include <functional>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include "thread.h"
#include "../tools/timestamp.h"

namespace advanced {
namespace concurrency {

/** @test TestTimer in tests/test_timer(.h|.cpp) */

/**
 * Base class for a timer that executes the function on_interval every
 * timeout_ms.
 *
 * Note:
 * If you override on_start and on_stop functions, do not forget to call
 * the "safe_delete" method on destructor
 * Also, the on_start and on_stop methods won't be executed if the destructor
 * object of base_timer_t is called before the timer has been stopped
 */
class base_timer_t : public thread_t {
  public:

  base_timer_t() noexcept
  { }

  template <class T>
  base_timer_t(T interval_ms) noexcept
    :  _interval_ms(static_cast<size_t>(interval_ms))
  { }

  virtual
  ~base_timer_t() override {
    std::unique_lock<std::mutex> guard{ _deletion_mtx };
    _deleted = true;
    guard.unlock();
    safe_delete();
  }

  virtual int
  on_interval(void) = 0;

  /**
   * Stops the timer. If the should join parameter (default: false) is true,
   * the current thread joins the thread.
   * on_stop event handler is called.
   */
  inline void
  stop(bool should_join = false) {
    _is_running = false;
    on_stop();
    _cv.notify_one();
    if (should_join) {
      join();
    }
  }

  /**
   * It returns whether the current timer is running or not.
   */
  inline bool
  is_running() {
    return _is_running && thread_t::is_running();
  }

  /**
   * It sets the timer interval (ms) to a value passed by parameter.
   */
  inline void
  set_interval(size_t interval_ms) {
    _interval_ms = std::chrono::milliseconds{ interval_ms };
  }

protected:

  /**
   * Call it in your destructor if you override the on_start and/or on_stop
   * methods
   */
  void
  safe_delete() {
    stop();
    join();
  }

  /**
   * on stop callback handler. You should override this method if you need to do
   * some cleanup
   *
   * Note:
   * If you override on_start and/or on_stop functions, do not forget to call
   * the "safe_delete" method on destructor
   */
  virtual void
  on_stop() { }

  /**
   * on start callback handler.
   * You can override this method if you need to do some initialization
   *
   * Note:
   * If you override on_start and/or on_stop functions, do not forget to call
   * the "safe_delete" method on destructor
   */
  virtual void
  on_start() { }

  /**
   * It wakes up the internal conditional variable
   */
  inline void
  notify() {
    _forced_awaken = true;
    _cv.notify_one();
  }

  private:

  virtual int
  run(void) final override {
    using std::chrono::milliseconds;
    int return_value{ EXIT_SUCCESS };
    milliseconds remaing_time{ _interval_ms };

    _on_start();
    _is_running = true;
    tools::timestamp_t timestamp;
    while (_is_running) {
      std::unique_lock<std::mutex> guard{ _deletion_mtx };
      auto cv_status = _cv.wait_for(guard, remaing_time);
      if (_is_running) {
        if ((std::cv_status::timeout == cv_status || _forced_awaken)) {
          return_value   = on_interval();
          _forced_awaken = false;
          _is_running    = _is_running && (return_value == EXIT_SUCCESS);
          remaing_time   = _interval_ms;
        }
        else {
          remaing_time -= (timestamp.difference<>());
          remaing_time  = std::min(_interval_ms, remaing_time);
        }
        timestamp.reset();
      }
    }
    _on_stop();
    return return_value;
  } // LCOV_EXCL_LINE

  inline void
  _on_start() {
    std::unique_lock<std::mutex> guard{ _deletion_mtx };
    if (!_deleted) {
      on_start();
    }
  }

  inline void
  _on_stop() {
    std::unique_lock<std::mutex> guard{ _deletion_mtx };
    if (!_deleted) {
      on_stop();
    }
  }

  std::atomic_bool              _forced_awaken{false};
  std::atomic_bool              _deleted{ false };
  std::atomic_bool              _is_running{false};
  std::chrono::milliseconds     _interval_ms{ 100 };
  std::condition_variable       _cv;
  std::mutex                    _mtx;
  std::mutex                    _deletion_mtx;
};

class timer_t : public base_timer_t {
  std::function<bool()> _func;
public:
  timer_t(size_t timeout_ms, std::function<bool()> func)
      : base_timer_t{ timeout_ms }, _func(func)
  { }

  inline virtual int
  on_interval()
  override {
    return _func() ? EXIT_SUCCESS : EXIT_FAILURE;
  }
};

}
}
