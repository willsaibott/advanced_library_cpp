#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>

namespace advanced {
  /**
   * @param[in] n                  number
   * @param[in] sorting (optional) whether the vector should be sorted
   * @return the divisors of the given number n
   */
  std::vector<uint64_t>
  divisors_of(uint64_t n, bool sorting = false) {
    std::vector<uint64_t> divisors;
    uint64_t square = static_cast<uint64_t>(std::sqrt(n));
    for(uint64_t ii = 2; ii <= square; ii++) {
      if(n % ii == 0ull) {
        divisors.push_back(ii);
        if(ii * ii != n) {
          divisors.push_back(n / ii);
        }
      }
    }
    if (sorting) {
      std::sort(divisors.begin(), divisors.end());
    }
    return divisors;
  }

  /**
   * @param[in] a first  number
   * @param[in] b second number
   * @return the greatest common divisor
   */
  uint64_t gcd(uint64_t a, uint64_t b) {
    return b == 0ull ? a : gcd(b, (a % b));
  }
}
