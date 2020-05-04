#include <string>

namespace advanced {
namespace tools {

/** @test TestEnum in test/test_enum(.h|.cpp) */

/**
 *
 * @brief It gets a enum class value from std::string if there is a
 *        std::to_string function defined for this enum class
 *
 * Usage:
 * In dummy.h:
 * #include <algorithm>
 * // define your enum:
 * enum class dummy {
 *  first,
 *  second,
 *  third,
 *  ...
 *  MAX
 * };
 *
 * namespace std {
 *
 *  // define std::to_string function for it:
 *  inline string
 *  to_string(const dummy& d) {
 *    static std::string names[static_cast<size_t>(dummy::MAX)+1ul] = {
 *      "first",
 *      "second",
 *      "third",
 *      ...
 *       "MAX"
 *      };
 *      return names[std::min(static_cast<size_t>(d), static_cast<size_t>(dummy::MAX))];
 *    }
 *  }
 * }
 * ...
 *
 * // use it in main.cpp:
 * #include "dummy.h"
 * #include "enum_factory.h"
 * int main() {
 *     const dummy d  = advanced::from_string<dummy>("second"); // dummy::second;
 *     const dummy d2 = advanced::from_string<dummy, dummy::first, dummy::first>("second"); // first
 * }
 *
 * @see TestEnum at tests/test_enum.h for more usages
 */
template <class T, T minimum = static_cast<T>(0), T maximum = T::MAX>
inline T
from_string(const std::string& str) {
  T result{ maximum };
  for (T value = minimum;
         value < maximum;
         value = static_cast<T>(static_cast<size_t>(value)+1))
  {
    if (str == std::to_string(value)) {
      result = value;
      break;
    }
  }
  return result;
}

}
}
