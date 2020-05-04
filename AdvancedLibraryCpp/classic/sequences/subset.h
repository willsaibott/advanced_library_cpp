#pragma once
#include <algorithm>
#include <unordered_map>
#include <vector>
#include "../../tools/types.h"

namespace advanced {
namespace sequences {

  /**
   * Static class to perform classic subset operations like sum of subsets and
   * so on...
   */
  template <typename number_t = int>
  class subset_t {


  public:
    using sum_cache_t = std::vector<std::unordered_map<number_t, size_t>>;

    /**
     * Returns the number of subsets that has sum equals to the param "sum"
     * It uses Dynamic programming for solving this classic problem
     * If you need to get or pass the memorization object, use the second version
     * below
     */
    template <class ForwardIt>
    static inline size_t
    sum_of_subsets(ForwardIt first, ForwardIt last, number_t sum) {
      sum_cache_t cache(std::distance(first, last));
      return sum_of_subsets(first, last, first, sum, cache);
    }

    /**
     * Returns the number of subsets that has sum equals to the param "sum"
     * It uses Dynamic programming for solving this classic problem
     * If you need to get or pass the memorization object, use this version
     */
    template <class ForwardIt>
    static inline size_t
    sum_of_subsets(ForwardIt    first,
                   ForwardIt    last,
                   ForwardIt    it,
                   number_t     sum,
                   sum_cache_t& cache)
    {
      size_t result { 0 };
      if (it == last) {
        result = sum == 0;
      }
      else {
        auto ii     = std::distance(first, it);
        auto cached = cache[ii].find(sum);
        if (cached != cache[ii].end()) {
          result = cached->second;
        }
        else {
          result =
              cache[ii][sum] =
                  sum_of_subsets(first, last, it + 1, sum, cache) +
                  sum_of_subsets(first, last, it + 1, sum - *it, cache);
        }
      }
      return result;
    }
  };
}
}
