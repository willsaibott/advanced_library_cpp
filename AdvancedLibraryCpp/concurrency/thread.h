#pragma once
#include <functional>
#include <thread>

namespace advanced {
namespace concurrency {

  /**
   * std::thread Wrapper class
   */
  class thread_t {
    public:

    virtual ~thread_t() {
      if (_t.joinable()) {
        _t.join();
      }
    }

    void
    start() {
      _exit_code = NOT_FINISHED;
      _t = std::thread{ [this]() { _exit_code = run(); } };
    }

    std::thread&
    instance() {
      return _t;
    }


    virtual void
    join() final {
      if (_t.joinable()) {
        _t.join();
      }
    }

    bool
    joinable() const noexcept {
      return _t.joinable();
    }

    int
    exit_code() const noexcept {
      return _exit_code;
    }

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
