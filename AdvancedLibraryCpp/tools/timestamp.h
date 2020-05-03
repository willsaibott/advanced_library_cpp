#pragma once
#include <chrono>

namespace advanced {
namespace tools {

class timestamp_t {
public:
  timestamp_t() : _start(clock_t::now()) {}

  inline void reset() {
    _start = clock_t::now();
  }

  template<class duration_type = std::chrono::milliseconds> double
  elapsed() const {
    using std::chrono::duration_cast;
    return duration_cast<duration_type>(clock_t::now() - _start).count();
  }

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
