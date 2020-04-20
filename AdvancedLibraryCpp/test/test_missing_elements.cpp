#include "test_missing_elements.h"
#include "../classic/sequences/missing_element.h"
#include <vector>
#include <array>

using namespace advanced;

TestMissingElements::
TestMissingElements(QObject *parent) : QObject(parent) { }

void TestMissingElements::
test_find_missing_element_of_sequence_1_N() {
  std::vector<int>    numbers_vector { 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12 };
  std::array<int, 11> numbers_array  { 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12 };
  const int expected{ 7 };
  auto res = find_missing_arithmetic_in_sorted(numbers_vector.begin(),
                                               numbers_vector.end());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);

  res = find_missing_arithmetic_in_sorted(numbers_array.begin(),
                                          numbers_array.end());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);
}

void TestMissingElements::
test_find_missing_element_of_sequence_not_starting_in_1() {
  std::vector<int>    numbers_vector { 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15 };
  std::array<int, 11> numbers_array { 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15 };
  const int expected{ 7 };
  auto res = find_missing_arithmetic_in_sorted(numbers_vector.begin(),
                                               numbers_vector.end(),
                                               1,
                                               numbers_vector.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);

  res = find_missing_arithmetic_in_sorted(numbers_array.begin(),
                                          numbers_array.end(),
                                          1,
                                          numbers_array.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);
}

void TestMissingElements::
test_find_missing_element_of_arithmetic_progression() {
  std::vector<int>    numbers_vector { 3, 5, 7, 9, 11, 13, 17, 19, 21, 23, 25 };
  std::array<int, 11> numbers_array  { 3, 5, 7, 9, 11, 13, 17, 19, 21, 23, 25 };
  const int expected{ 15 };
  auto res = find_missing_arithmetic_in_sorted(numbers_vector.begin(),
                                               numbers_vector.end(),
                                               2,
                                               numbers_vector.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);

  res = find_missing_arithmetic_in_sorted(numbers_array.begin(),
                                          numbers_array.end(),
                                          2,
                                          numbers_array.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);
}

void TestMissingElements::
test_find_missing_element_of_int_geometric_progression() {
  std::vector<int>   numbers_vector { 3, 9, 27, 81, 243, 2187 };
  std::array<int, 6> numbers_array  { 3, 9, 27, 81, 243, 2187 };
  const int expected{ 729 };
  auto res = find_missing_geometric_in_sorted(numbers_vector.begin(),
                                              numbers_vector.end(),
                                              3,
                                              numbers_vector.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);

  res = find_missing_geometric_in_sorted(numbers_array.begin(),
                                         numbers_array.end(),
                                         3,
                                         numbers_array.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);
}

void TestMissingElements::
test_find_missing_element_of_double_geometric_progression() {
  std::vector<double>   numbers_vector { 3.0, 9.0, 27.0, 81.0, 243.0, 2187.0 };
  std::array<double, 6> numbers_array  { 3.0, 9.0, 27.0, 81.0, 243.0, 2187.0 };
  const double expected{ 729.0 };
  auto res = find_missing_geometric_in_sorted(numbers_vector.begin(),
                                              numbers_vector.end(),
                                              3.0,
                                              numbers_vector.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);

  res = find_missing_geometric_in_sorted(numbers_array.begin(),
                                         numbers_array.end(),
                                         3.0,
                                         numbers_array.front());

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);
}

void TestMissingElements::
test_find_missing_element_of_unsorted_values() {
  std::vector<int>    numbers_vector { 13, 14, 8, 6, 3, 7, 4, 5, 10, 11, 12 };
  std::array<int, 11> numbers_array  { 13, 14, 8, 6, 3, 7, 4, 5, 10, 11, 12 };
  const int expected{ 9 };
  auto res = find_missing_sequential_range(numbers_vector.begin(),
                                           numbers_vector.end(),
                                           3);

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);

  res = find_missing_sequential_range(numbers_array.begin(),
                                      numbers_array.end(),
                                      3);

  QVERIFY(res.first);
  QCOMPARE(res.second, expected);
}

void TestMissingElements::
test_find_not_common_element() {
  std::vector<int> numbers_vector1 { 13, 14, 8, 6, 3, 7, 4, 5, 10, 11, 12 };
  std::vector<int> numbers_vector2 { 13, 14, 8, 6, 3, 7, 4, 5, 10, 11     };
  const int expected{ 12 };
  auto res = find_not_common_element(numbers_vector1.begin(),
                                     numbers_vector1.end(),
                                     numbers_vector2.begin(),
                                     numbers_vector2.end());

  QCOMPARE(res, expected);
}

void TestMissingElements::
test_find_not_repeated_element() {
  std::vector<int>    numbers_vector {
    13, 14, 8, 6, 3, 7, 4, 5, 10, 11, 12,
    8, 6, 4, 3, 13, 5, 10, 11, 14, 12
  };
  const int expected{ 7 };
  auto res = find_non_repeated_element(numbers_vector.begin(),
                                       numbers_vector.end());

  QCOMPARE(res, expected);
}

void TestMissingElements::
test_non_valid_sequences() {
  std::vector<std::pair<std::vector<int>, std::pair<bool, int>>> invalids {
    {{ 1, 4, 2, 32, 2, 65, 100 }, { true, 2  } }, // not sorted
    {{ 4, 5, 6, 7, 8, 9, 10    }, { true, 2  } }, // not passing 3, or 4 as lower
    {{ 1, 3, 5, 7, 9, 11, 13   }, { true, 2  } }, // not passing factor as 2
    {{ 1 }, { false, 1 }}                         // unique
  };

  for (const auto& entry : invalids) {
    auto& invalid  = entry.first;
    auto& expected = entry.second;
    try {
      auto r = find_missing_arithmetic_in_sorted(invalid.begin(), invalid.end());
      QCOMPARE(r.first,  expected.first);
      QCOMPARE(r.second, expected.second);
    }
    catch(const invalid_sequence_t&) {

    }
    catch (...) {
      QFAIL("Invalid exception");
    }
  }
}
