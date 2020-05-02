#pragma once
#include <vector>
#include <math.h>
#include <type_traits>
#include <stdexcept>

namespace advanced {
namespace sequences {

  class invalid_sequence_t : public std::runtime_error {
  public:
    invalid_sequence_t() : std::runtime_error ("Invalid sequence") {}
  };

  /**
   * find missing element for a arithmetic progression in a sorted array\vector
   * int_type_t: int type
   * Time complexity: O(log n)
   */
  template <class ForwardIt, typename int_type_t = int>
  inline std::pair<bool, int_type_t>
  find_missing_arithmetic_in_sorted(ForwardIt         first,
                                    ForwardIt         last,
                                    const int_type_t& factor = static_cast<int_type_t>(1),
                                    const int_type_t& lower  = static_cast<int_type_t>(1))
  {
    using diff = typename std::iterator_traits<ForwardIt>::difference_type;
    std::pair<bool, int_type_t> result{ false, lower };
    const size_t MAX_ITERATIONS = std::distance(first, last);
    size_t iteration{ 0 };
    int_type_t expected;
    ForwardIt mid, pit;
    diff step, count = MAX_ITERATIONS;

    if (*first != lower) {
      if (*first == lower + factor) {
        result = { true, lower };
      }
      else {
        throw invalid_sequence_t{};
      }
    }

    while (!result.first) {
      if (iteration++ >= MAX_ITERATIONS) {
        throw invalid_sequence_t{};
      }
      step     = count >> 1;
      mid      = first + step;
      expected = lower + std::distance(first, mid) * factor;
      if (mid == first) { break; }

      pit      = std::prev(mid);
      if (expected == *mid) {
        // missing number must be in right branch:
        count += step;
      }
      else {
        if (*pit == expected - factor) {
          // found it!
          result = { true, expected };
        }
        else {
          // missing number must be in left branch:
          count -= step;
        }
      }
    }
    return result;
  }

  /**
   * find missing element for a geometric progression in a sorted array\vector
   * of integers. Do not use it for float point types or any other kind of type
   * int_type_t: int type
   * Time complexity: O(log n)
   */
  template <class    ForwardIt,
            typename int_type_t = int,
            typename opt =
            typename
              std::enable_if<!std::is_floating_point<int_type_t>::value>::type
            >
  inline std::pair<bool, int_type_t>
  find_missing_geometric_in_sorted(ForwardIt         first,
                                   ForwardIt         last,
                                   const int_type_t& factor,
                                   const int_type_t& lower = static_cast<int_type_t>(1))
  {
    static_assert(std::is_integral<int_type_t>::value,
                  "function only valid for integer values");
    using diff = typename std::iterator_traits<ForwardIt>::difference_type;
    const size_t MAX_ITERATIONS = std::distance(first, last);
    size_t iteration{ 0 };
    std::pair<bool, int_type_t> result{ false, lower };
    int_type_t expected;
    ForwardIt mid, pit, nit;
    diff step, count = std::distance(first, last);

    if (*first != lower) {
      if (*first == lower * factor) {
        result = { true, lower };
      }
      else {
        throw invalid_sequence_t{};
      }
    }

    while (!result.first && count > 0) {
      if (iteration++ >= MAX_ITERATIONS) {
        throw invalid_sequence_t{};
      }
      step     = count >> 1;
      mid      = first + step;
      if (mid == first || mid == last) { break; }

      pit      = std::prev(mid);
      nit      = std::next(mid);
      expected = *mid * factor;
      if (*nit != expected) {
        // found it!
        result = { true, expected };
      }
      else {
        expected = *pit * factor;
        if (*mid != expected) {
          // found it!
          result = { true, expected };
        }
        else {
          expected = lower * std::pow(factor, std::distance(first, mid));
          if (*mid == expected) {
            // missing number must be in right branch:
            count += step;
          }
          else {
            // missing number must be in left branch:
            count -= step;
          }
        }
      }
    }
    return result;
  }

