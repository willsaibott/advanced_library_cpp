#pragma once

#include <QObject>
#include <QTest>
#include "../tools/math_operation.h"

class TestMath : public QObject
{
  Q_OBJECT
public:
  explicit TestMath(QObject *parent = nullptr);

private slots:

  void test_sum();
  void test_subtraction();
  void test_multiplication();
  void test_division();
  void test_min();
  void test_max();
};

