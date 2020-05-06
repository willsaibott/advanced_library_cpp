#pragma once

#include <cinttypes>
#include <random>
#include <iostream>
#include <string>

#include "types.h"
namespace advanced {
namespace tools {

/** @test TestRandom in test/test_random(.h|.cpp) */

template <typename int_type>
inline int_type
next_random(ul start, ul end);

inline double
next_random_double(double start, double end);

inline std::string
next_random_string(size_t count, const std::string& character_set = "");

inline char
next_random_char(const std::string& character_set = "");

/**
 * @brief It generates a random number between (inclusive) "start" and "end"
 * params
 *
 * @param start begin of random range, this is inclusive
 * @param end   end of random range, this is inclusive
 */
template <typename int_type = ul>
inline int_type
next_random(ul start, ul end) {
  static std::random_device dev;
  static std::mt19937       rng(dev());
  using distribution = std::uniform_int_distribution<std::mt19937::result_type>;

  distribution dist(start, end);
  return static_cast<int_type>(dist(rng));
} // LCOV_EXCL_LINE

/**
 * @brief It generates a random number between (inclusive) "start" and "end"
 * params
 *
 * @param start begin of random range, this is inclusive
 * @param end   end of random range, this is inclusive
 */
inline double
next_random_double(double start, double end) {
  static std::random_device dev;
  static std::mt19937       rng(dev());
  using distribution = std::uniform_real_distribution<>;

  distribution dist(start, end);
  return dist(rng);
} // LCOV_EXCL_LINE

/**
 * @brief It generates a random std::string of "count" size using the
 * character_set as the possible chars that could occur in the string.
 * If no character set is passed, All printable chars in ascii table will be
 * used
 *
 * @param count         size of the generated random std::string
 * @param character_set possible chars, default: all chars
 */
inline std::string
next_random_string(size_t count, const std::string& character_set) {
  static std::random_device dev;
  static std::mt19937       rng(dev());
  using distribution = std::uniform_int_distribution<std::mt19937::result_type>;

  std::string output(count, ' ');
  if (character_set.empty()) {
    for (size_t ii = 0; ii < count; ii++) {
      output[ii] = next_random_char();
    }
  }
  else {
    distribution dist(0, character_set.size()-1);
    for (size_t ii = 0; ii < count; ii++) {
      output[ii] = character_set[dist(rng)];
    }
  }
  return output;
} // LCOV_EXCL_LINE

/**
 * @brief It generates a random char using the character set as the possible
 * chars. If the character_set is ommitted, all the printable chars in
 * ascii table will be used.
 *
 * @param character_set possible chars, default: all chars
 */
inline char
next_random_char(const std::string& character_set) {
  static std::random_device dev;
  static std::mt19937       rng(dev());
  using distribution = std::uniform_int_distribution<std::mt19937::result_type>;
  char output;

  if (character_set.empty()) {
    distribution dist(static_cast<ul>(' '), static_cast<ul>('~'));
    output = static_cast<char>(dist(rng));
  }
  else {
    distribution dist(0, character_set.size()-1);
    output = character_set[dist(rng)];
  }
  return output;
} // LCOV_EXCL_LINE

}
}
