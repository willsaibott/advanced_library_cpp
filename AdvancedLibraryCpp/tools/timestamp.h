#pragma once
#include <chrono>

namespace advanced {
namespace tools {

/** @test Todo */

/**
 * @brief The timestamp_t class is a class for time measure using
 * std::high_resolution_clock that is a wrapper for OS specific tools for time
 * measuring.
 */
class timestamp_t {
public:
  timestamp_t() : _start(clock_t::now()) {}

  /**
   * @brief reset  it resets the internal std::high_resolution_clock
   */
  inline void
  reset() {
    _start = clock_t::now();
  }

  /**
   * @brief returns the time elapsed since the last reset, or the creation of
   * this object as a double after casting to the specific duration_type
   * specified in the template argument
   */
  template<class duration_type = std::chrono::milliseconds> double
  elapsed() const {
    using std::chrono::duration_cast;
    return duration_cast<duration_type>(clock_t::now() - _start).count();
  }

  /**
   * @brief returns the time elapsed since the last reset, or the creation of
   * this object as the specific duration_type specified in the template argument
   */
  template<class duration_type = std::chrono::milliseconds> duration_type
  difference() const {
    using std::chrono::duration_cast;
    return duration_cast<duration_type>(clock_t::now() - _start);
  }

private:
    using clock_t = std::chrono::high_resolution_clock;
    std::chrono::time_point<clock_t> _start;
};

}
}
