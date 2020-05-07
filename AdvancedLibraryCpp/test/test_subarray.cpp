#include <vector>
#include <array>
#include "test_subarray.h"
#include "../classic/sequences/subarray.h"

using namespace advanced::sequences;

TestSubArray::
TestSubArray(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestSubArray");
}

void TestSubArray::
test_max_length_of_subvector_of_sum_zero() {
  const std::vector<long> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const size_t expected{ 4 };
  const auto actual =
      subarray_t<long>::max_length_of_sum(input.begin(), input.end(), 0);
  QCOMPARE(actual, expected);
}

void TestSubArray::
test_max_length_of_subvector_of_sum_non_zero() {
  const std::vector<long> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const size_t expected{ 6 };
  const auto actual =
      subarray_t<long>::max_length_of_sum(input.begin(), input.end(), 12);
  QCOMPARE(actual, expected);
}

void TestSubArray::
test_max_length_of_subarray_of_sum_zero() {
  const std::array<long, 8> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const size_t expected{ 4 };
  const auto actual =
      subarray_t<long>::max_length_of_sum(input.begin(), input.end(), 0);
  QCOMPARE(actual, expected);
}

void TestSubArray::
test_max_length_of_subarray_of_sum_non_zero() {
  const std::array<long, 8> input { 2, 3, -3, -5, 8, 0, 9, 10 };
  const size_t expected{ 5 };
  const auto actual =
      subarray_t<long>::max_length_of_sum(input.begin(), input.end(), 3);
  QCOMPARE(actual, expected);
}

void TestSubArray::
test_max_sum_of_subarray() {
  const std::vector<double> input { -2.0, -3.0, 4.0, -1.0, -2.0, 1.0, 5.0, -3.0 };
  const double expected { 7.0 };
  const auto actual =
      subarray_t<double>::max_sum_of(input.begin(), input.end());
  QCOMPARE(actual, expected);
}

void TestSubArray::
test_max_length_sum_full_array() {
  const std::vector<double> input { -2.0, -2.0, 4.0, -2.0, -2.0, 2.0, 1.0, 1.0 };
  const long expected { 8 };
  const auto actual =
      subarray_t<long>::max_length_of_sum(input.begin(), input.end(), 0.0);
  QCOMPARE(actual, expected);
}