  /**
   * find missing element for a geometric progression in a sorted array\vector
   * of float points. Use find_missing_geometric_in_sorted<ForwardIt, int_type_t>
   * for integer types
   * float_type: double or float
   * Time complexity: O(log n)
   */
  template <class ForwardIt,
            typename float_type = double,
            typename opt =
            typename
               std::enable_if<std::is_floating_point<float_type>::value>::type>
  std::pair<bool, float_type>
  find_missing_geometric_in_sorted(ForwardIt          first,
                                   ForwardIt          last,
                                   const float_type&  factor,
                                   const float_type&  lower       = static_cast<float_type>(1.0),
                                   const float_type&  float_error = static_cast<float_type>(0.0001))
  {
    static_assert(std::is_floating_point<float_type>::value,
                  "function only valid for float/double values");
    using diff = typename std::iterator_traits<ForwardIt>::difference_type;

    const size_t MAX_ITERATIONS = std::distance(first, last);
    size_t iteration{ 0 };
    std::pair<bool, float_type> result{ false, lower };
    float_type expected;
    ForwardIt mid, pit, nit;
    diff step, count = std::distance(first, last);

    if (*first != lower) {
      if (*first == lower * factor) {
        result = { true, lower };
      }
      else {
        throw invalid_sequence_t{};
      }
    }

    while (!result.first && count > 0) {
      if (iteration++ >= MAX_ITERATIONS) {
        throw invalid_sequence_t{};
      }
      step     = count >> 1;
      mid      = first + step;
      if (mid == first || mid == last) { break; }

      pit      = std::prev(mid);
      nit      = std::next(mid);
      expected = *mid * factor;
      if (*nit > expected + float_error || *nit < expected - float_error) {
        // found it!
        result = { true, expected };
      }
      else {
        expected = *pit * factor;
        if (*mid > expected + float_error || *mid < expected - float_error) {
          // found it!
          result = { true, expected };
        }
        else {
          expected = lower * std::pow(factor, std::distance(first, mid));
          if (*mid <= expected + float_error || *mid >= expected - float_error) {
            // missing number must be in right branch:
            count += step;
          }
          else {
            // missing number must be in left branch:
            count -= step;
          }
        }
      }
    }
    return result;
  }

  /**
   * find missing sequential range using XOR in unsorted/sorted integer
   * arrays/vectors.
   * Time complexity: O(n)
   */
  template <class ForwardIt, typename int_type_t = int>
  inline std::pair<bool, int_type_t>
  find_missing_sequential_range(ForwardIt  first,
                                ForwardIt  last,
                                int_type_t lower = static_cast<int_type_t>(1)) {
    auto dist = std::distance(first, last);
    std::pair<bool, int_type_t> result { dist > 0, static_cast<int_type_t>(0) };

    if (result.first) {
      auto it = first;
      int_type_t elem = *it;

      while (++it != last) {
        elem ^= *(it);
      }

      auto max = lower + dist;
      for (int_type_t ii = lower; ii <= max; ii++) {
        elem ^= ii;
      }

      result.second = elem;
    }

    return result;
  }

  /**
   * find the element that is not in the other list using XOR in
   * unsorted/sorted integer arrays/vectors.
   * Time complexity: O(n)
   */
  template <class ForwardIt, typename int_type_t = int>
  int_type_t
  find_not_common_element(ForwardIt first_first,
                          ForwardIt last_first,
                          ForwardIt first_second,
                          ForwardIt last_second)
  {
    ForwardIt it    = first_first;
    int_type_t elem = *it;

    // iterating in the first:
    while (++it != last_first) {
      elem ^= *(it);
    }

    // iterating in the second:
    it = first_second;
    while (it != last_second) {
      elem ^= *(it++);
    }
    return elem;
  }

  /**
   * find the element that is not repeated in the list using XOR in
   * unsorted/sorted integer arrays/vectors.
   * Time complexity: O(n)
   */
  template <class ForwardIt, typename int_type_t = int>
  int_type_t
  find_non_repeated_element(ForwardIt first, ForwardIt last) {
    ForwardIt it    = first;
    int_type_t elem = *it;
    while (++it != last) {
      elem ^= *(it);
    }
    return elem;
  }
}
}
