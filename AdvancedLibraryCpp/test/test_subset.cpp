#include <vector>
#include <array>
#include "../classic/sequences/subset.h"
#include "test_subset.h"


using namespace advanced::sequences;

TestSubset::
TestSubset(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestSubset");
}

void TestSubset::
test_subset_of_vector_with_sum_zero() {
  const std::vector<long> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const long expected{ 8 };
  const auto actual =
      subset_t<long>::sum_of_subsets(input.begin(), input.end(), 0);
  QCOMPARE(actual, expected);
}

void TestSubset::
test_subset_of_vector_with_sum_non_zero() {
  const std::vector<long> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const long expected{ 8 };
  const auto actual =
      subset_t<long>::sum_of_subsets(input.begin(), input.end(), 2);
  QCOMPARE(actual, expected);
  // 2,0
  // 2
  // 2,3-3
  // 2,3-3, 0
  // 2,-3,-5,8
  // 2,-3,-5,8, 0
  // 10, -5, -3
  // 10, -5, -3, 0
}

void TestSubset::
test_subset_of_array_with_sum_zero() {
  const std::array<long, 8> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const long expected{ 8 };
  const auto actual =
      subset_t<long>::sum_of_subsets(input.begin(), input.end(), 0);
  QCOMPARE(actual, expected);
}

void TestSubset::
test_subset_of_array_with_sum_non_zero() {
  const std::array<long, 8> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const long expected{ 8 };
  const auto actual =
      subset_t<long>::sum_of_subsets(input.begin(), input.end(), 2);
  QCOMPARE(actual, expected);
  // 2,0
  // 2
  // 2,3-3
  // 2,3-3, 0
  // 2,-3,-5,8
  // 2,-3,-5,8, 0
  // 10, -5, -3
  // 10, -5, -3, 0
}
