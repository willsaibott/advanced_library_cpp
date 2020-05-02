#pragma once

#include <cinttypes>
#include <random>
#include <iostream>
#include <string>

#include "types.h"
namespace advanced {
namespace tools {

  template <typename int_type = ul>
  inline int_type
  next_random(ul start, ul end) {
    static std::random_device dev;
    static std::mt19937       rng(dev());
    using distribution = std::uniform_int_distribution<std::mt19937::result_type>;

    distribution dist(start, end);
    return static_cast<int_type>(dist(rng));
  }

  inline double
  next_random_double(double start, double end) {
    static std::random_device dev;
    static std::mt19937       rng(dev());
    using distribution = std::uniform_real_distribution<>;

    distribution dist(start, end);
    return dist(rng);
  }

  inline std::string
  next_random_string(size_t count, const std::string& character_set) {
    static std::random_device dev;
    static std::mt19937       rng(dev());
    using distribution = std::uniform_int_distribution<std::mt19937::result_type>;

    if (character_set.empty()) { return ""; }

    std::string output(count, ' ');
    if (!character_set.empty()) {
      distribution dist(0, character_set.size()-1);

      for (size_t ii = 0; ii < count; ii++) {
        output[ii] = character_set[dist(rng)];
      }
    }

    return output;
  }

  inline char
  next_random_char(const std::string& character_set) {
    static std::random_device dev;
    static std::mt19937       rng(dev());
    using distribution = std::uniform_int_distribution<std::mt19937::result_type>;

    if (character_set.empty()) { return '\0'; }
    distribution dist(0, character_set.size()-1);
    return character_set[dist(rng)];
  }
}
}