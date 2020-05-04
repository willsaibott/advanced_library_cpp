#pragma once
#include <functional>
#include <thread>

namespace advanced {
namespace concurrency {

/**
 * @brief std::thread Wrapper class.
 *
 * Override the virtual run method and your class will be Java-like
 * @example
 *
 * class MyThread : public thread_t {
 *  virtual int
 *  run() override {
 *    std::cout << "Hello world from another thread!" << std::endl
 *  }
 * };
 *
 * later, in some random cpp file:
 * MyThread t;
 * t.start();
 * t.join(); // If you call the join method inside your class destructor,
 *           // you do not need to call it before the scope goes away.
 *
 */
class thread_t {
  public:

  /**
   * @brief ~thread_t   It calls join if the thread is joinable
   */
  virtual
  ~thread_t() {
    if (_t.joinable()) {
      _t.join();
    }
  }

  /**
   * @brief start it creates another thread that will run everything the context
   * that is implemented in the "run" method of your class
   */
  void
  start() {
    _exit_code = NOT_FINISHED;
    _t = std::thread{ [this]() { _exit_code = run(); } };
  }

  /**
   * @brief instance  returns a reference to the original std::thread object
   *                  created
   * @return returns the original std::thread object created
   */
  std::thread&
  instance() {
    return _t;
  }

  /**
   * @brief join    It joins to this thread if joinable
   */
  virtual void
  join() final {
    if (_t.joinable()) {
      _t.join();
    }
  }

  /**
   * @brief joinable   joinable wrapper for std::thread::joinable()
   * @return  whether the thread is joinable or not
   */
  bool
  joinable() const noexcept {
    return _t.joinable();
  }

  /**
   * @brief exit_code  it returns the exit code from the executed thread
   * @return NOT_FINISHED if the thread is executing, NOT_STARTED if it didn't
   *         start, or the return value from the "run" method
   */
  int
  exit_code() const noexcept {
    return _exit_code;
  }

  /**
   * @brief is_running  whether the current thread is running or not
   * @return whether the current thread is running or not
   */
  bool
  is_running() const noexcept {
    return _exit_code == NOT_FINISHED;
  }

  static const int NOT_FINISHED;
  static const int NOT_STARTED;

protected:

  /**
   * Override it and implement your own thread class
   */
  virtual int
  run() = 0;

  private:

  std::thread _t;
  int         _exit_code{ NOT_STARTED };
};

inline const int thread_t::NOT_FINISHED{ -100 };
inline const int thread_t::NOT_STARTED{ -101 };

}
}
