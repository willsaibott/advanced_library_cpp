#include <string>

namespace advanced {
namespace tools {

  /**
   * Usage:
   * In dummy.h:
   * #include <algorithm>
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
   * in main.cpp:
   * #include "dummy.h"
   * #include "enum_factory.h"
   * int main() {
   *     const dummy d  = advanced::from_string<dummy>("second"); // dummy::second;
   *     const dummy d2 = advanced::from_string<dummy, dummy::first, dummy::first>("second"); // first
   * }
   *
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
