#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>
#include "../../tools/types.h"

namespace advanced {
namespace manipulators {

  template <typename number_t = int>
  class subarray_t {

  public:

    template <class ForwardIt>
    static inline size_t
    max_length_of_sum(ForwardIt first, ForwardIt last, number_t required_sum) {
      using cache_t = std::unordered_map<long long, size_t>;
      auto size = std::distance(first, last);
      size_t max_length { 0 };
      number_t sum { 0 };
      cache_t first_occurrence(size);

      for (auto it = first; it != last; it++) {
        auto ii = std::distance(first, it);
        sum += *it;
        if (sum == required_sum) {
          max_length = ii + 1ul;
        }
        else {
          auto res = first_occurrence.find(sum - required_sum);
          if (res != first_occurrence.end()) {
            max_length = std::max(max_length, ii - res->second);
          }
          else if (required_sum == 0 || !first_occurrence.count(sum)){
            first_occurrence[sum] = ii;
          }
        }
      }

      return max_length;
    }

    template <class ForwardIt>
    static inline number_t
    max_sum_of(ForwardIt first, ForwardIt last) {
      if (first == last) return static_cast<number_t>(0);
      number_t max_sum { *first };
      number_t current_max { *first };
      for (auto it = first; it != last; it++) {
        current_max = std::max(*it, current_max + *it);
        max_sum     = std::max(current_max, max_sum);
      }
      return max_sum;
    }
  };
}
}
