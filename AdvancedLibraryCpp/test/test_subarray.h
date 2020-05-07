#pragma once

#include <QObject>
#include <QTest>

class TestSubArray : public QObject
{
  Q_OBJECT
public:
  explicit TestSubArray(QObject *parent = nullptr);

signals:

private slots:
  void test_max_length_of_subvector_of_sum_zero();
  void test_max_length_of_subvector_of_sum_non_zero();
  void test_max_length_of_subarray_of_sum_zero();
  void test_max_length_of_subarray_of_sum_non_zero();
  void test_max_sum_of_subarray();
  void test_max_length_sum_full_array();
};

