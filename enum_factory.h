#include <string>

namespace advanced {

  /**
   * Usage:
   * enum class dummy {
   *  first,
   *  second,
   *  third,
   *  ...
   *  MAX
   * }
   *
   * namespace std {
   *
   *  inline string
   *  to_string(const dummy& d) {
   *    static std::string names[static_cast<dummy::MAX>] = {
   *      "first",
   *      "second",
   *      "third",
   *      ...
   *      return names[static_cast<size_t>(d)];
   *    }
   *  }
   * }
   *
   * int main() {
   *     dummy t = from_string("second"); // dummy::second;
   * }
   */
  template <class T, T minimum = static_cast<T>(0), T maximum = T::MAX>
  inline T
  from_string(const std::string& str) {
    T result{T::MAX};
    for (T value = static_cast<T>(0);
           value < T::MAX;
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