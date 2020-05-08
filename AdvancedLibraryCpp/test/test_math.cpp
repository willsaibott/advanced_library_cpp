#include "test_math.h"

using namespace advanced::tools;

TestMath::
TestMath(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestMath");
}

void TestMath::
test_sum() {
  static const sum_t<int> sum;
  static const sum_t<std::string> concat;
  QCOMPARE(sum(4, 5), 9);
  QCOMPARE(sum(4, 5, -3, 3, 0, 15, -9, -6), 9);
  QCOMPARE(sum(-6), -6);

  auto result = concat("Goku", " ", "is ", "stronger ", "than ", "superman");
  QCOMPARE(result, "Goku is stronger than superman");
  result = concat(result,  " ne");
  QCOMPARE(result, "Goku is stronger than superman ne");
}

void TestMath::
test_subtraction() {
  static const subtraction_t<int> operation;
  QCOMPARE(operation(4, 5), -1);
}

void TestMath::
test_multiplication() {
  static const multiplication_t<int>    operation;
  static const multiplication_t<double> operation2;
  QCOMPARE(operation(20), 20);
  QCOMPARE(operation(4, 5), 20);
  QCOMPARE(operation(4, 5, 8), 160);
  QCOMPARE(operation2(20.0), 20.0);
  QCOMPARE(operation2(2.0, 5.0), 10.0);
  QCOMPARE(operation2(4.0, 5.0, 8.0), 160.0);
}

void TestMath::
test_division() {
  static const division_t<int>    operation;
  static const division_t<double> operation2;
  QCOMPARE(operation(5), 5);
  QCOMPARE(operation(20, 5), 4);
  QCOMPARE(operation2(20.0), 20.0);
  QCOMPARE(operation2(20.0, 5.0), 4.0);
}

void TestMath::
test_min() {
  static const minimum_t<int>         operation;
  static const minimum_t<double>      operation2;
  static const minimum_t<std::string> operation3;
  QCOMPARE(operation(20, 5), 5);
  QCOMPARE(operation(160, 8, 5), 5);
  QCOMPARE(operation2(20.0, 5.0), 5.0);
  QCOMPARE(operation2(160.0, 8.0, 5.0), 5.0);
  QCOMPARE(operation3("Robben", "Batman"), "Batman");
  QCOMPARE(operation3("Goku", "Gohan", "Goten"), "Gohan");
}

void TestMath::
test_max() {
  static const maximum_t<int>         operation;
  static const maximum_t<double>      operation2;
  static const maximum_t<std::string> operation3;
  QCOMPARE(operation(20, 5), 20);
  QCOMPARE(operation(160, 8, 5), 160);
  QCOMPARE(operation2(20.0, 5.0), 20.0);
  QCOMPARE(operation2(160.0, 8.0, 5.0), 160.0);
  QCOMPARE(operation3("Robben", "Batman"), "Robben");
  QCOMPARE(operation3("Goku", "Gohan", "Goten"), "Goten");
}
